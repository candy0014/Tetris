#ifndef PLACE_H
#define PLACE_H

#include "timer.h"
#include "block.h"
#include "map.h"
#include "init.h"

namespace Place{

int play(map &mp,Block::block B,int flag_h=0){
	int x=-2,y=(mapWidth-1)/2,type=0;
	if(!B.check(x,y,type,mp)){
		Sleep(1000);
		return 2;
	}
	B.put(x,y,type,mp);
	double tim=timer.get();
	int vis[128];
	double t[128];
	for(auto ii:KEY){
		int i=(int)ii;
		vis[i]=0,t[i]=timer.get();
		if(custom[i]=="CW"||custom[i]=="CCW"||custom[i]=="F"||custom[i]=="HD"||custom[i]=="RE"){
			vis[i]=-Interactive::keydown(i);
		}
	}
	int cnt_op=0;
	std::vector<int> kb_rotate,kb_move;
	for(auto ii:KEY){
		int i=(int)ii;
		if(custom[i]=="F"||custom[i]=="CW"||custom[i]=="CCW") kb_rotate.emplace_back(i);
		if(custom[i]=="L"||custom[i]=="R") kb_move.emplace_back(i);
	}
	while(1){
		if(timer.get()-tim>EPLD){
			if(!B.check(x+1,y,type,mp)) break;
		}
		if(timer.get()-tim>SPE){
			if(B.check(x+1,y,type,mp)) B.put(x,y,type,mp,0),x++,B.put(x,y,type,mp),tim=timer.get();
		}
		int last_x=x,last_y=y,last_type=type;
		bool flag_hd=0;
		for(auto ii:KEY){
			int i=(int)ii;
			int tmp=Interactive::keydown(i);
			if(vis[i]==-1){
				if(!tmp) vis[i]=0;
				continue;
			}
			if(!vis[i]&&!tmp) continue;
			if(tmp&&custom[i]=="HD"){
				while(B.check(x+1,y,type,mp)) x++;
				flag_hd=1;
				break;
			}
			if(tmp&&custom[i]=="RE") return 2;
			if(vis[i]&&!tmp){
				vis[i]=0;
				continue;
			}
			else if(!vis[i]&&tmp){
				t[i]=timer.get(),vis[i]=1;
				if(custom[i]=="H"){
					if(flag_h) continue;
					B.put(last_x,last_y,last_type,mp,0);
					if(Init::now_hold!=-1) Block::get_block(Init::now_hold).put_hold(0);
					B.put_hold(-1);
					return 1;
				}
				if(custom[i]=="F"){
					double ma_tim=0;int mak=0;
					for(auto d:kb_rotate) if(vis[d]){
						if(ma_tim<t[d]) ma_tim=t[d],mak=d;
					}
					if(mak==i){
						int flag=!B.check(x+1,y,type,mp);
						if(B.checks(x,y,type,(type+2)%4,mp)){
							type=(type+2)%4,cnt_op+=flag;
							if(flag&&cnt_op<=EPLD_lim) tim=timer.get();
						}
					}
				}
				if(custom[i]=="CCW"){
					double ma_tim=0;int mak=0;
					for(auto d:kb_rotate) if(vis[d]){
						if(ma_tim<t[d]) ma_tim=t[d],mak=d;
					}
					if(mak==i){
						int flag=!B.check(x+1,y,type,mp);
						if(B.checks(x,y,type,(type+3)%4,mp)){
							type=(type+3)%4,cnt_op+=flag;
							if(flag&&cnt_op<=EPLD_lim) tim=timer.get();
						}
					}
				}
				if(custom[i]=="CW"){
					double ma_tim=0;int mak=0;
					for(auto d:kb_rotate) if(vis[d]){
						if(ma_tim<t[d]) ma_tim=t[d],mak=d;
					}
					if(mak==i){
						int flag=!B.check(x+1,y,type,mp);
						if(B.checks(x,y,type,(type+1)%4,mp)){
							type=(type+1)%4,cnt_op+=flag;
							if(flag&&cnt_op<=EPLD_lim) tim=timer.get();
						}
					}
				}
				if(custom[i]=="SD"&&B.check(x+1,y,type,mp)) x++,tim=timer.get();
				if(custom[i]=="L"){
					double ma_tim=0;int mak=0;
					for(auto d:kb_move) if(vis[d]){
						if(ma_tim<t[d]) ma_tim=t[d],mak=d;
					}
					if(mak==i){
						int flag=!B.check(x+1,y,type,mp);
						if(B.check(x,y-1,type,mp)){
							y--,cnt_op+=flag;
							if(flag&&cnt_op<=EPLD_lim) tim=timer.get();
						}
					}
				}
				if(custom[i]=="R"){
					double ma_tim=0;int mak=0;
					for(auto d:kb_move) if(vis[d]){
						if(ma_tim<t[d]) ma_tim=t[d],mak=d;
					}
					if(mak==i){
						int flag=!B.check(x+1,y,type,mp);
						if(B.check(x,y+1,type,mp)){
							y++,cnt_op+=flag;
							if(flag&&cnt_op<=EPLD_lim) tim=timer.get();
						}
					}
				}
			}
			else if(vis[i]&&tmp){
				if(custom[i]=="CW"||custom[i]=="CCW"||custom[i]=="F") continue;
				if(custom[i]=="SD"){
					if(SDF==0){
						while(B.check(x+1,y,type,mp)) x++;
						tim=timer.get();
					}
					else if(timer.get()-t[i]>SPE/SDF){
						t[i]=timer.get();
						if(B.check(x+1,y,type,mp)) x++,tim=timer.get();
					}
				}
				if(custom[i]=="L"){
					double ma_tim=0;int mak=0;
					for(auto d:kb_move) if(vis[d]){
						if(ma_tim<t[d]) ma_tim=t[d],mak=d;
					}
					if(mak==i){
						if(vis[i]==1){
							if(timer.get()-t[i]>DAS){
								t[i]=timer.get(),vis[i]=2;
								int flag=!B.check(x+1,y,type,mp);
								if(B.check(x,y-1,type,mp)){
									y--,cnt_op+=flag;
									if(flag&&cnt_op<=EPLD_lim) tim=timer.get();
								}
							}
						}
						else if(timer.get()-t[i]>ARR){
							t[i]=timer.get();
							int flag=!B.check(x+1,y,type,mp);
							if(B.check(x,y-1,type,mp)){
								y--,cnt_op+=flag;
								if(flag&&cnt_op<=EPLD_lim) tim=timer.get();
							}
						}
					}
				}
				if(custom[i]=="R"){
					double ma_tim=0;int mak=0;
					for(auto d:kb_move) if(vis[d]){
						if(ma_tim<t[d]) ma_tim=t[d],mak=d;
					}
					if(mak==i){
						if(vis[i]==1){
							if(timer.get()-t[i]>DAS){
								t[i]=timer.get(),vis[i]=2;
								int flag=!B.check(x+1,y,type,mp);
								if(B.check(x,y+1,type,mp)){
									y++,cnt_op+=flag;
									if(flag&&cnt_op<=EPLD_lim) tim=timer.get();
								}
							}
						}
						else if(timer.get()-t[i]>ARR){
							t[i]=timer.get();
							int flag=!B.check(x+1,y,type,mp);
							if(B.check(x,y+1,type,mp)){
								y++,cnt_op+=flag;
								if(flag&&cnt_op<=EPLD_lim) tim=timer.get();
							}
						}
					}
				}
			}
		}
		if(x!=last_x||y!=last_y||type!=last_type){
			B.put(last_x,last_y,last_type,mp,0),B.put(x,y,type,mp);
		}
		if(flag_hd) break;
	}
	for(int i=0;i<4;i++) mp[x+B.shape[type][i][0]][y+B.shape[type][i][1]]=B.ty;
	int tag[105],cnt_clear=0;
	for(int i=-mapHeightN;i<mapHeightP;i++){
		tag[i+mapHeightN]=1;
		for(int j=0;j<mapWidth;j++){
			if(mp[i][j]==-1){tag[i+mapHeightN]=0;break;}
		}
		cnt_clear+=tag[i+mapHeightN];
	}
	if(!FSBorYPA){
		Interactive::go(7,-7,1);
		Interactive::rgb_set(255,255,255);
		std::cout<<"　　　　　　";
		Interactive::go(7,-7,1);
		if(cnt_clear==1) std::cout<<"ＳＩＮＧＬＥ";
		if(cnt_clear==2) std::cout<<"ＤＯＵＢＬＥ";
		if(cnt_clear==3) std::cout<<"ＴＲＩＰＬＥ";
		if(cnt_clear==4) std::cout<<"ＱＵＡＤ　　";
		Interactive::go(9,-8,1);
		std::cout<<"  　　　　　";
		Interactive::go(9,-8,1);
		if(cnt_clear){
			Init::combo++;
			if(Init::combo>=2){
				Interactive::rgb_set(139,185,61);
				std::cout<<Init::combo-1;
				if(Init::combo-1<10) std::cout<<" ";
				Interactive::rgb_set(255,255,255);
				std::cout<<"ＣＯＭＢＯ";
			}
		}
		else Init::combo=0;
		Interactive::go(mapHeightP+2,0);
	}
	int now=mapHeightP,flag=0;
	for(int i=mapHeightP-1;i>=-mapHeightN;i--){
		now--;
		while(now+mapHeightN>=1&&tag[now+mapHeightN]) now--,flag=1;
		now=std::max(now,1-mapHeightN);
		if(flag==0) continue;
		for(int j=0;j<mapWidth;j++) if(mp[i][j]!=mp[now][j]){
			Interactive::go(i,j);
			Interactive::setcol(mp[now][j]);
			if(mp[now][j]==-1) std::cout<<"  ";
			else std::cout<<"█ ";
		}
		mp[i]=mp[now];
	}
	Interactive::go(mapHeightP+2,0);
	return 0;
}

}

#endif
