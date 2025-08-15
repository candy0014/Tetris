#ifndef KEYHELPER_H
#define KEYHELPER_H

#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <atomic>
#include <mutex>

class KEYHELPER {
public:
	KEYHELPER(const char* device_path = "/dev/input/event0") 
		: fd(-1), running(false) {
		fd = open(device_path, O_RDONLY | O_NONBLOCK);
		unsigned long keybits[KEY_MAX/8/sizeof(unsigned long)] = {0};
		for (int i = 0; i < KEY_MAX; i++) {
			if (test_bit(i, keybits)) {
				key_states[i] = 0;
			}
		}
		running = true;
		monitor_thread = std::thread(&KEYHELPER::eventLoop, this);
	}
	~KEYHELPER() {
		running = false;
		if (monitor_thread.joinable()) {
			monitor_thread.join();
		}
		if (fd >= 0) {
			close(fd);
		}
	}
	bool keydown(int keycode) {
		std::lock_guard<std::mutex> lock(state_mutex);
		if (key_states.find(keycode) != key_states.end()) {
			return key_states[keycode] != 0;
		}
		return false;
	}
private:
	static bool test_bit(int bit, const unsigned long* array) {
		return (array[bit/(8*sizeof(unsigned long))] >> (bit % (8*sizeof(unsigned long)))) & 1;
	}
	void eventLoop() {
		struct input_event ev;
		while (running) {
			ssize_t n = read(fd, &ev, sizeof(ev));
			if (n < 0) {
				if (errno == EAGAIN) {
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
					continue;
				}
				break;
			}
			if (n != sizeof(ev)) {
				continue;
			}
			if (ev.type == EV_KEY) {
				std::lock_guard<std::mutex> lock(state_mutex);
				key_states[ev.code] = ev.value;
			}
		}
	}
	
private:
	int fd;
	std::atomic<bool> running;
	std::thread monitor_thread;
	std::mutex state_mutex;
	std::map<int, int> key_states;
};
#endif