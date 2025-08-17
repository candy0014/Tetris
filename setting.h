#ifndef SETTING_H
#define SETTING_H

#include "interactive.h"
#include "timer.h"
#include "init.h"
#include "function.h"

namespace Setting{
	struct node{
		std::string name;
		int *a;
		double *b;
		std::string *c;
	}var[105];
	int cnt_var;
	int lin[105],cnt_lin;
	void insert0(std::string name,int *a){
		var[cnt_var].name=name,var[cnt_var].a=a,var[cnt_var].b=NULL,var[cnt_var].c=NULL;
		lin[cnt_var]=++cnt_lin;
		cnt_var++;
	}
	void insert1(std::string name,double *a){
		var[cnt_var].name=name,var[cnt_var].a=NULL,var[cnt_var].b=a,var[cnt_var].c=NULL;
		lin[cnt_var]=++cnt_lin;
		cnt_var++;
	}
	void insert2(std::string name,std::string *a){
		var[cnt_var].name=name,var[cnt_var].a=NULL,var[cnt_var].b=NULL,var[cnt_var].c=a;
		lin[cnt_var]=++cnt_lin;
		cnt_var++;
	}
	void print(int i,int op=0){
		if(op){
			if(!FSBorYPA) Interactive::setcol(3);
			else Interactive::setcol(-1);
		}
		Interactive::gotoxy(lin[i],1);
		std::cout<<var[i].name<<"=";
		if(op==2) std::cout<<"                    ";
		else{
			if(var[i].a!=NULL) std::cout<<(*var[i].a);
			if(var[i].b!=NULL) std::cout<<(*var[i].b);
			if(var[i].c!=NULL) std::cout<<(*var[i].c);
		}
		if(op) Interactive::setcol(-3);
	}
	void setting(){
		Function::clear();
		cnt_var=0,cnt_lin=0;
		insert0("Model",&Model);
		insert0("Racing Distance",&RacingDistance);
		insert1("Blitz Time",&BlitzTime);
		cnt_lin++;
		insert0("Garbage Model",&GarbageModel);
		insert0("Cheese Model",&CheeseModel);
		insert0("Num of Hole",&HoleNum);
		insert0("Layer Height",&LayerHeight);
		insert1("Time Interval",&TimeInterval);
		insert1("Cheese Messiness",&CheeseMessiness);
		insert1("Garbage Multiple",&GarbageMultiple);
		cnt_lin++;
		insert1("Speed",&Speed);
		insert1("SDF",&SDF);
		insert1("DAS",&DAS);
		insert1("ARR",&ARR);
		cnt_lin++;
		insert0("Width",&mapWidth);
		insert0("Height",&mapHeight);
		insert0("Num of Next",&NextNum);
		insert0("Open Hold",&OpenHold);
		insert0("Invisible",&Invisible);
		insert0("Ghost",&Ghost);
		insert1("EPLD",&EPLD);
		insert0("Limit of EPLD",&EPLDLim);
		insert2("Rotation System",&RotationSystem);
		cnt_lin++;
		insert0("Windows Version",&WindowsVersion);
		insert0("FSBorYPA",&FSBorYPA);
		Interactive::setcol(-3);
		for(int i=0;i<cnt_var;i++) print(i);
		int now=0;print(0,1);
		int vis[128];
		double t[128];
		for(int i=0;i<128;i++) vis[i]=0,t[i]=0;
		for(auto i:{KEY_UP,KEY_DOWN}) t[i]=timer.get(),vis[i]=-Interactive::keydown(i);
		std::vector<char>letter;
		for(int i=0;i<26;i++) letter.emplace_back(i+'A');
		for(int i=0;i<10;i++) letter.emplace_back(i+'0');
		while(1){
			for(auto i:KEY) if(custom[i]=="RE"&&Interactive::keydown(i)) return;
			for(auto i:{KEY_UP,KEY_DOWN,KEY_ENTER}){
				int tmp=Interactive::keydown(i);
				if(!vis[i]&&!tmp) continue;
				if(vis[i]&&!tmp){
					if(vis[i]!=-1&&i==KEY_ENTER){
						print(now,2);
						Interactive::gotoxy(lin[now],var[now].name.length()+2);
						Function::cur_show();
						#ifdef _WIN32
						FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
						#endif
						#ifdef __linux__
						tcflush(STDIN_FILENO,TCIFLUSH);
						#endif
						std::string tmp;
						if(!FSBorYPA) Interactive::setcol(3);
						else Interactive::setcol(-1);
						getline(std::cin,tmp);
						if(tmp.length()){
							if(var[now].a!=NULL){
								int v=0,flag=1;
								for(int j=0;j<(int)tmp.length();j++){
									if('0'<=tmp[j]&&tmp[j]<='9') v=v*10+tmp[j]-'0';
									if(tmp[j]=='-'&&v==0&&flag==1) flag=-1;
								}
								(*var[now].a)=v*flag;
							}
							if(var[now].b!=NULL){
								double v=0,w=1;int flag=0;
								for(int j=0;j<(int)tmp.length();j++){
									if('0'<=tmp[j]&&tmp[j]<='9'){
										if(!flag) v=v*10+tmp[j]-'0';
										else w/=10,v+=w*(tmp[j]-'0');
									}
									if(tmp[j]=='.') flag=1;
								}
								(*var[now].b)=v;
							}
							if(var[now].c!=NULL) (*var[now].c)=tmp;
						}
						print(now,1);
						Function::cur_hide();
						vis[i]=-1;
						if(var[now].name=="FSBorYPA"){
							for(int j=0;j<cnt_var;j++) print(j);
							print(now,1);
						}
						timer.sleep(0.1);
					}
					else vis[i]=0;
					continue;
				}
				else if(!vis[i]&&tmp){
					t[i]=timer.get(),vis[i]=1;
					if(i==KEY_UP){
						double ma_tim=0;int mak=0;
						for(auto d:{KEY_UP,KEY_DOWN}) if(vis[d]){
							if(ma_tim<t[d]) ma_tim=t[d],mak=d;
						}
						if(mak==i) print(now),now=(now-1+cnt_var)%cnt_var,print(now,1);
					}
					if(i==KEY_DOWN){
						double ma_tim=0;int mak=0;
						for(auto d:{KEY_UP,KEY_DOWN}) if(vis[d]){
							if(ma_tim<t[d]) ma_tim=t[d],mak=d;
						}
						if(mak==i) print(now),now=(now+1)%cnt_var,print(now,1);
					}
				}
				else if(vis[i]&&tmp){
					if(i==KEY_UP){
						double ma_tim=0;int mak=0;
						for(auto d:{KEY_UP,KEY_DOWN}) if(vis[d]){
							if(ma_tim<t[d]) ma_tim=t[d],mak=d;
						}
						if(mak==i){
							if(vis[i]==1){
								if(timer.get()-t[i]>0.2){
									t[i]=timer.get(),vis[i]=2;
									print(now),now=(now-1+cnt_var)%cnt_var,print(now,1);
								}
							}
							else if(timer.get()-t[i]>0.05){
								t[i]=timer.get();
								print(now),now=(now-1+cnt_var)%cnt_var,print(now,1);
							}
						}
					}
					if(i==KEY_DOWN){
						double ma_tim=0;int mak=0;
						for(auto d:{KEY_UP,KEY_DOWN}) if(vis[d]){
							if(ma_tim<t[d]) ma_tim=t[d],mak=d;
						}
						if(mak==i){
							if(vis[i]==1){
								if(timer.get()-t[i]>0.2){
									t[i]=timer.get(),vis[i]=2;
									print(now),now=(now+1)%cnt_var,print(now,1);
								}
							}
							else if(timer.get()-t[i]>0.05){
								t[i]=timer.get();
								print(now),now=(now+1)%cnt_var,print(now,1);
							}
						}
					}
				}
			}
		}
	}
}

#endif
