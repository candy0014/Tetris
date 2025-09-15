#ifndef GARBAGE_H
#define GARBAGE_H
#include <random>
#include <iostream>
#include <algorithm>
#include <deque>
#include "map.h"
#include "block.h"
namespace Garbage{
	constexpr int BufferSize=1000;
	std::mt19937 rd(std::chrono::steady_clock::now().time_since_epoch().count());
	int chessboard_flag;
	int last_hole[BufferSize];
	struct Buffer{
		int cnt_backfire;
		double tim_backfire;
		int flag_hole;
	};
	std::deque<Buffer>buf;
	int arr_buf[BufferSize];
	void init(){
		chessboard_flag=0;
		for(int i=0;i<mapWidth;i++) last_hole[i]=i;
		std::shuffle(last_hole,last_hole+mapWidth,rd);
		buf.clear();
	}
	void add_garbage(int height,map &mp,int op=0){
		if(GarbageModel==3&&CheeseModel<=0&&!op) return;
		height=std::min(height,mapHeight+mapHeightN);
		for(int i=-mapHeightN;i<mapHeight-height;i++){
			for(int j=0;j<mapWidth;j++) if(mp[i][j]!=mp[i+height][j]){
				Interactive::go(i,j),Interactive::setcol(mp[i+height][j]);
				Function::put_square(mp[i+height][j]!=-1&&(!Invisible||mp[i+height][j]==7));
			}
			mp[i]=mp[i+height];
		}
		if(GarbageModel==1||GarbageModel==2||GarbageModel==5){
			if(op!=2) std::shuffle(last_hole,last_hole+mapWidth,rd);
		}
		if((GarbageModel!=3&&GarbageModel!=4)||std::abs(CheeseModel)==1){
			for(int i=mapHeight-height;i<mapHeight;i++){
				if(GarbageModel==3||GarbageModel==4){
					if(rd()%1000000<1000000*CheeseMessiness){
						std::shuffle(last_hole,last_hole+mapWidth,rd);
					}
				}
				for(int j=0;j<mapWidth;j++) mp[i][j]=7;
				if(HoleNum>=0){
					for(int j=0;j<std::min(mapWidth,std::max(1,HoleNum));j++) mp[i][last_hole[j]]=-1;
				}
				else{
					for(int j=0;j<mapWidth;j++) if(last_hole[j]+std::min(mapWidth,-HoleNum)<=mapWidth){
						for(int k=last_hole[j];k<last_hole[j]+std::min(mapWidth,-HoleNum);k++) mp[i][k]=-1;
						break;
					}
				}
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
	void update_buffer(){
		int _arr_buf[BufferSize];memset(_arr_buf,0,sizeof(_arr_buf));
		int x=0;
		double now=timer.get();
		for(auto y:buf){
			for(int i=x+1;i<=x+y.cnt_backfire&&i<BufferSize;i++){
				double tmp=now-y.tim_backfire;
				if(GarbageModel==1){
					if(tmp<=0.3) _arr_buf[i]=1;
					else _arr_buf[i]=2;
				}
				if(GarbageModel==5){
					if(tmp<=2.5) _arr_buf[i]=1;
					else if(tmp<=5) _arr_buf[i]=2;
					else _arr_buf[i]=3;
				}
			}
			x+=y.cnt_backfire;
		}
		for(int i=1;i<BufferSize;i++) if(arr_buf[i]!=_arr_buf[i]){
			arr_buf[i]=_arr_buf[i];
			if(i<=mapHeight+mapHeightN){
				Interactive::go(mapHeight-i,0,-3);
				if(arr_buf[i]==0) Function::put_square(0);
				if(GarbageModel==1){
					if(arr_buf[i]==1) Interactive::setcol(-4),Function::put_square(2);
					if(arr_buf[i]==2) Interactive::setcol(-4),Function::put_square(1);
				}
				if(GarbageModel==5){
					if(arr_buf[i]==1) Interactive::setcol(-5),Function::put_square(2);
					if(arr_buf[i]==2) Interactive::setcol(-4),Function::put_square(2);
					if(arr_buf[i]==3) Interactive::setcol(-4),Function::put_square(1);
				}
			}
		}
	}
	void add_buffer(int atk,map &mp){
		if(atk) buf.push_back(Buffer{(int)(atk*GarbageMultiple),timer.get(),0});
		update_buffer();
	}
	void offset_buffer(int atk,int cnt_clear,map &mp){
		if(cnt_clear){
			while(buf.size()){
				if(buf.front().cnt_backfire<=atk) atk-=buf.front().cnt_backfire,buf.pop_front();
				else{buf.front().cnt_backfire-=atk;break;}
			}
			if(!buf.size()&&atk&&GarbageModel==1) add_buffer(atk,mp);
		}
		else{
			int tmp=8;
			double tim_lim;
			if(GarbageModel==1) tim_lim=0.3;
			if(GarbageModel==5) tim_lim=5;
			while(buf.size()&&timer.get()-buf.front().tim_backfire>=tim_lim&&tmp){
				if(buf.front().cnt_backfire<=tmp){
					add_garbage(buf.front().cnt_backfire,mp,buf.front().flag_hole*2),tmp-=buf.front().cnt_backfire,buf.pop_front();
				}
				else{
					Buffer g=buf.front();
					add_garbage(tmp,mp,g.flag_hole*2),buf.pop_front(),buf.push_front(Buffer{g.cnt_backfire-tmp,g.tim_backfire,1});
					break;
				}
			}
		}
		update_buffer();
	}
}

#endif