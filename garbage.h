#ifndef GARBAGE_H
#define GARBAGE_H
#include <random>
#include <iostream>
#include <algorithm>
#include "map.h"
namespace Garbage{
	std::mt19937 rd(std::chrono::steady_clock::now().time_since_epoch().count());
	int chessboard_flag;
	int last_hole[105];
	void init(){
		chessboard_flag=0;
		for(int i=0;i<mapWidth;i++) last_hole[i]=i;
		std::shuffle(last_hole,last_hole+mapWidth,rd);
	}
	void add_garbage(int height,map &mp,int flag_begin=0){
		if(GarbageModel==3&&CheeseModel<=0&&!flag_begin) return;
		height=std::min(height,mapHeight+mapHeightN);
		for(int i=-mapHeightN;i<mapHeight-height;i++){
			for(int j=0;j<mapWidth;j++) if(mp[i][j]!=mp[i+height][j]){
				Interactive::go(i,j),Interactive::setcol(mp[i+height][j]);
				Function::put_square(mp[i+height][j]!=-1&&(!Invisible||mp[i+height][j]==7));
			}
			mp[i]=mp[i+height];
		}
		if(GarbageModel==1||GarbageModel==2){
			std::shuffle(last_hole,last_hole+mapWidth,rd);
		}
		if((GarbageModel!=3&&GarbageModel!=4)||std::abs(CheeseModel)==1){
			for(int i=mapHeight-height;i<mapHeight;i++){
				if(GarbageModel==3||GarbageModel==4){
					if(rd()%1000000<1000000*CheeseMessiness){
						std::shuffle(last_hole,last_hole+mapWidth,rd);
					}
				}
				for(int j=0;j<mapWidth;j++) mp[i][j]=7;
				for(int j=0;j<std::min(mapWidth,std::max(1,HoleNum));j++) mp[i][last_hole[j]]=-1;
				for(int j=0;j<mapWidth;j++){
					Interactive::go(i,j),Interactive::setcol(mp[i][j]);
					Function::put_square(mp[i][j]!=-1);
				}
			}
		}
		else{
			for(int i=mapHeight-height;i<mapHeight;i++){
				chessboard_flag^=1;
				int vis[105],flag=1;
				memset(vis,0,sizeof(vis));
				for(int j=0;j<mapWidth;j++){
					if(std::abs(CheeseModel)==2){
						if(rd()%1000000<1000000*CheeseMessiness) vis[j]=1;
					}
					if(std::abs(CheeseModel)==3) vis[j]=(chessboard_flag^j)&1;
					if(!vis[j]) flag=0;
				}
				if(flag) vis[rd()%mapWidth]=0;
				for(int j=0;j<mapWidth;j++){
					mp[i][j]=(vis[j]==0)?-1:7;
					Interactive::go(i,j),Interactive::setcol(mp[i][j]);
					Function::put_square(mp[i][j]!=-1);
				}
			}
		}
	}
	void update_backfire(int x,int &cnt_backfire){
		if(x>cnt_backfire){
			for(int i=cnt_backfire+1;i<=x;i++) if(i<=mapHeight+mapHeightN){
				Interactive::go(mapHeight-i,0,-3),Interactive::setcol(-4);
				Function::put_square();
			}
		}
		else{
			for(int i=cnt_backfire;i>x;i--) if(i<=mapHeight+mapHeightN){
				Interactive::go(mapHeight-i,0,-3);
				Function::put_square(0);
			}
		}
		cnt_backfire=x;
	}
}

#endif