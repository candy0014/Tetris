#ifndef SETTING_H
#define SETTING_H

#include "interactive.h"
#include "timer.h"
#include "init.h"

namespace Setting{
	struct node{
		std::string name;
		int *a;
		double *b;
		std::string *c;
	}var[105];
	int cnt_var;
	void insert0(std::string name,int *a){
		var[cnt_var].name=name,var[cnt_var].a=a,var[cnt_var].b=NULL,var[cnt_var].c=NULL;
		cnt_var++;
	}
	void insert1(std::string name,double *a){
		var[cnt_var].name=name,var[cnt_var].a=NULL,var[cnt_var].b=a,var[cnt_var].c=NULL;
		cnt_var++;
	}
	void insert2(std::string name,std::string *a){
		var[cnt_var].name=name,var[cnt_var].a=NULL,var[cnt_var].b=NULL,var[cnt_var].c=a;
		cnt_var++;
	}
	void print(int i,int op=0){
		if(op){
			if(!FSBorYPA) Interactive::setcol(3);
			else Interactive::setcol(-1);
		}
		Interactive::gotoxy(i+1,1);
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
		system("cls");
		cnt_var=0;
		insert0("Windows Version",&version);
		insert2("Rotation System",&RotationSystem);
		insert1("Speed",&SPE);
		insert1("SDF",&SDF);
		insert1("DAS",&DAS);
		insert1("ARR",&ARR);
		insert0("Width",&mapWidth);
		insert0("Height",&mapHeight);
		insert0("Num of Next",&Next_num);
		insert0("Open Hold",&Open_hold);
		insert0("Invisible",&Invisible);
		insert0("Ghost",&Ghost);
		insert0("Model",&Model);
		insert0("Racing Distance",&lim_line);
		insert1("Blitz Time",&lim_time);
		insert0("Garbage",&Garbage);
		insert0("Layer Height",&layer_height);
		insert1("Time Interval",&time_interval);
		insert1("Cheese Messiness",&cheese_messiness);
		insert1("Garbage Multiple",&garbage_multiple);
		insert1("EPLD",&EPLD);
		insert0("Limit of EPLD",&EPLD_lim);
		insert0("FSBorYPA",&FSBorYPA);
		Interactive::setcol(-3);
		for(int i=0;i<cnt_var;i++) print(i);
		int now=0;print(0,1);
		int vis[128];
		double t[128];
		for(auto i:{VK_UP,VK_DOWN}) t[i]=timer.get(),vis[i]=-Interactive::keydown(i);
		std::vector<char>letter;
		for(int i=0;i<26;i++) letter.emplace_back(i+'A');
		for(int i=0;i<10;i++) letter.emplace_back(i+'0');
		while(1){
			for(auto i:KEY) if(custom[i]=="RE"&&Interactive::keydown(i)) return;
			for(auto i:{VK_UP,VK_DOWN,VK_RETURN}){
				int tmp=Interactive::keydown(i);
				if(!vis[i]&&!tmp) continue;
				if(vis[i]&&!tmp){
					if(vis[i]!=-1&&i==VK_RETURN){
						print(now,2);
						Interactive::gotoxy(now+1,var[now].name.length()+2);
						Init::cur_show();
						FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
						std::string tmp;
						if(!FSBorYPA) Interactive::setcol(3);
						else Interactive::setcol(-1);
						getline(std::cin,tmp);
						if(tmp.length()){
							if(var[now].a!=NULL){
								int v=0;
								for(int j=0;j<(int)tmp.length();j++) if('0'<=tmp[j]&&tmp[j]<='9') v=v*10+tmp[j]-'0';
								(*var[now].a)=v;
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
						Init::cur_hide();
						vis[i]=-1;
						if(var[now].name=="FSBorYPA"){
							for(int j=0;j<cnt_var;j++) print(j);
							print(now,1);
						}
					}
					else vis[i]=0;
					continue;
				}
				else if(!vis[i]&&tmp){
					t[i]=timer.get(),vis[i]=1;
					if(i==VK_UP){
						double ma_tim=0;int mak=0;
						for(auto d:{VK_UP,VK_DOWN}) if(vis[d]){
							if(ma_tim<t[d]) ma_tim=t[d],mak=d;
						}
						if(mak==i&&now) print(now),now--,print(now,1);
					}
					if(i==VK_DOWN){
						double ma_tim=0;int mak=0;
						for(auto d:{VK_UP,VK_DOWN}) if(vis[d]){
							if(ma_tim<t[d]) ma_tim=t[d],mak=d;
						}
						if(mak==i&&now+1<cnt_var) print(now),now++,print(now,1);
					}
				}
				else if(vis[i]&&tmp){
					if(i==VK_UP){
						double ma_tim=0;int mak=0;
						for(auto d:{VK_UP,VK_DOWN}) if(vis[d]){
							if(ma_tim<t[d]) ma_tim=t[d],mak=d;
						}
						if(mak==i){
							if(vis[i]==1){
								if(timer.get()-t[i]>0.2){
									t[i]=timer.get(),vis[i]=2;
									if(now) print(now),now--,print(now,1);
								}
							}
							else if(timer.get()-t[i]>0.05){
								t[i]=timer.get();
								if(now) print(now),now--,print(now,1);
							}
						}
					}
					if(i==VK_DOWN){
						double ma_tim=0;int mak=0;
						for(auto d:{VK_UP,VK_DOWN}) if(vis[d]){
							if(ma_tim<t[d]) ma_tim=t[d],mak=d;
						}
						if(mak==i){
							if(vis[i]==1){
								if(timer.get()-t[i]>0.2){
									t[i]=timer.get(),vis[i]=2;
									if(now+1<cnt_var) print(now),now++,print(now,1);
								}
							}
							else if(timer.get()-t[i]>0.05){
								t[i]=timer.get();
								if(now+1<cnt_var) print(now),now++,print(now,1);
							}
						}
					}
				}
			}
		}
	}
}

#endif
