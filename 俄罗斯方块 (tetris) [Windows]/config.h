#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>
#include <vector>
#include <windows.h>

namespace Config{
int version=10; //Windows版本
std::string RotationSystem="SRS"; //旋转系统可选 SRS/ARS
double SPE=0.6; //默认下落速度，单位 秒/行
double SDF=0; //软降重力倍率，设为 0 即为正无穷
double DAS=0.1; //自动移动延迟，单位 秒
double ARR=0.022; //自动重复速率，单位 秒/列
double EPLD=0.5; //锁定延迟（移动重置）
int mapWidth=10; //地图宽度
int mapHeight=20; //地图高度
int Next_num=5; //Next 显示个数，设为 0 则不显示
int Open_hold=1; //是否允许 Hold
int EPLD_lim=15; //移动重置重置次数限制
int Invisible=0; //启用隐形块
int Ghost=1; //阴影块（方块落点）
int Model=0; //0:ZEN；1:40-lines；2:Blitz
int lim_line=40; //竞速行数
double lim_time=120; //闪电战时间，单位 秒
int Garbage=0; //0:无垃圾行；2:unclear（打出的攻击立即以垃圾行返回）；3:cheese-layer（固定垃圾行层数）；4:cheese-timer（定时增加垃圾行）
int layer_height=6; //固定垃圾行层数
double time_interval=1; //垃圾行增加速率
double cheese_messiness=1; //垃圾行混乱程度（0~1)
double garbage_multiple=0.5; //unclear 攻击倍率
int FSBorYPA=0; //开起来试试？（防教练用

constexpr int Bag=100000;
constexpr int Margin=30;
constexpr int mapHeightN = 8;
std::map<char,std::string>custom;
std::vector<char> KEY;

struct Temp{
	Temp(){
		/*
			键位配置：中括号内为【键值表】，等于号后为功能
			键值表：请自行搜索 “Windows 虚拟键值表”
			功能：
			"L": 向左移动
			"R": 向右移动
			"CW": 顺时针旋转
			"CCW": 逆时针旋转
			"F": 180 度旋转
			"SD": 软降
			"HD": 硬降
			"H": Hold
			"RE": 重开
		*/
		custom[VK_LEFT]="L";
		custom[VK_RIGHT]="R";
		custom[VK_UP]="CW";
		custom['Z']="CCW";
		custom['A']="F";
		custom[VK_DOWN]="SD";
		custom[VK_SPACE]="HD";
		custom['C']="H";
		custom['R']="RE";
		custom['P']="SET";
		// custom[VK_SHIFT]="H";
		for(auto x:custom) KEY.emplace_back(x.first);
	}
};

static Temp temp;

}

using namespace Config;

#endif
