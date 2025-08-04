#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include <windows.h>
#include "config.h"

namespace Interactive{

bool keydown(char g) {
	return GetAsyncKeyState(g) & 0x8000;
}

void gotoxy(int x,int y){
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	HANDLE hConsoleOut;
	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);
	csbiInfo.dwCursorPosition.Y = x;
	csbiInfo.dwCursorPosition.X = y;
	SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition);
}

void go(int x,int y,int op=0){
	if(Garbage==1&&y<0) y--;
	gotoxy(x+mapHeightN,y*2+Margin+op);
}
void go_hold(int x,int y,int type){
	go(x+3,y-4,(type!=0&&type!=3));
}
void go_next(int id,int x,int y,int type){
	go(x+3+id*3,y+mapWidth+2,(type!=0&&type!=3));
}
void rgb_set(int wr,int wg,int wb,int br=12,int bg=12,int bb=12){
	printf("\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm",wr,wg,wb,br,bg,bb);
}
void setcol(int type){
	if(type==-4){//red
		rgb_set(202,70,77);
	}
	if(type==-3){//white
		rgb_set(255,255,255);
	}
	if(type==-2){
		rgb_set(204,204,204);
		return;
	}
	if(type==-1){//grey
		rgb_set(36,38,41);
		return;
	}
	if(FSBorYPA){//black
		rgb_set(0,0,0);
		return;
	}
	if(RotationSystem=="SRS"){
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
