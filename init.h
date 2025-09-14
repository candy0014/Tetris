#ifndef INIT_H
#define INIT_H

#include "block.h"
#include "timer.h"
#include "garbage.h"

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
std::mt19937 rd(std::chrono::steady_clock::now().time_since_epoch().count());
short now_hold;
int combo,b2b;
long long score;
double last_tim,last_tim2,begin_tim;
int cnt_block,cnt_atk;
int cnt_line;
void init(){
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
	Garbage::init();
	if(FSBorYPA) Interactive::setcol(-1);
	else Interactive::setcol(-3);
	for(int i=0;i<Bag*7;i++) bl[i]=i%7;
	for(int i=0;i<Bag;i++) shuffle(bl+i*7,bl+(i+1)*7,rd);
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
				if(GarbageModel!=1&&GarbageModel!=5){
					Interactive::gotoxy(i+mapHeightN,Margin-1);
					if(i!=5) std::cout<<"|";
					else std::cout<<"+";
					fflush(stdout);
				}
			}
			Interactive::gotoxy(i+mapHeightN,Margin-12-2*(GarbageModel==1||GarbageModel==5));
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
