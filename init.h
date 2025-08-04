#ifndef INIT_H
#define INIT_H

#include "block.h"
#include "map.h"
#include "config.h"
#include "timer.h"

#include <random>
#include <chrono>
#include <algorithm>

namespace Init{
short bl[Bag*7];
std::mt19937 rd(std::chrono::steady_clock::now().time_since_epoch().count());
short now_hold;
int combo,b2b;
long long score;
void cur_hide(){CONSOLE_CURSOR_INFO cur={1,0};SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cur);}
void cur_show(){CONSOLE_CURSOR_INFO cur={1,1};SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cur);}
void rgb_init(){
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwInMode, dwOutMode;
	GetConsoleMode(hIn, &dwInMode);
	GetConsoleMode(hOut, &dwOutMode);
	dwInMode |= 0x0200;
	dwOutMode |= 0x0004;
	SetConsoleMode(hIn, dwInMode);
	SetConsoleMode(hOut, dwOutMode);
}
int get_dig(long long x){
	if(x==0) return 1;
	int cnt=0;
	while(x) cnt++,x/=10;
	return cnt;
}
void print_score(){
	if(FSBorYPA) Interactive::setcol(-1);
	else Interactive::setcol(-3);
	int dig=get_dig(score);
	Interactive::gotoxy(mapHeight+mapHeightN+1,Margin+mapWidth-(dig+1)/2);
	std::cout<<score;fflush(stdout);
}
std::mt19937 rnd(time(0));
void add_garbage(int pos,int height,map &mp){
	height=std::min(height,mapHeight+mapHeightN);
	for(int i=-mapHeightN;i<mapHeight-height;i++){
		for(int j=0;j<mapWidth;j++) if(mp[i][j]!=mp[i+height][j]){
			Interactive::go(i,j);
			Interactive::setcol(mp[i+height][j]);
			if(mp[i+height][j]==-1) std::cout<<"  ";
			else{
				if(!Invisible||mp[i+height][j]==7){
					if(version<=10) printf("█ ");//std::cout<<"█ ";
					else printf("██");//std::cout<<"██";
				}
				else std::cout<<"  ";
			}
			fflush(stdout);
		}
		mp[i]=mp[i+height];
	}
	for(int i=mapHeight-height;i<mapHeight;i++){
		for(int j=0;j<mapWidth;j++){
			mp[i][j]=(j==pos)?-1:7;
			Interactive::go(i,j),Interactive::setcol(mp[i][j]);
			if(mp[i][j]==-1) std::cout<<"  ";
			else{
				if(version<=10) printf("█ ");//std::cout<<"█ ";
				else printf("██");//std::cout<<"██";
			}
			fflush(stdout);
		}
	}
}
double last_tim,last_tim2,begin_tim;
int last_hole;
int cnt_block,cnt_atk;
int cnt_line;
int cnt_backfire;
void update_backfire(int x){
	if(x>cnt_backfire){
		for(int i=cnt_backfire+1;i<=x;i++) if(i<=mapHeight+mapHeightN){
			Interactive::go(mapHeight-i,0,-3),Interactive::setcol(-4);
			if(version<=10) std::cout<<"█ ";
			else std::cout<<"██";
			fflush(stdout);
		}
	}
	else{
		for(int i=cnt_backfire;i>x;i--) if(i<=mapHeight+mapHeightN){
			Interactive::go(mapHeight-i,0,-3);
			std::cout<<"  ",fflush(stdout);
		}
	}
	cnt_backfire=x;
}
void init(){
	int lines=mapHeight+mapHeightN+3,cols=Margin*2+mapWidth*2;
	std::string _l="",_c="";
	while(lines) _l=char(lines%10+'0')+_l,lines/=10;
	while(cols) _c=char(cols%10+'0')+_c,cols/=10;
	std::string tmp="mode con cols="+_c+" lines="+_l;
	system(tmp.c_str());
	system("cls");
	now_hold=-1,board.clear(),combo=b2b=0;
	cnt_block=cnt_atk=0;
	cnt_line=0,cnt_backfire=0;
	if(Model!=0) score=0;
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
	if(Open_hold){
		for(int i=0;i<=5;i++){
			Interactive::gotoxy(i+mapHeightN,Margin-12-2*(Garbage==1));
			if(i==0) std::cout<<"+---HOLD---";
			else if(i!=5) std::cout<<"|";
			else std::cout<<"+----------";
			fflush(stdout);
		}
	}
	if(Next_num){
		for(int i=0;i<=3*Next_num+2;i++){
			if(i>=mapHeight){
				Interactive::gotoxy(i+mapHeightN,Margin+mapWidth*2);
				if(i!=3*Next_num+2) std::cout<<"|";
				else std::cout<<"+";
			}
			Interactive::gotoxy(i+mapHeightN,Margin+mapWidth*2+1);
			if(i==0) std::cout<<"---NEXT---+";
			else if(i!=3*Next_num+2) std::cout<<"          |";
			else std::cout<<"----------+";
			fflush(stdout);
		}
	}
	for(int i=0;i<Next_num;i++) Block::get_block(bl[i]).put_next(i);
	last_hole=rnd()%mapWidth;
	if(Garbage==3){
		for(int i=1;i<=layer_height;i++){
			if(rnd()%1000000<1000000*cheese_messiness) last_hole=rnd()%mapWidth;
			add_garbage(last_hole,1,board);
		}
	}
	print_score();
	double _tim=0;
	if(Model==2) _tim=lim_time;
	Interactive::go(16,-5,-Init::get_dig((int)_tim));
	printf("%.2f S",_tim);fflush(stdout);
	Interactive::go(17,-5,-1);
	printf("0.00 PPS");fflush(stdout);
	if(Model==1){
		Interactive::go(18,-4,-Init::get_dig(Init::cnt_line)-2);
		std::cout<<"0/"<<lim_line<<" LINES",fflush(stdout);
	}
	else{
		Interactive::go(18,-5,-1);
		printf("0.00 APM");fflush(stdout);
	}
	setvbuf(stdout,NULL,_IONBF,0);
}
}

#endif
