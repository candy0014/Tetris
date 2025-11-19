#ifndef USER_CONFIG_H
#define USER_CONFIG_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
int defval[15]={VK_LEFT,VK_RIGHT,VK_DOWN,VK_SPACE,VK_UP,'Z','A','C','R','P'};
#endif
#ifdef __linux__
#include <linux/input.h>
int defval[15]={KEY_LEFT,KEY_RIGHT,KEY_DOWN,KEY_SPACE,KEY_UP,KEY_Z,KEY_A,KEY_C,KEY_R,KEY_P};
#endif
std::string control_name[15]={"L","R","SD","HD","CW","CCW","F","H","RE","SET"};

namespace UserConfig{

struct node{
	std::string name;
	int *a;
	double *b;
	std::string *c;
	node(std::string _name="",int *_a=NULL,double *_b=NULL,std::string *_c=NULL){
		name=_name,a=_a,b=_b,c=_c;
	}
};
std::vector<node>var;
void insert0(std::string name,int *a){var.emplace_back(node(name,a,NULL,NULL));}
void insert1(std::string name,double *a){var.emplace_back(node(name,NULL,a,NULL));}
void insert2(std::string name,std::string *a){var.emplace_back(node(name,NULL,NULL,a));}
std::string erase_space(std::string s){
	std::string t="";
	int i=0,j=(int)s.length()-1;
	while(i<(int)s.length()&&s[i]==' ') i++;
	while(j>=0&&s[j]==' ') j--;
	for(int k=i;k<=j;k++) t+=s[k];
	return t;
}

std::string server_ip=""; //联机的服务端地址，若不联机则随意
int LinuxKeyevent=4; //Linux 键盘操作获取位置，配置方法见 README。

int Model=0; //0:ZEN；1:40-lines；2:Blitz；-2:Blitz（固定为自定速度不变，无得分倍率）
int RacingDistance=40; //竞速行数
double BlitzTime=120; //闪电战时间，单位 秒

int GarbageModel=0;
//0:无垃圾行；1:backfire；2:unclear（打出的攻击立即以垃圾行返回）；
//3:cheese-layer（固定垃圾行层数）；4:cheese-timer（定时增加垃圾行）
//5:生存模式（测试版）（目前效果：每 0.1 秒进行一次判定，每次以基于 APM 的概率随机加入 1~5 行垃圾行，5 秒后未被抵消的垃圾行进入场地）
//6:联机模式（测试版）（详见 readme）
int CheeseModel=1;
//1:正常垃圾行；2:单块以 CheeseMessiness 的概率是垃圾；3:棋盘；
//-x:在 cheese-layer 模式下，初始 x 模式的消垃圾行后不会继续增长
int HoleNum=1; //在除了 cheese-layer 的 2/3 模式下，垃圾行的洞数；若为负数则洞的位置为一个区间
int LayerHeight=6; //固定垃圾行层数
double TimeInterval=3; //垃圾行增加速率
double CheeseMessiness=0.8; //垃圾行混乱程度 / Cheese模式为2时的垃圾块概率（0~1)
double GarbageMultiple=1; //unclear/backfire 攻击倍率
double SurvivalAPM=60; //生存模式的 APM

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
std::string RotationSystem="SRS+"; //旋转系统可选 SRS+/SRS/ARS

int Autoplay=0; //是否开启 Autoplay
double Autoplay_PPS=2.5; //Autoplay 的 PPS

int WindowsVersion=10; //Windows 版本
int FSBorYPA=0; //开起来试试？（防教练用

std::map<int,std::string>custom;

struct Temp{
	Temp(){
		insert0("Model",&UserConfig::Model);
		insert0("Racing Distance",&UserConfig::RacingDistance);
		insert1("Blitz Time",&UserConfig::BlitzTime);
		insert0("Garbage Model",&UserConfig::GarbageModel);
		insert0("Cheese Model",&UserConfig::CheeseModel);
		insert0("Num of Hole",&UserConfig::HoleNum);
		insert0("Layer Height",&UserConfig::LayerHeight);
		insert1("Time Interval",&UserConfig::TimeInterval);
		insert1("Cheese Messiness",&UserConfig::CheeseMessiness);
		insert1("Garbage Multiple",&UserConfig::GarbageMultiple);
		insert1("APM for Survival",&UserConfig::SurvivalAPM);
		insert1("Speed",&UserConfig::Speed);
		insert1("SDF",&UserConfig::SDF);
		insert1("DAS",&UserConfig::DAS);
		insert1("ARR",&UserConfig::ARR);
		insert0("Width",&UserConfig::mapWidth);
		insert0("Height",&UserConfig::mapHeight);
		insert0("Num of Next",&UserConfig::NextNum);
		insert0("Open Hold",&UserConfig::OpenHold);
		insert0("Invisible",&UserConfig::Invisible);
		insert0("Anti Gravity",&UserConfig::AntiGravity);
		insert0("Ghost",&UserConfig::Ghost);
		insert1("EPLD",&UserConfig::EPLD);
		insert0("Limit of EPLD",&UserConfig::EPLDLim);
		insert2("Rotation System",&UserConfig::RotationSystem);
		insert0("Autoplay",&UserConfig::Autoplay);
		insert1("PPS of Autoplay",&UserConfig::Autoplay_PPS);
		insert0("Windows Version",&UserConfig::WindowsVersion);
		insert0("FSBorYPA",&UserConfig::FSBorYPA);
		insert2("Server IP",&UserConfig::server_ip);
		insert0("Linux Keyevent",&UserConfig::LinuxKeyevent);
		std::ifstream conf;
		conf.open("user_config.txt",std::ios::in);
		std::string Line;
		while(getline(conf,Line)){
			std::string name="",tmp="";
			int fl=0;
			for(int i=0;i<(int)Line.length();i++){
				if(Line[i]=='='&&!fl){fl=1;continue;}
				if(!fl) name+=Line[i];
				else tmp+=Line[i];
			}
			name=erase_space(name),tmp=erase_space(tmp);
			if(tmp=="") continue;
			for(int i=0;i<(int)var.size();i++) if(var[i].name==name){
				if(var[i].a!=NULL){
					int v=0,flag=1;
					for(int j=0;j<(int)tmp.length();j++){
						if('0'<=tmp[j]&&tmp[j]<='9') v=v*10+tmp[j]-'0';
						if(tmp[j]=='-'&&v==0&&flag==1) flag=-1;
					}
					(*var[i].a)=v*flag;
				}
				if(var[i].b!=NULL){
					double v=0,w=1;int flag=0,flagg=1;
					for(int j=0;j<(int)tmp.length();j++){
						if('0'<=tmp[j]&&tmp[j]<='9'){
							if(!flag) v=v*10+tmp[j]-'0';
							else w/=10,v+=w*(tmp[j]-'0');
						}
						if(tmp[j]=='-'&&v==0&&flagg==1&&flag==0) flagg=-1;
						if(tmp[j]=='.') flag=1;
					}
					(*var[i].b)=v*flagg;
				}
				if(var[i].c!=NULL) (*var[i].c)=tmp;
				break;
			}
		}
		conf.close();
		conf.open("keybinding.txt",std::ios::in);
		for(int i=0,val;i<10;i++){
			conf>>val;
			if(val==-1) val=defval[i];
			custom[val]=control_name[i];
		}
	}
};

static Temp temp;

}

#endif
