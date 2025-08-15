#ifndef GARBAGE_H
#define GARBAGE_H

#include "map.h"
#include "config.h"
namespace Garbage{
	void add_garbage(int pos,int height,map &mp){
		height=std::min(height,mapHeight+mapHeightN);
		for(int i=-mapHeightN;i<mapHeight-height;i++){
			for(int j=0;j<mapWidth;j++) if(mp[i][j]!=mp[i+height][j]){
				Interactive::go(i,j),Interactive::setcol(mp[i+height][j]);
				Function::put_square(mp[i+height][j]!=-1&&(!Invisible||mp[i+height][j]==7));
			}
			mp[i]=mp[i+height];
		}
		for(int i=mapHeight-height;i<mapHeight;i++){
			for(int j=0;j<mapWidth;j++){
				mp[i][j]=(j==pos)?-1:7;
				Interactive::go(i,j),Interactive::setcol(mp[i][j]);
				Function::put_square(mp[i][j]!=-1);
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