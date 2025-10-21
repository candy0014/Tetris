#ifndef SETTING_H
#define SETTING_H

#include "interactive.h"
#include "timer.h"
#include "init.h"
#include "function.h"
namespace Setting{
	#include <vector>
	struct node{
		std::string name;
		int *a;
		double *b;
		std::string *c;
		int lin;
		node(std::string _name="",int *_a=NULL,double *_b=NULL,std::string *_c=NULL,int _lin=0){
			name=_name,a=_a,b=_b,c=_c,lin=_lin;
		}
	};
	std::string title[105];
	std::vector<node>var[105];
	int cnt_var[105];
	int cnt_lin,cnt_pag;
	void insert0(std::string name,int *a){
		var[cnt_pag].emplace_back(node(name,a,NULL,NULL,cnt_lin));
		cnt_var[cnt_pag]++,cnt_lin++;
	}
	void insert1(std::string name,double *a){
		var[cnt_pag].emplace_back(node(name,NULL,a,NULL,cnt_lin));
		cnt_var[cnt_pag]++,cnt_lin++;
	}
	void insert2(std::string name,std::string *a){
		var[cnt_pag].emplace_back(node(name,NULL,NULL,a,cnt_lin));
		cnt_var[cnt_pag]++,cnt_lin++;
	}
	int now_pag;
	void print(int i,int op=0,int len=20){
		if(op){
			if(!FSBorYPA) Interactive::setcol(-2);
			else Interactive::setcol(-1);
		}
		Interactive::gotoxy(var[now_pag][i].lin,5);
		std::cout<<var[now_pag][i].name<<" = ";
		if(op==2){
			for(int i=1;i<=len;i++) std::cout<<" ";
		}
		else{
			if(var[now_pag][i].a!=NULL) std::cout<<(*var[now_pag][i].a);
			if(var[now_pag][i].b!=NULL) std::cout<<(*var[now_pag][i].b);
			if(var[now_pag][i].c!=NULL) std::cout<<(*var[now_pag][i].c);
		}
		if(op) Interactive::setcol(-3);
	}
	void print_page(){
		Function::clear();
		if(!FSBorYPA) Interactive::setcol(-4);
		else Interactive::setcol(-1);
		Interactive::gotoxy(2,4);
		std::cout<<title[now_pag];
		Interactive::setcol(-3);
		for(int i=0;i<cnt_var[now_pag];i++) print(i);
	}
	int flag_first;
	void setting(){
		if(!flag_first) now_pag=0,flag_first=1;
		cnt_lin=3,cnt_pag=0,var[cnt_pag].clear(),cnt_var[cnt_pag]=0;
		title[cnt_pag]="MODEL";
		insert0("Model",&UserConfig::Model);
		insert0("Racing Distance",&UserConfig::RacingDistance);
		insert1("Blitz Time",&UserConfig::BlitzTime);
		cnt_lin=3,cnt_pag++,var[cnt_pag].clear(),cnt_var[cnt_pag]=0;
		title[cnt_pag]="GARBAGE";
		insert0("Garbage Model",&UserConfig::GarbageModel);
		insert0("Cheese Model",&UserConfig::CheeseModel);
		insert0("Num of Hole",&UserConfig::HoleNum);
		insert0("Layer Height",&UserConfig::LayerHeight);
		insert1("Time Interval",&UserConfig::TimeInterval);
		insert1("Cheese Messiness",&UserConfig::CheeseMessiness);
		insert1("Garbage Multiple",&UserConfig::GarbageMultiple);
		insert1("APM for Survival",&UserConfig::SurvivalAPM);
		cnt_lin=3,cnt_pag++,var[cnt_pag].clear(),cnt_var[cnt_pag]=0;
		title[cnt_pag]="HANDLING";
		insert1("Speed",&UserConfig::Speed);
		insert1("SDF",&UserConfig::SDF);
		insert1("DAS",&UserConfig::DAS);
		insert1("ARR",&UserConfig::ARR);
		cnt_lin=3,cnt_pag++,var[cnt_pag].clear(),cnt_var[cnt_pag]=0;
		title[cnt_pag]="GAME";
		insert0("Width",&UserConfig::mapWidth);
		insert0("Height",&UserConfig::mapHeight);
		insert0("Num of Next",&UserConfig::NextNum);
		insert0("Open Hold",&UserConfig::OpenHold);
		insert0("Invisible",&UserConfig::Invisible);
		insert0("Anti Gravity",&UserConfig::AntiGravity);
		insert0("Ghost",&UserConfig::Ghost);
		insert1("EPLD",&UserConfig::EPLD);
		insert0("Limit of EPLD",&UserConfig::EPLDLim);
		insert2("Rotation System",&UserConfig::RotationSystem);
		cnt_lin=3,cnt_pag++,var[cnt_pag].clear(),cnt_var[cnt_pag]=0;
		title[cnt_pag]="AUTOPLAY";
		insert0("Autoplay",&UserConfig::Autoplay);
		insert1("PPS of Autoplay",&UserConfig::Autoplay_PPS);
		cnt_lin=3,cnt_pag++,var[cnt_pag].clear(),cnt_var[cnt_pag]=0;
		title[cnt_pag]="OTHERS";
		insert0("Windows Version",&UserConfig::WindowsVersion);
		insert0("FSBorYPA",&UserConfig::FSBorYPA);
		cnt_pag++;
		print_page();
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
			for(auto i:{KEY_UP,KEY_DOWN,KEY_ENTER,KEY_LEFT,KEY_RIGHT}){
				int tmp=Interactive::keydown(i);
				if(!vis[i]&&!tmp) continue;
				if(vis[i]&&!tmp){
					if(vis[i]!=-1&&i==KEY_ENTER){
						print(now,2);
						Interactive::gotoxy(var[now_pag][now].lin,var[now_pag][now].name.length()+8);
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
							if(var[now_pag][now].a!=NULL){
								int v=0,flag=1;
								for(int j=0;j<(int)tmp.length();j++){
									if('0'<=tmp[j]&&tmp[j]<='9') v=v*10+tmp[j]-'0';
									if(tmp[j]=='-'&&v==0&&flag==1) flag=-1;
								}
								(*var[now_pag][now].a)=v*flag;
							}
							if(var[now_pag][now].b!=NULL){
								double v=0,w=1;int flag=0,flagg=1;
								for(int j=0;j<(int)tmp.length();j++){
									if('0'<=tmp[j]&&tmp[j]<='9'){
										if(!flag) v=v*10+tmp[j]-'0';
										else w/=10,v+=w*(tmp[j]-'0');
									}
									if(tmp[j]=='-'&&v==0&&flagg==1&&flag==0) flagg=-1;
									if(tmp[j]=='.') flag=1;
								}
								(*var[now_pag][now].b)=v*flagg;
							}
							if(var[now_pag][now].c!=NULL) (*var[now_pag][now].c)=tmp;
						}
						print(now,2,tmp.length()),print(now,1);
						Function::cur_hide();
						vis[i]=-1;
						if(var[now_pag][now].name=="FSBorYPA"){
							FSBorYPA=std::min(1,std::max(0,UserConfig::FSBorYPA));
							print_page(),print(now,1);
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
						if(mak==i) print(now),now=(now-1+cnt_var[now_pag])%cnt_var[now_pag],print(now,1);
					}
					if(i==KEY_DOWN){
						double ma_tim=0;int mak=0;
						for(auto d:{KEY_UP,KEY_DOWN}) if(vis[d]){
							if(ma_tim<t[d]) ma_tim=t[d],mak=d;
						}
						if(mak==i) print(now),now=(now+1)%cnt_var[now_pag],print(now,1);
					}
					if(i==KEY_LEFT){
						now_pag=(now_pag-1+cnt_pag)%cnt_pag,now=0;
						print_page(),print(0,1);
					}
					if(i==KEY_RIGHT){
						now_pag=(now_pag+1)%cnt_pag,now=0;
						print_page(),print(0,1);
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
									print(now),now=(now-1+cnt_var[now_pag])%cnt_var[now_pag],print(now,1);
								}
							}
							else if(timer.get()-t[i]>0.05){
								t[i]=timer.get();
								print(now),now=(now-1+cnt_var[now_pag])%cnt_var[now_pag],print(now,1);
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
									print(now),now=(now+1)%cnt_var[now_pag],print(now,1);
								}
							}
							else if(timer.get()-t[i]>0.05){
								t[i]=timer.get();
								print(now),now=(now+1)%cnt_var[now_pag],print(now,1);
							}
						}
					}
				}
			}
		}
	}
}

#endif
