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
void init(){
	system("cls");
	now_hold=-1,board.clear(),combo=b2b=0;
	if(FSBorYPA) Interactive::setcol(-1);
	else Interactive::setcol(-2);
	for(int i=0;i<Bag*7;i++) bl[i]=i%7;
	for(int i=0;i<Bag;i++) shuffle(bl+i*7,bl+(i+1)*7,rd);
	for(int i=0;i<=mapHeightP;i++){
		Interactive::gotoxy(i+mapHeightN,Margin-1);
		std::cout<<"|+"[i==0||i==mapHeightP];
		for(int j=0;j<mapWidth;j++){
			if(i!=mapHeightP) std::cout<<"  ";
			else std::cout<<"--";
		}
		std::cout<<"|+"[i==0||i==mapHeightP];
		std::cout<<"\n";
	}
	if(Open_hold){
		for(int i=0;i<=5;i++){
			Interactive::gotoxy(i+mapHeightN,Margin-12);
			if(i==0) std::cout<<"+---HOLD---";
			else if(i!=5) std::cout<<"|";
			else std::cout<<"+----------";
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
		}
	}
	for(int i=0;i<Next_num;i++) Block::get_block(bl[i]).put_next(i);
}
}

#endif
