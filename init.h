#ifndef INIT_H
#define INIT_H

#include "block.h"
#include "timer.h"
#include "garbage.h"
#include "setting.h"

#include <random>
#include <chrono>
#include <algorithm>
#include <unistd.h>
#include <cstdio>

#ifdef __linux__
#include <sys/ioctl.h>
#include <termios.h>
#endif

namespace Init{
short bl[Bag*7];
short now_hold;
int combo,b2b;
long long score;
double last_tim,last_tim2,begin_tim;
int cnt_block,cnt_atk;
int cnt_line;
int i_to_fit(int u,int l,int r){
	return std::min(std::max(u,l),r);
}
double d_to_fit(double u,double l,double r){
	return std::min(std::max(u,l),r);
}
void init(){
	Speed=UserConfig::Speed;
	if(Speed<0) Speed=1e9+5;
	SDF=d_to_fit(UserConfig::SDF,0,1e18);
	DAS=d_to_fit(UserConfig::DAS,0,1e18);
	ARR=d_to_fit(UserConfig::ARR,0,1e18);
	mapWidth=i_to_fit(UserConfig::mapWidth,4,100);
	mapHeight=i_to_fit(UserConfig::mapHeight,2,100);
	NextNum=i_to_fit(UserConfig::NextNum,0,100);
	OpenHold=i_to_fit(UserConfig::OpenHold,0,1);
	Invisible=i_to_fit(UserConfig::Invisible,0,1);
	AntiGravity=i_to_fit(UserConfig::AntiGravity,0,1);
	Ghost=i_to_fit(UserConfig::Ghost,0,1);
	EPLD=d_to_fit(UserConfig::EPLD,0,1e18);
	EPLDLim=i_to_fit(UserConfig::EPLDLim,0,1000000000);
	RotationSystem=UserConfig::RotationSystem;
	if(RotationSystem!="ARS") RotationSystem="SRS";
	WindowsVersion=i_to_fit(UserConfig::WindowsVersion,1,11);
	FSBorYPA=i_to_fit(UserConfig::FSBorYPA,0,1);
	flagBlitz=(UserConfig::Model==-2);
	Model=i_to_fit(std::abs(UserConfig::Model),0,2);
	RacingDistance=i_to_fit(UserConfig::RacingDistance,1,1000000000);
	BlitzTime=d_to_fit(UserConfig::BlitzTime,0,1e18);
	GarbageModel=i_to_fit(UserConfig::GarbageModel,0,6);
	CheeseModel=i_to_fit(UserConfig::CheeseModel,-3,3);
	if(CheeseModel==0||(GarbageModel!=3&&GarbageModel!=4)) CheeseModel=1;
	if(std::abs(CheeseModel)!=2&&std::abs(CheeseModel)!=3) HoleNum=i_to_fit(UserConfig::HoleNum,1,mapWidth-1);
	LayerHeight=i_to_fit(UserConfig::LayerHeight,0,mapHeight);
	TimeInterval=d_to_fit(UserConfig::TimeInterval,0,1e18);
	CheeseMessiness=d_to_fit(UserConfig::CheeseMessiness,0,1);
	GarbageMultiple=d_to_fit(UserConfig::GarbageMultiple,0,1e18);
	if(GarbageModel==6) Model=0;

	if(GarbageModel==6){
		Function::clear();
		Interactive::gotoxy(1,1);
		if(FSBorYPA) Interactive::setcol(-1);
		else Interactive::setcol(-3);
		std::cout<<"Waiting for another player...";fflush(stdout);
		std::string s;
		int fl=0;
		for(auto x:KEY){
			if(custom[x]=="SET"&&Interactive::keydown(x)){fl=-1;break;}
		}
		while((s=Function::receive())!="start1"){
			for(auto x:KEY){
				if(custom[x]=="SET"){
					if(!Interactive::keydown(x)) fl=0;
					if(Interactive::keydown(x)&&fl!=-1){Function::send("end_");Setting::setting();init();return;}
				}
			}
		}
		Function::send("start2");
		while((s=Function::receive())!="start3"){
			for(auto x:KEY){
				if(custom[x]=="SET"){
					if(!Interactive::keydown(x)) fl=0;
					if(Interactive::keydown(x)&&fl!=-1){Function::send("end_");Setting::setting();init();return;}
				}
			}
		}
	}

	int lines=std::max(32,mapHeight+mapHeightN+3),cols=std::max(100,Margin*2+mapWidth*2);
	#ifdef _WIN32
	std::string _l="",_c="";
	while(lines) _l=char(lines%10+'0')+_l,lines/=10;
	while(cols) _c=char(cols%10+'0')+_c,cols/=10;
	std::string tmp="mode con cols="+_c+" lines="+_l;
	system(tmp.c_str());
	#endif
	Function::clear();
	now_hold=-1,board.clear(),combo=b2b=0;
	cnt_block=cnt_atk=0;
	cnt_line=0;
	if(Model!=0) score=0;
	if(FSBorYPA) Interactive::setcol(-1);
	else Interactive::setcol(-3);
	for(int i=0;i<Bag*7;i++) bl[i]=i%7;
	if(GarbageModel==6){
		long long tmp=0;
		std::string s=Function::receive_();
		for(int i=0;s[i];i++) tmp=tmp*10+s[i]-'0';
		rd.seed((unsigned int)tmp);
		// system("clear");Interactive::setcol(-3);Interactive::gotoxy(1,1);std::cout<<tmp<<" "<<rd()<<"\n";//exit(0);
	}
	for(int i=0;i<Bag;i++) Function::shuffle(bl,i*7,(i+1)*7);
	Garbage::init();
	setvbuf(stdout,NULL,_IOFBF,4096);
	for(int i=0;i<=mapHeight;i++){
		Interactive::gotoxy(i+mapHeightN,Margin-1);
		std::cout<<"|+"[i==0||i==mapHeight];
		for(int j=0;j<mapWidth;j++){
			if(i!=mapHeight) std::cout<<"  ";
			else std::cout<<"--";
		}
		std::cout<<"|+"[i==0||i==mapHeight];
		std::cout<<std::endl;
	}
	if(OpenHold){
		for(int i=0;i<=5;i++){
			if(i>=mapHeight){
				if(GarbageModel!=1&&GarbageModel!=5&&GarbageModel!=6){
					Interactive::gotoxy(i+mapHeightN,Margin-1);
					if(i!=5) std::cout<<"|";
					else std::cout<<"+";
					fflush(stdout);
				}
			}
			Interactive::gotoxy(i+mapHeightN,Margin-12-2*(GarbageModel==1||GarbageModel==5||GarbageModel==6));
			if(i==0) std::cout<<"+---HOLD---";
			else if(i!=5) std::cout<<"|";
			else std::cout<<"+----------";
			fflush(stdout);
		}
	}
	if(NextNum){
		for(int i=0;i<=3*NextNum+2;i++){
			if(i>=mapHeight){
				Interactive::gotoxy(i+mapHeightN,Margin+mapWidth*2);
				if(i!=3*NextNum+2) std::cout<<"|";
				else std::cout<<"+";
				fflush(stdout);
			}
			Interactive::gotoxy(i+mapHeightN,Margin+mapWidth*2+1);
			if(i==0) std::cout<<"---NEXT---+";
			else if(i!=3*NextNum+2) std::cout<<"          |";
			else std::cout<<"----------+";
			fflush(stdout);
		}
	}
	for(int i=0;i<NextNum;i++) Block::get_block(bl[i]).put_next(i);
	if(GarbageModel==3){
		Garbage::add_garbage(LayerHeight,board,1);
	}
	Function::print_score(score);
	double _tim=0;
	if(Model==2) _tim=BlitzTime;
	Interactive::go(16,-5,-Function::get_dig((int)_tim));
	printf("%.2f S",_tim);fflush(stdout);
	Interactive::go(17,-5,-1);
	printf("0.00 PPS");fflush(stdout);
	if(Model==1){
		Interactive::go(18,-4,-Function::get_dig(Init::cnt_line)-Function::get_dig(RacingDistance));
		std::cout<<"0/"<<RacingDistance<<" LINES",fflush(stdout);
	}
	else{
		Interactive::go(18,-5,-1);
		printf("0.00 APM");fflush(stdout);
	}
	setvbuf(stdout,NULL,_IONBF,0);
}
}

#endif
