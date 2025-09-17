#ifndef PLACE_H
#define PLACE_H

#include "init.h"

namespace Place{

int play(map &mp,Block::block B,int flag_h=0){
	int BlitzLevel=0;
	if(Model==2&&!flagBlitz){
		int tmp=Init::cnt_line;
		for(int i=1;;i++){
			if(tmp>i*2+(i+9)/10) tmp-=i*2+(i+9)/10;
			else{BlitzLevel=i;break;}
		}
		double SpeedTable[20]={0,1,0.643,0.404,0.249,0.15,0.088,0.0505,0.0283,0.0155,0.00827,0.00431,0.00219,0.00108,0.00052,0};
		Speed=SpeedTable[std::min(BlitzLevel,15)];
	}
	int x=-2,y=(mapWidth-1)/2,type=0;
	if(!B.check(x,y,type,mp)){
		if(Invisible){
			for(int i=mapHeight-1;i>=-mapHeightN;i--){
				for(int j=0;j<mapWidth;j++) if(mp[i][j]!=-1){
					Interactive::go(i,j);
					Interactive::setcol(mp[i][j]);
					Function::put_square();
				}
			}
			timer.sleep(0.5);
		} 
		timer.sleep(1);
		Function::send("end");
		return 2;
	}
	B.put(x,y,type,mp);
	double tim=timer.get();
	int vis[128];
	double t[128];
	for(int i=0;i<128;i++) vis[i]=0,t[i]=0;
	for(auto ii:KEY){
		int i=(int)ii;
		vis[i]=0,t[i]=timer.get();
		if(custom[i]=="CW"||custom[i]=="CCW"||custom[i]=="F"||custom[i]=="HD"||custom[i]=="RE"||custom[i]=="SET"){
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
		if(GarbageModel==1||GarbageModel==5||GarbageModel==6) Garbage::update_buffer();
		if(Model==2&&timer.get()-Init::begin_tim>BlitzTime){
			B.put(x,y,type,mp,0);
			if(!FSBorYPA) Interactive::setcol(-3);
			else Interactive::setcol(-1);
			Interactive::go(16,-8);std::cout<<"      ";
			Interactive::go(16,-5,-1),std::cout<<"0.00";
			return 0;
		}
		if(timer.get()-Init::last_tim2>0.1){
			if(GarbageModel==5){
				if(rd()%30==0){
					int atk=rd()%4+1;
					Garbage::add_buffer(atk,mp);
				}
			}
			if(GarbageModel==6){
				std::string s;
				while((s=Function::receive())!="fail"){
					if(s=="end"){
						return 2;
					}
					else{
						int atk=0;
						for(int i=0;s[i];i++) atk=atk*10+s[i]-'0';
						Garbage::add_buffer(atk,mp);
					}
				}
			}
			setvbuf(stdout,NULL,_IOFBF,4096);
			if(!FSBorYPA) Interactive::setcol(-3);
			else Interactive::setcol(-1);
			double _tim=timer.get()-Init::begin_tim,_pps=Init::cnt_block/_tim,_apm=Init::cnt_atk*60/_tim;
			Interactive::go(17,-7);std::cout<<"    ",fflush(stdout);
			if(Model==2){
				Interactive::go(16,-8);std::cout<<"      ",fflush(stdout);
				_tim=BlitzTime-_tim;
			}
			Interactive::go(16,-5,-Function::get_dig((int)_tim));
			printf("%.2f S",_tim);fflush(stdout);
			Interactive::go(17,-5,-Function::get_dig((int)_pps));
			printf("%.2f PPS",_pps);fflush(stdout);
			if(Model!=1){
				Interactive::go(18,-7);std::cout<<"    ",fflush(stdout);
				Interactive::go(18,-5,-Function::get_dig((int)_apm));
				printf("%.2f APM",_apm);fflush(stdout);
			}
			Interactive::gotoxy(1,1);
			setvbuf(stdout,NULL,_IONBF,0);
			Init::last_tim2=timer.get();
		}
		if(GarbageModel==4&&timer.get()-Init::last_tim>TimeInterval){
			setvbuf(stdout,NULL,_IOFBF,4096);
			B.put(x,y,type,mp,0);
			Garbage::add_garbage(1,mp);
			if(!B.check(x,y,type,mp)) x--;
			B.put(x,y,type,mp);
			setvbuf(stdout,NULL,_IONBF,0);
			Init::last_tim=timer.get();
		}
		if(timer.get()-tim>EPLD&&Speed<=1e9){
			if(!B.check(x+1,y,type,mp)) break;
		}
		if(timer.get()-tim>Speed){
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
			if(tmp&&custom[i]=="RE"){
				if(GarbageModel==6) Function::send("end");
				return 2;
			}
			if(tmp&&custom[i]=="SET"){
				if(GarbageModel==6) Function::send("end");
				return 3;
			}
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
							if(flag&&cnt_op<=EPLDLim) tim=timer.get(),last_op=rotate_op+1;
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
							if(flag&&cnt_op<=EPLDLim) tim=timer.get(),last_op=rotate_op+1;
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
							if(flag&&cnt_op<=EPLDLim) tim=timer.get(),last_op=rotate_op+1;
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
							if(flag&&cnt_op<=EPLDLim) tim=timer.get();
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
							if(flag&&cnt_op<=EPLDLim) tim=timer.get();
						}
					}
				}
			}
			else if(vis[i]&&tmp){
				if(custom[i]=="CW"||custom[i]=="CCW"||custom[i]=="F") continue;
				if(custom[i]=="SD"){
					if(SDF==0){
						while(B.check(x+1,y,type,mp)) x++,Init::score++,tim=timer.get();
					}
					else if(timer.get()-t[i]>Speed/SDF){
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
									if(flag&&cnt_op<=EPLDLim) tim=timer.get();
								}
							}
						}
						else if(timer.get()-t[i]>ARR){
							t[i]=timer.get();
							int flag=!B.check(x+1,y,type,mp);
							if(B.check(x,y-1,type,mp)){
								y--,cnt_op+=flag,last_op=0;
								if(flag&&cnt_op<=EPLDLim) tim=timer.get();
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
									if(flag&&cnt_op<=EPLDLim) tim=timer.get();
								}
							}
						}
						else if(timer.get()-t[i]>ARR){
							t[i]=timer.get();
							int flag=!B.check(x+1,y,type,mp);
							if(B.check(x,y+1,type,mp)){
								y++,cnt_op+=flag,last_op=0;
								if(flag&&cnt_op<=EPLDLim) tim=timer.get();
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
	int cnt_garbage=0;
	for(int i=-mapHeightN;i<mapHeight;i++){
		tag[i+mapHeightN]=1;
		for(int j=0;j<mapWidth;j++){
			if(mp[i][j]==-1){tag[i+mapHeightN]=0;break;}
		}
		for(int j=1;j<mapWidth&&pc_flag;j++) if(std::min(0,mp[i][j])!=std::min(0,mp[i][0])) pc_flag=0;
		cnt_clear+=tag[i+mapHeightN];
		if(tag[i+mapHeightN]&&i>=mapHeight-LayerHeight) cnt_garbage++;
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
		for(int i=0;i<4;i++) if(x+_dir[i][0]+dx>=mapHeight||y+_dir[i][1]<0||y+_dir[i][1]>=mapWidth||mp[x+_dir[i][0]+dx][y+_dir[i][1]]!=-1){
			cnt1++;
			if(RotationSystem=="SRS"){
				if(i==type||i==(type+1)%4) cnt2++;
			}
			if(RotationSystem=="ARS"){
				if(i==(type+2)%4||i==(type+3)%4) cnt2++;
			}
		}
		if(cnt1>=3&&(cnt2==2||last_op==5)){
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
	int b2b_charging=0;
	if(!b2b_flag){
		if(Init::b2b>=5) b2b_charging=Init::b2b-1;
		Init::b2b=0;
	}
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
	Interactive::go(mapHeight+2,0);
	int now=mapHeight,flag=0;
	if(AntiGravity){
		for(int i=mapHeight-1;i>=-mapHeightN;i--) if(tag[i+mapHeightN]){
			for(int j=0;j<mapWidth;j++){
				mp[i][j]=-1;
				Interactive::go(i,j);
				Function::put_square(0);
			}
		}
	}
	else{
		for(int i=mapHeight-1;i>=-mapHeightN;i--){
			now--;
			while(now+mapHeightN>=1&&tag[now+mapHeightN]) now--,flag=1;
			now=std::max(now,1-mapHeightN);
			if(flag==0) continue;
			for(int j=0;j<mapWidth;j++) if(mp[i][j]!=mp[now][j]){
				Interactive::go(i,j);
				Interactive::setcol(mp[now][j]);
				Function::put_square(mp[now][j]!=-1&&(!Invisible||mp[now][j]==7));
			}
			mp[i]=mp[now];
		}
	}
	if(GarbageModel==3){
		Garbage::add_garbage(cnt_garbage,mp);
	}
	int atk=0;
	if(cnt_clear){
		int basic=0,level_b2b=0;
		if(cnt_clear==2) basic=1;
		if(cnt_clear==3) basic=2;
		if(cnt_clear==4) basic=4;
		if(B.ty==5&&spin_flag){
			if(!mini_flag) basic=cnt_clear*2;
			else basic=(cnt_clear>=2);
		}
		int leveltable[10]={1,3,8,24,67,185,504,1370,1000000000};
		for(int i=0;i<=8;i++) if(Init::b2b<=leveltable[i]){level_b2b=i;break;}
		atk=(basic+level_b2b)*(1+(Init::combo-1)*0.25);
		if(!basic&&!level_b2b){
			if(Init::combo<=2) atk=0;
			else if(Init::combo<=6) atk=1;
			else if(Init::combo<=16) atk=2;
			else atk=3;
		}
		atk+=pc_flag*5+b2b_charging;
	}
	if(GarbageModel==2) Garbage::add_garbage((int)(atk*GarbageMultiple),mp);
	if(GarbageModel==1||GarbageModel==5||GarbageModel==6) Garbage::offset_buffer(atk,cnt_clear,mp);
	Init::cnt_atk+=atk,Init::cnt_block++;
	int dscore=0;
	if(cnt_clear==1) dscore+=100;
	if(cnt_clear==2) dscore+=300;
	if(cnt_clear==3) dscore+=500;
	if(cnt_clear==4) dscore+=800;
	if(B.ty==5&&spin_flag){
		if(!mini_flag) dscore=(cnt_clear+1)*400;
		else dscore=(100<<cnt_clear);
	}
	if(pc_flag) dscore+=3500;
	if(b2b_flag==1&&Init::b2b>=2) dscore=dscore*1.5;
	if(Init::combo) dscore+=50*(Init::combo-1);
	if(Model==2&&!flagBlitz) Init::score+=dscore*BlitzLevel;
	else Init::score+=dscore;
	Function::print_score(Init::score);
	Init::cnt_line+=cnt_clear;
	if(Model==1){
		Interactive::go(18,-4,-Function::get_dig(Init::cnt_line)-Function::get_dig(RacingDistance));
		std::cout<<Init::cnt_line<<"/"<<RacingDistance<<" LINES";
	}
	setvbuf(stdout,NULL,_IONBF,0);
	Interactive::go(mapHeight+2,0);
	return 0;
}

}

#endif
