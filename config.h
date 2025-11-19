#ifndef CONFIG_H
#define CONFIG_H

#include "chrono"
#include <random>
#include <map>
#include <string>
#include <vector>
#include "socket.h"
#include "user_config.h"

#ifdef _WIN32
#include <windows.h>
#define KEY_UP VK_UP
#define KEY_DOWN VK_DOWN
#define KEY_LEFT VK_LEFT
#define KEY_RIGHT VK_RIGHT
#define KEY_ENTER VK_RETURN
#endif
#ifdef __linux__
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include "keyhelper.h"
#endif

namespace Config{

int Autoplay;
double Autoplay_PPS;

int Model;
int flagBlitz;
int RacingDistance;
double BlitzTime;

int GarbageModel;
int CheeseModel;
int HoleNum;
int LayerHeight;
double TimeInterval;
double CheeseMessiness;
double GarbageMultiple;
double SurvivalAPM;

double Speed;
double SDF;
double DAS;
double ARR;

int mapWidth;
int mapHeight;
int NextNum;
int OpenHold;
int Invisible;
int AntiGravity;
int Ghost;
double EPLD;
int EPLDLim;
std::string RotationSystem;

int WindowsVersion;
#ifdef __linux__
KEYHELPER keyhelper;
#endif
int FSBorYPA;
constexpr int Margin=30;
constexpr int mapHeightN = 8;
std::map<int,std::string>custom;
std::vector<int> KEY;

Socket::UDP sock;
std::string server_ip="";

std::mt19937 rd(std::chrono::steady_clock::now().time_since_epoch().count());

struct Temp{
	Temp(){
		#ifdef __linux__
		keyhelper.init(("/dev/input/event"+std::to_string(UserConfig::LinuxKeyevent)).c_str());
		#endif
		custom=UserConfig::custom;
		for(auto x:custom) KEY.emplace_back(x.first);
		server_ip=UserConfig::server_ip;
		int my_port=rd()%5000+3001;
		sock.bind(my_port);
		sock.setNonBlocking();
		std::string s="";
		for(int i=1;i<=20;i++) s+=char(rd()%26+'a');
		sock.send(server_ip,3000,s);
	}
};

static Temp temp;

}

using namespace Config;

#endif