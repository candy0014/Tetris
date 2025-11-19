#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include "config.h"
#include <iostream>

namespace Interactive{

bool keydown(int g) {
	#ifdef _WIN32
	return GetAsyncKeyState(g) & 0x8000;
	#endif
	#ifdef __linux__
	return keyhelper.keydown(g);
	#endif
}

void gotoxy(int x,int y){
	#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	HANDLE hConsoleOut;
	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);
	csbiInfo.dwCursorPosition.Y = x;
	csbiInfo.dwCursorPosition.X = y;
	SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition);
	#endif
	#ifdef __linux__
	std::cout<<"\x1b["<<x+1<<";"<<y+1<<"H",std::cout.flush();
	#endif
}

void go(int x,int y,int op=0){
	if((GarbageModel==1||GarbageModel==5||GarbageModel==6)&&y<0) y--;
	gotoxy(x+mapHeightN,y*2+Margin+op);
}
void go_hold(int x,int y,int type){
	go(x+3,y-4,(type!=0&&type!=3));
}
void go_next(int id,int x,int y,int type){
	go(x+3+id*3,y+mapWidth+2,(type!=0&&type!=3));
}
void rgb_set(int wr,int wg,int wb,int br=12,int bg=12,int bb=12){
	#ifdef _WIN32
	printf("\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm",wr,wg,wb,br,bg,bb);
	#endif
	#ifdef __linux__
	std::cout<<"\x1b[38;2;"+std::to_string(wr)+";"+std::to_string(wg)+";"+std::to_string(wb)+"m",std::cout.flush();
	#endif
}
void setcol(int type){
	if(type==-1){//grey
		rgb_set(36,38,41);
		return;
	}
	if(FSBorYPA){//black
		if(type==-4) rgb_set(36,38,41);
		else rgb_set(0,0,0);
		return;
	}
	if(type==-6){ //orange
		rgb_set(237,109,0);
		return;
	}
	if(type==-5){
		rgb_set(157,15,0);
		return;
	}
	if(type==-4){//red
		rgb_set(233,22,0);
		return;
	}
	if(type==-3){//white
		rgb_set(255,255,255);
		return;
	}
	if(type==-2){//yellow
		rgb_set(193,168,65);
		return;
	}
	if(RotationSystem=="SRS"||RotationSystem=="SRS+"){
		if(type==0) rgb_set(54,235,168);
		if(type==1) rgb_set(95,78,179);
		if(type==2) rgb_set(197,117,68);
		if(type==3) rgb_set(193,168,65);
		if(type==4) rgb_set(139,185,61);
		if(type==5) rgb_set(176,78,166);
		if(type==6) rgb_set(202,70,77);
	}
	if(RotationSystem=="ARS"){
		if(type==0) rgb_set(202,70,77);
		if(type==1) rgb_set(95,78,179);
		if(type==2) rgb_set(197,117,68);
		if(type==3) rgb_set(193,168,65);
		if(type==4) rgb_set(176,78,166);
		if(type==5) rgb_set(54,235,168);
		if(type==6) rgb_set(139,185,61);
	}
	if(type==7){
		rgb_set(78,78,78);
	}
}

}

#endif
