#ifndef FUNCTION_H
#define FUNCTION_H

#include "interactive.h"
#include <iostream>
#ifdef __linux__
#include <termios.h>
#endif

namespace Function{
	void cur_hide(){
		#ifdef _WIN32
		CONSOLE_CURSOR_INFO cur={1,0};SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cur);
		#endif
		#ifdef __linux__
		struct termios term;
		tcgetattr(STDIN_FILENO, &term);
		term.c_lflag &= ~ECHO;
		term.c_lflag &= ~ICANON;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
		std::cout <<"\x1b[?25l";
		std::cout.flush();
		#endif
	}
	void cur_show(){
		#ifdef _WIN32
		CONSOLE_CURSOR_INFO cur={1,1};SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cur);
		#endif
		#ifdef __linux__
		struct termios term;
		tcgetattr(STDIN_FILENO, &term);
		term.c_lflag |= ECHO;
		term.c_lflag |= ICANON;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
		std::cout << "\x1b[?25h";
		std::cout.flush();
		#endif
	}
	void rgb_init(){
		#ifdef _WIN32
		HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD dwInMode, dwOutMode;
		GetConsoleMode(hIn, &dwInMode);
		GetConsoleMode(hOut, &dwOutMode);
		dwInMode |= 0x0200;
		dwOutMode |= 0x0004;
		SetConsoleMode(hIn, dwInMode);
		SetConsoleMode(hOut, dwOutMode);
		#endif
		#ifdef __linux__
		std::cout<<"\x1b[48;2;"<<12<<";"<<12<<";"<<12<<"m";
	    std::cout<<"\x1b[2J";
	    std::cout<<"\x1b[H";
	    std::cout.flush();
		#endif
	}
	void clear(){
		#ifdef _WIN32
		system("cls");
		#endif
		#ifdef __linux__
		system("clear");
		#endif
	}
	void put_square(int op=1){
		if(op){
			#ifdef _WIN32
			if(WindowsVersion<=10) std::cout<<"█ ";
			else std::cout<<"██";
			#endif
			#ifdef __linux__
			std::cout<<"██";
			// if(op==1) std::cout<<"██";
			// if(op==2) std::cout<<"🗖";
			#endif
			fflush(stdout);
		}
		else std::cout<<"  ",fflush(stdout);
	}
	int get_dig(long long x){
		if(x==0) return 1;
		int cnt=0;
		while(x) cnt++,x/=10;
		return cnt;
	}
	void print_score(long long &score){
		if(FSBorYPA) Interactive::setcol(-1);
		else Interactive::setcol(-3);
		int dig=get_dig(score);
		Interactive::gotoxy(mapHeight+mapHeightN+1,Margin+mapWidth-(dig+1)/2);
		std::cout<<score;fflush(stdout);
	}
	void send(std::string s){
		sock.send(server_ip,3000,s);
	}
	std::string receive(){
		return sock.receive().data;
	}
	std::string receive_(){
		std::string s;
		while((s=receive())=="fail");
		return s;
	}
	std::string receive_(std::string t){
		std::string s;
		while((s=receive())!=t);
		return s;
	}
	void receive_clear(){
		std::string s;
		while((s=receive())!="fail");
	}
	void shuffle(int a[],int l,int r){
		for(int i=l;i<r;i++){
			std::swap(a[i],a[rd()%(i-l+1)+l]);
		}
	}
	void shuffle(short a[],int l,int r){
		for(int i=l;i<r;i++){
			std::swap(a[i],a[rd()%(i-l+1)+l]);
		}
	}
}

#endif