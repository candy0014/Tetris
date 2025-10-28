#ifndef GAME_H
#define GAME_H

#include "place.h"
#include "init.h"
#include "block.h"
#include "timer.h"
#include "setting.h"
namespace Game{

void game(){
	int flag=0;
	if(GarbageModel==6){
		Function::send("start4");
		Function::receive_("start5");
	}
	if(1){
		
	if(Model==1||Model==2||GarbageModel==6){
		double tim=timer.get();
		Interactive::setcol(3);
		Interactive::go(1,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"┏━━━┓";
		Interactive::go(2,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"    ┃";
		Interactive::go(3,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"  ━━┫";
		Interactive::go(4,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"    ┃";
		Interactive::go(5,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"┗━━━┛";
		int fl=0,fl2=0;
		for(auto x:KEY){
			if(custom[x]=="RE"&&Interactive::keydown(x)){fl=-1;break;}
			if(custom[x]=="SET"&&Interactive::keydown(x)){fl2=-1;break;}
		}
		while(timer.get()-tim<1){
			if(GarbageModel!=6){
				for(auto x:KEY){
					if(custom[x]=="RE"){
						if(!Interactive::keydown(x)) fl=0;
						if(Interactive::keydown(x)&&fl!=-1) return;
					}
					if(custom[x]=="SET"){
						if(!Interactive::keydown(x)) fl2=0;
						if(Interactive::keydown(x)&&fl2!=-1){Setting::setting();return;}
					}
				}
			}
		}
		tim=timer.get();
		Interactive::go(1,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"┏━━━┓";
		Interactive::go(2,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"    ┃";
		Interactive::go(3,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"┏━━━┛";
		Interactive::go(4,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"┃    ";
		Interactive::go(5,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"┗━━━┛";
		while(timer.get()-tim<1){
			if(GarbageModel!=6){
				for(auto x:KEY){
					if(custom[x]=="RE"){
						if(!Interactive::keydown(x)) fl=0;
						if(Interactive::keydown(x)&&fl!=-1) return;
					}
					if(custom[x]=="SET"){
						if(!Interactive::keydown(x)) fl2=0;
						if(Interactive::keydown(x)&&fl2!=-1){Setting::setting();return;}
					}
				}
			}
		}
		tim=timer.get();
		Interactive::go(1,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"  ┓  ";
		Interactive::go(2,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"  ┃  ";
		Interactive::go(3,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"  ┃  ";
		Interactive::go(4,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"  ┃  ";
		Interactive::go(5,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"  ┻  ";
		while(timer.get()-tim<1){
			if(GarbageModel!=6){
				for(auto x:KEY){
					if(custom[x]=="RE"){
						if(!Interactive::keydown(x)) fl=0;
						if(Interactive::keydown(x)&&fl!=-1) return;
					}
					if(custom[x]=="SET"){
						if(!Interactive::keydown(x)) fl2=0;
						if(Interactive::keydown(x)&&fl2!=-1){Setting::setting();return;}
					}
				}
			}
		}
		tim=timer.get();
		Interactive::go(1,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"     ";
		Interactive::go(2,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"     ";
		Interactive::go(3,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"     ";
		Interactive::go(4,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"     ";
		Interactive::go(5,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"     ";
	}
	}
	Init::last_tim=Init::last_tim2=Init::begin_tim=Init::last_tim3=timer.get();
	Init::flag_first=0;
	if(GarbageModel==6){
		Function::send("start6");
	}
	for(int i=0;;i=(i+1)%105){
		if(i%7==0){
			for(int j=(i+98)%105,k=0;k<7;j++,k++) Init::bl[j]=k;
			Function::shuffle(Init::bl,(i+98)%105,(i+98)%105+7);
		}
		setvbuf(stdout,NULL,_IOFBF,4096);
		Init::next_block.clear();
		Init::next_block.emplace_back(Init::bl[i]);
		for(int j=0;j<NextNum;j++){
			Block::get_block(Init::bl[(i+j)%105]).put_next(j,0);
			Block::get_block(Init::bl[(i+j+1)%105]).put_next(j);
			Init::next_block.emplace_back(Init::bl[(i+j+1)%105]);
		}
		setvbuf(stdout,NULL,_IONBF,0);
		int res=Place::play(board,Block::get_block(Init::bl[i]),flag);
		if(res==2){
			if(GarbageModel==6){
				Function::receive_clear();
				Function::send("end");
			}
			// exit(0);
			return;
		}
		if(res==3){
			if(GarbageModel==6){
				Function::receive_clear();
				Function::send("end");
			}
			Setting::setting();
			return;
		}
		else if(res==1){
			if(Init::now_hold!=-1) std::swap(Init::now_hold,Init::bl[i]),i--,flag=1;
			else Init::now_hold=Init::bl[i],flag=1;
		}
		else{
			if(flag) Block::get_block(Init::now_hold).put_hold();
			flag=0;
		}
		if(Model==1&&Init::cnt_line>=RacingDistance) break;
		if(Model==2&&timer.get()-Init::begin_tim>BlitzTime) break;
	}
	if(GarbageModel==6){
		Function::receive_clear();
		Function::send("end");
	}
	if(Model==1||Model==2){
		if(Model==1){
			double tim=timer.get()-Init::begin_tim;
			Interactive::setcol(3),Interactive::go(1,mapWidth/2,-((mapWidth%2==0)+(Function::get_dig((int)tim))+4)/2);
			printf("%.3f",tim);
		}
		if(Model==2){
			Interactive::setcol(3),Interactive::go(1,mapWidth/2,-((mapWidth%2==0)+Function::get_dig(Init::score))/2);
			std::cout<<Init::score;
		}
		while(1){
			int flag=0;
			for(auto x:KEY){
				if(custom[x]=="RE"&&Interactive::keydown(x)){flag=1;break;}
				if(custom[x]=="SET"&&Interactive::keydown(x)){Setting::setting();return;}
			}
			if(flag) break;
		}
	}
}

}

#endif
