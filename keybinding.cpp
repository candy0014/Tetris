#include "interactive.h"
#include "function.h"
#include "timer.h"
#include <vector>
#include <iostream>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __linux__
#include <linux/input.h>
#endif

std::string name[15]={
	"Move Piece Left",
	"Move Piece Right",
	"Soft Drop",
	"Hard Drop",
	"Rotate Clockwise",
	"Rotate Counterclockwise",
	"Rotate 180",
	"Swap Hold Piece",
	"Retry Game",
	"Open Config"
};
int val[15];
void print(int i,int op=0,int len=20){
	if(op){
		if(!FSBorYPA) Interactive::setcol(-2);
		else Interactive::setcol(-1);
	}
	Interactive::gotoxy(i+3,5);
	std::cout<<name[i]<<" = ";
	if(op==2){
		for(int i=1;i<=len;i++) std::cout<<" ";
	}
	else{
		std::cout<<val[i];
	}
	if(op) Interactive::setcol(-3);
}
void print_page(){
	Function::clear();
	if(!FSBorYPA) Interactive::setcol(-6);
	else Interactive::setcol(-1);
	Interactive::gotoxy(2,4);
	std::cout<<"CONTROLS";
	Interactive::setcol(-3);
	for(int i=0;i<10;i++) print(i);
}
int main(){
	Function::cur_hide();
	Function::rgb_init();
	std::ifstream conf;
	conf.open("keybinding.txt",std::ios::in);
	for(int i=0;i<10;i++){
		conf>>val[i];
		if(val[i]==-1) val[i]=defval[i];
	}
	conf.close();
	print_page();
	int now=0;print(0,1);
	int vis[128];
	double t[128];
	for(int i=0;i<128;i++) vis[i]=0,t[i]=0;
	for(auto i:{KEY_UP,KEY_DOWN}) t[i]=timer.get(),vis[i]=-Interactive::keydown(i);
	while(1){
		for(auto i:{KEY_UP,KEY_DOWN,KEY_ENTER}){
			int tmp=Interactive::keydown(i);
			if(!vis[i]&&!tmp) continue;
			if(vis[i]&&!tmp){
				if(vis[i]!=-1&&i==KEY_ENTER){
					print(now,2);
					Interactive::gotoxy(now+3,name[now].length()+8);
					while(1){
						int flag=-1;
						for(int j=0;j<256;j++) if(Interactive::keydown(j)){flag=j;break;}
						if(flag!=-1){val[now]=flag;break;}
					}
					std::ofstream outf;
					outf.open("keybinding.txt",std::ios::out|std::ios::trunc);
					for(int j=0;j<10;j++) outf<<val[j]<<"\n";
					outf.close();
					print(now,1);
					Function::cur_hide();
					vis[i]=-1;
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
					if(mak==i) print(now),now=(now-1+10)%10,print(now,1);
				}
				if(i==KEY_DOWN){
					double ma_tim=0;int mak=0;
					for(auto d:{KEY_UP,KEY_DOWN}) if(vis[d]){
						if(ma_tim<t[d]) ma_tim=t[d],mak=d;
					}
					if(mak==i) print(now),now=(now+1)%10,print(now,1);
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
								print(now),now=(now-1+10)%10,print(now,1);
							}
						}
						else if(timer.get()-t[i]>0.05){
							t[i]=timer.get();
							print(now),now=(now-1+10)%10,print(now,1);
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
								print(now),now=(now+1)%10,print(now,1);
							}
						}
						else if(timer.get()-t[i]>0.05){
							t[i]=timer.get();
							print(now),now=(now+1)%10,print(now,1);
						}
					}
				}
			}
		}
	}
	return 0;
}