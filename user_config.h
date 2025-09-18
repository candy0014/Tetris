#ifndef USER_CONFIG_H
#define USER_CONFIG_H

#include <map>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#define KEY_UP VK_UP
#define KEY_DOWN VK_DOWN
#define KEY_ENTER VK_RETURN
#endif
#ifdef __linux__
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include "keyhelper.h"
KEYHELPER keyhelper("/dev/input/event4"); // 请按实际情况配置，见 readme
#endif

namespace UserConfig{

// =========================================下为配置部分=========================================

int Model=0; //0:ZEN；1:40-lines；2:Blitz；-2:Blitz（固定为自定速度不变，无得分倍率）
int RacingDistance=40; //竞速行数
double BlitzTime=120; //闪电战时间，单位 秒

int GarbageModel=0;
//0:无垃圾行；1:backfire；2:unclear（打出的攻击立即以垃圾行返回）；
//3:cheese-layer（固定垃圾行层数）；4:cheese-timer（定时增加垃圾行）
//5:生存模式（测试版）（目前效果：每 0.1 秒进行一次判定，每次有 1/30 的概率随机加入 1-4 行垃圾行，5 秒后未被抵消的垃圾行进入场地）
//6:联机模式（测试版）（详见 readme）
int CheeseModel=1;
//1:正常垃圾行；2:单块以 CheeseMessiness 的概率是垃圾；3:棋盘；
//-x:在 cheese-layer 模式下，初始 x 模式的消垃圾行后不会继续增长
int HoleNum=1; //在除了 cheese-layer 的 2/3 模式下，垃圾行的洞数；若为负数则洞的位置为一个区间
int LayerHeight=6; //固定垃圾行层数
double TimeInterval=3; //垃圾行增加速率
double CheeseMessiness=0.8; //垃圾行混乱程度 / Cheese模式为2时的垃圾块概率（0~1)
double GarbageMultiple=1; //unclear 攻击倍率

double Speed=0.6; //默认下落速度，单位 秒/行（若为负数则无重力）
double SDF=0; //软降重力倍率，设为 0 即为正无穷
double DAS=0.1; //自动移动延迟，单位 秒
double ARR=0.01; //自动重复速率，单位 秒/列

int mapWidth=10; //地图宽度
int mapHeight=20; //地图高度
int NextNum=5; //Next 显示个数，设为 0 则不显示
int OpenHold=1; //是否允许 Hold
int Invisible=0; //启用隐形块
int AntiGravity=0; //启用反重力
int Ghost=1; //阴影块（方块落点）
double EPLD=0.5; //锁定延迟（移动重置）
int EPLDLim=15; //移动重置重置次数限制
std::string RotationSystem="SRS"; //旋转系统可选 SRS/ARS

int WindowsVersion=10; //Windows版本
int FSBorYPA=1; //开起来试试？（防教练用

std::string server_id=""; //联机的服务端地址，若不联机则随意（需编译前配置）

std::map<char,std::string>custom;

struct Temp{
	Temp(){
		/*
			键位配置：中括号内为【键值表】，等于号后为功能
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
			"SET": 修改配置
		*/

		#ifdef _WIN32
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
		#endif
		#ifdef __linux__
		custom[KEY_LEFT]="L";
		custom[KEY_RIGHT]="R";
		custom[KEY_UP]="CW";
		custom[KEY_X]="CW";
		custom[KEY_Z]="CCW";
		custom[KEY_A]="F";
		custom[KEY_DOWN]="SD";
		custom[KEY_SPACE]="HD";
		custom[KEY_C]="H";
		custom[KEY_R]="RE";
		custom[KEY_P]="SET";
		#endif
	}
};

// =========================================上为配置部分=========================================

static Temp temp;

}

#endif
