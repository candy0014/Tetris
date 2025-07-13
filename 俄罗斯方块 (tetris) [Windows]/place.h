#ifndef PLACE_H
#define PLACE_H

#include "timer.h"
#include "block.h"
#include "map.h"
#include "init.h"

namespace Place{

std::mt19937 rnd(time(0));
int play(map &mp,Block::block B,int flag_h=0){
	int x=-2,y=(mapWidth-1)/2,type=0;
	if(!B.check(x,y,type,mp)){
		if(Invisible){
			for(int i=mapHeightP-1;i>=-mapHeightN;i--){
				for(int j=0;j<mapWidth;j++) if(mp[i][j]!=-1){
					Interactive::go(i,j);
					Interactive::setcol(mp[i][j]);
					if(version<=10) std::cout<<"█ ";
					else std::cout<<"██";
				}
			}
			Sleep(1000);
		} 
		Sleep(300);
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
	int last_op=0;
	while(1){
		if(time_interval!=0&&timer.get()-Init::last_tim>time_interval){
			setvbuf(stdout,NULL,_IOFBF,4096);
			B.put(x,y,type,mp,0);
			if(!B.check(x+1,y,type,mp)) x--;
			for(int i=-mapHeightN;i<mapHeightP-1;i++){
				for(int j=0;j<mapWidth;j++) if(mp[i][j]!=mp[i+1][j]){
					Interactive::go(i,j);
					Interactive::setcol(mp[i+1][j]);
					if(mp[i+1][j]==-1) std::cout<<" ";
					else{
						if(version<=10) std::cout<<"█ ";
						else std::cout<<"██";
					}
					fflush(stdout);
				}
				mp[i]=mp[i+1];
			}
			if(rnd()%1000000<1000000*cheese_messiness) Init::last_hole=rnd()%mapWidth;
			for(int j=0;j<mapWidth;j++){
				mp[mapHeightP-1][j]=(j==Init::last_hole)?-1:8;
				Interactive::go(mapHeightP-1,j),Interactive::setcol(mp[mapHeightP-1][j]);
				if(mp[mapHeightP-1][j]==-1) std::cout<<" ";
				else{
					if(version<=10) std::cout<<"█ ";
					else std::cout<<"██";
				}
				fflush(stdout);
			}
			B.put(x,y,type,mp);
			setvbuf(stdout,NULL,_IONBF,0);
			Init::last_tim=timer.get();
		}
		if(timer.get()-tim>EPLD){
			if(!B.check(x+1,y,type,mp)) break;
		}
		if(timer.get()-tim>SPE){
			if(B.check(x+1,y,type,mp)) B.put(x,y,type,mp,0),x++,B.put(x,y,type,mp),tim=timer.get(),last_op=0;
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
				while(B.check(x+1,y,type,mp)) x++,last_op=0,Init::score+=2;
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
						int flag=!B.check(x+1,y,type,mp),rotate_op;
						if(B.checks(x,y,type,(type+2)%4,mp,rotate_op)){
							type=(type+2)%4,cnt_op+=flag;
							if(flag&&cnt_op<=EPLD_lim) tim=timer.get(),last_op=rotate_op+1;
						}
					}
				}
				if(custom[i]=="CCW"){
					double ma_tim=0;int mak=0;
					for(auto d:kb_rotate) if(vis[d]){
						if(ma_tim<t[d]) ma_tim=t[d],mak=d;
					}
					if(mak==i){
						int flag=!B.check(x+1,y,type,mp),rotate_op;
						if(B.checks(x,y,type,(type+3)%4,mp,rotate_op)){
							type=(type+3)%4,cnt_op+=flag;
							if(flag&&cnt_op<=EPLD_lim) tim=timer.get(),last_op=rotate_op+1;
						}
					}
				}
				if(custom[i]=="CW"){
					double ma_tim=0;int mak=0;
					for(auto d:kb_rotate) if(vis[d]){
						if(ma_tim<t[d]) ma_tim=t[d],mak=d;
					}
					if(mak==i){
						int flag=!B.check(x+1,y,type,mp),rotate_op;
						if(B.checks(x,y,type,(type+1)%4,mp,rotate_op)){
							type=(type+1)%4,cnt_op+=flag;
							if(flag&&cnt_op<=EPLD_lim) tim=timer.get(),last_op=rotate_op+1;
						}
					}
				}
				if(custom[i]=="SD"&&B.check(x+1,y,type,mp)) x++,tim=timer.get(),last_op=0,Init::score++;
				if(custom[i]=="L"){
					double ma_tim=0;int mak=0;
					for(auto d:kb_move) if(vis[d]){
						if(ma_tim<t[d]) ma_tim=t[d],mak=d;
					}
					if(mak==i){
						int flag=!B.check(x+1,y,type,mp);
						if(B.check(x,y-1,type,mp)){
							y--,cnt_op+=flag,last_op=0;
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
							y++,cnt_op+=flag,last_op=0;
							if(flag&&cnt_op<=EPLD_lim) tim=timer.get();
						}
					}
				}
			}
			else if(vis[i]&&tmp){
				if(custom[i]=="CW"||custom[i]=="CCW"||custom[i]=="F") continue;
				if(custom[i]=="SD"){
					if(SDF==0){
						while(B.check(x+1,y,type,mp)) x++,Init::score++;
						tim=timer.get();
					}
					else if(timer.get()-t[i]>SPE/SDF){
						t[i]=timer.get();
						if(B.check(x+1,y,type,mp)) x++,tim=timer.get(),Init::score++;
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
									y--,cnt_op+=flag,last_op=0;
									if(flag&&cnt_op<=EPLD_lim) tim=timer.get();
								}
							}
						}
						else if(timer.get()-t[i]>ARR){
							t[i]=timer.get();
							int flag=!B.check(x+1,y,type,mp);
							if(B.check(x,y-1,type,mp)){
								y--,cnt_op+=flag,last_op=0;
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
									y++,cnt_op+=flag,last_op=0;
									if(flag&&cnt_op<=EPLD_lim) tim=timer.get();
								}
							}
						}
						else if(timer.get()-t[i]>ARR){
							t[i]=timer.get();
							int flag=!B.check(x+1,y,type,mp);
							if(B.check(x,y+1,type,mp)){
								y++,cnt_op+=flag,last_op=0;
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
	if(Invisible){
		B.put(x,y,type,mp,0);
	}
	int nomove_flag=1,dir[4][2]={1,0,0,1,-1,0,0,-1};
	for(int i=0;i<4;i++) if(B.check(x+dir[i][0],y+dir[i][1],type,mp)) nomove_flag=0;
	for(int i=0;i<4;i++) mp[x+B.shape[type][i][0]][y+B.shape[type][i][1]]=B.ty;
	int tag[105],cnt_clear=0,pc_flag=1;
	for(int i=-mapHeightN;i<mapHeightP;i++){
		tag[i+mapHeightN]=1;
		for(int j=0;j<mapWidth;j++){
			if(mp[i][j]==-1){tag[i+mapHeightN]=0;break;}
		}
		for(int j=1;j<mapWidth&&pc_flag;j++) if(std::min(0,mp[i][j])!=std::min(0,mp[i][0])) pc_flag=0;
		cnt_clear+=tag[i+mapHeightN];
	}
	int b2b_flag=0,spin_flag=0,mini_flag=0;
	setvbuf(stdout,NULL,_IOFBF,4096);
	Interactive::go(7,-7,1);
	std::cout<<"　          ",fflush(stdout);
	if(B.ty!=5){
		if(nomove_flag&&last_op){
			Interactive::go(7,-7,1);
			if(!FSBorYPA) Interactive::setcol(B.ty);
			else Interactive::setcol(-1);
			if(B.ty==0) std::cout<<"Ｉ";
			if(B.ty==1) std::cout<<"Ｊ";
			if(B.ty==2) std::cout<<"Ｌ";
			if(B.ty==3) std::cout<<"Ｏ";
			if(B.ty==4) std::cout<<"Ｓ";
			if(B.ty==6) std::cout<<"Ｚ";
			std::cout<<"-Spin",spin_flag=1;
			fflush(stdout);
		}
	}
	else{
		int cnt1=0,cnt2=0,dx=0;
		int _dir[4][2]={-1,-1,-1,1,1,1,1,-1};
		if(RotationSystem=="ARS") dx=1;
		for(int i=0;i<4;i++) if(x+_dir[i][0]+dx>=mapHeightP||y+_dir[i][1]<0||y+_dir[i][1]>=mapWidth||mp[x+_dir[i][0]+dx][y+_dir[i][1]]!=-1){
			cnt1++;
			if(RotationSystem=="SRS"){
				if(i==type||i==(type+1)%4) cnt2++;
			}
			if(RotationSystem=="ARS"){
				if(i==(type+2)%4||i==(type+3)%4) cnt2++;
			}
		}
		if(cnt1>=3&&(cnt2==2||last_op==6)){
			Interactive::go(7,-7,1);
			if(!FSBorYPA) Interactive::setcol(B.ty);
			else Interactive::setcol(-1);
			spin_flag=1;
			std::cout<<"Ｔ-Spin";
		}
		else if(last_op&&(nomove_flag||cnt1==3)){
			Interactive::go(7,-7,1);
			if(!FSBorYPA) Interactive::setcol(B.ty);
			else Interactive::setcol(-1);
			spin_flag=1,mini_flag=1;
			std::cout<<"Ｔ-Spin Mini";
		}
		fflush(stdout);
	}	
	Interactive::go(8,-7,1);
	if(!FSBorYPA) Interactive::setcol(-3);
	else Interactive::setcol(-1);
	std::cout<<"　　　　　　",fflush(stdout);
	Interactive::go(8,-7,1);
	if(cnt_clear==0) b2b_flag=2;
	if(cnt_clear==1) std::cout<<"ＳＩＮＧＬＥ";
	if(cnt_clear==2) std::cout<<"ＤＯＵＢＬＥ";
	if(cnt_clear==3) std::cout<<"ＴＲＩＰＬＥ";
	if(cnt_clear==4) std::cout<<"ＱＵＡＤ　　",b2b_flag=1;
	fflush(stdout);
	b2b_flag|=spin_flag|pc_flag;
	Interactive::go(9,-7,2);
	std::cout<<"           ",fflush(stdout);
	if(!b2b_flag) Init::b2b=0;
	if(b2b_flag==1) Init::b2b++;
	if(Init::b2b>=2){
		Interactive::go(9,-7,2);
		if(!FSBorYPA) Interactive::setcol(-3);
		else Interactive::setcol(-1);
		std::cout<<"B2B×"<<Init::b2b-1;
	}
	fflush(stdout);
	Interactive::go(11,-9,0);
	std::cout<<"       　　　　　",fflush(stdout);
	if(cnt_clear){
		Init::combo++;
		if(Init::combo>=2){
			int dig=0,tmp=Init::combo-1;
			while(tmp) dig++,tmp/=10;
			Interactive::go(11,-7-dig/2+!(dig&1),dig&1);
			if(!FSBorYPA) Interactive::setcol(-3);
			else Interactive::setcol(-1);
			std::cout<<Init::combo-1<<" ";
			std::cout<<"ＣＯＭＢＯ";
			fflush(stdout);
		}
	}
	else Init::combo=0;
	Interactive::go(13,-5,0);
	std::cout<<"　　　",fflush(stdout);
	Interactive::go(14,-6,0);
	std::cout<<"　　　　　",fflush(stdout);
	if(pc_flag){
		if(!FSBorYPA) Interactive::rgb_set(238,219,47);
		else Interactive::setcol(-1);
		Interactive::go(13,-5,0);
		std::cout<<"ＡＬＬ",fflush(stdout);
		Interactive::go(14,-6,0);
		std::cout<<"ＣＬＥＡＲ",fflush(stdout);
	}
	Interactive::go(mapHeightP+2,0);
	int now=mapHeightP,flag=0;
	for(int i=mapHeightP-1;i>=-mapHeightN;i--){
		now--;
		while(now+mapHeightN>=1&&tag[now+mapHeightN]) now--,flag=1;
		now=std::max(now,1-mapHeightN);
		if(flag==0) continue;
		for(int j=0;j<mapWidth;j++) if(mp[i][j]!=mp[now][j]){
			Interactive::go(i,j);
			Interactive::setcol(mp[now][j]);
			if(!Invisible){
				if(mp[now][j]==-1) std::cout<<"  ";
				else{
					if(version<=10) std::cout<<"█ ";
					else std::cout<<"██";
				}
				fflush(stdout);
			}
		}
		mp[i]=mp[now];
	}
	int dscore=0;
	if(cnt_clear==1) dscore+=100;
	if(cnt_clear==2) dscore+=300;
	if(cnt_clear==3) dscore+=500;
	if(cnt_clear==4) dscore+=800;
	if(B.ty==5&&spin_flag){
		if(!mini_flag) dscore+=(cnt_clear+1)*400;
		else dscore+=(100<<cnt_clear);
	}
	if(pc_flag) dscore+=3500;
	if(b2b_flag==1&&Init::b2b>=2) dscore=dscore*1.5;
	if(Init::combo) dscore+=50*(Init::combo-1);
	Init::score+=dscore;
	Init::print_score();
	setvbuf(stdout,NULL,_IONBF,0);
	Interactive::go(mapHeightP+2,0);
	return 0;
}

}

#endif
