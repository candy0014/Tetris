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
#define KEY_ENTER VK_RETURN
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
int FSBorYPA;

constexpr int Margin=30;
constexpr int mapHeightN = 8;
std::map<char,std::string>custom;
std::vector<char> KEY;

Socket::UDP sock;
std::string server_id="10.49.27.16";

std::mt19937 rd(std::chrono::steady_clock::now().time_since_epoch().count());

struct Temp{
	Temp(){
		custom=UserConfig::custom;
		for(auto x:custom) KEY.emplace_back(x.first);
		server_id=UserConfig::server_id;
		int my_port=rd()%5000+3001;
		sock.bind(my_port);
		sock.setNonBlocking();
		std::string s="";
		for(int i=1;i<=20;i++) s+=char(rd()%26+'a');
		sock.send(server_id,3000,s);
	}
};

static Temp temp;

}

using namespace Config;

#endif
