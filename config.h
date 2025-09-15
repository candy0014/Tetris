#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>
#include <vector>
#include "user_config.h"

#ifdef _WIN32
#include <windows.h>
#define KEY_UP VK_UP
#define KEY_DOWN VK_DOWN
#define KEY_ENTER VK_RETURN
#endif

namespace Config{

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

constexpr int Bag=100000;
constexpr int Margin=30;
constexpr int mapHeightN = 8;
std::map<char,std::string>custom;
std::vector<char> KEY;

struct Temp{
	Temp(){
		custom=UserConfig::custom;
		for(auto x:custom) KEY.emplace_back(x.first);
	}
};

static Temp temp;

}

using namespace Config;

#endif
