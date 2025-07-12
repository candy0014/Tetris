#ifndef INIT_H
#define INIT_H

#include "block.h"
#include "map.h"
#include "config.h"

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
void print_score(){
	if(FSBorYPA) Interactive::setcol(-1);
	else Interactive::setcol(-2);
	int dig=(score==0);
	long long tmp=score;
	while(tmp) dig++,tmp/=10;
	Interactive::gotoxy(mapHeightP+mapHeightN+1,Margin+mapWidth-(dig+1)/2);
	std::cout<<score;fflush(stdout);
}
void init(){
	system("cls");
	now_hold=-1,board.clear(),combo=b2b=0;
	// score=0;
	if(FSBorYPA) Interactive::setcol(-1);
	else Interactive::setcol(-2);
	for(int i=0;i<Bag*7;i++) bl[i]=i%7;
	for(int i=0;i<Bag;i++) shuffle(bl+i*7,bl+(i+1)*7,rd);
	setvbuf(stdout,NULL,_IOFBF,4096);
	for(int i=0;i<=mapHeightP;i++){
		Interactive::gotoxy(i+mapHeightN,Margin-1);
		std::cout<<"|+"[i==0||i==mapHeightP];
		for(int j=0;j<mapWidth;j++){
			if(i!=mapHeightP) std::cout<<"  ";
			else std::cout<<"--";
		}
		std::cout<<"|+"[i==0||i==mapHeightP];
		std::cout<<std::endl;
	}
	if(Open_hold){
		for(int i=0;i<=5;i++){
			Interactive::gotoxy(i+mapHeightN,Margin-12);
			if(i==0) std::cout<<"+---HOLD---";
			else if(i!=5) std::cout<<"|";
			else std::cout<<"+----------";
			fflush(stdout);
		}
	}
	if(Next_num){
		for(int i=0;i<=3*Next_num+2;i++){
			if(i>=mapHeightP){
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
	print_score();
	setvbuf(stdout,NULL,_IONBF,0);
}
}

#endif
