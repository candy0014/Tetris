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
	if(Model==1||Model==2){
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
		tim=timer.get();
		Interactive::go(1,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"┏━━━┓";
		Interactive::go(2,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"    ┃";
		Interactive::go(3,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"┏━━━┛";
		Interactive::go(4,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"┃    ";
		Interactive::go(5,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"┗━━━┛";
		while(timer.get()-tim<1){
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
		tim=timer.get();
		Interactive::go(1,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"  ┓  ";
		Interactive::go(2,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"  ┃  ";
		Interactive::go(3,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"  ┃  ";
		Interactive::go(4,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"  ┃  ";
		Interactive::go(5,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"  ┻  ";
		while(timer.get()-tim<1){
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
		tim=timer.get();
		Interactive::go(1,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"     ";
		Interactive::go(2,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"     ";
		Interactive::go(3,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"     ";
		Interactive::go(4,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"     ";
		Interactive::go(5,mapWidth/2,-(mapWidth%2==0)-2),std::cout<<"     ";
	}
	Init::last_tim=Init::last_tim2=Init::begin_tim=timer.get();
	for(int i=0;i<Bag*7;i++){
		setvbuf(stdout,NULL,_IOFBF,4096);
		for(int j=0;j<Next_num;j++){
			Block::get_block(Init::bl[i+j]).put_next(j,0);
			Block::get_block(Init::bl[i+j+1]).put_next(j);
		}
		setvbuf(stdout,NULL,_IONBF,0);
		int res=Place::play(board,Block::get_block(Init::bl[i]),flag|(!Open_hold));
		if(res==2) return;
		if(res==3){
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
		if(Model==1&&Init::cnt_line>=lim_line) break;
		if(Model==2&&timer.get()-Init::begin_tim>lim_time) break;
	}
	if(Model==1||Model==2){
		if(Model==1){
			double tim=timer.get()-Init::begin_tim;
			Interactive::setcol(3),Interactive::go(1,mapWidth/2,-((mapWidth%2==0)+(Init::get_dig((int)tim))+4)/2);
			printf("%.3f",tim);
		}
		if(Model==2){
			Interactive::setcol(3),Interactive::go(1,mapWidth/2,-((mapWidth%2==0)+Init::get_dig(Init::score))/2);
			std::cout<<Init::score;
		}
		while(1){
			int flag=0;
			for(auto x:KEY) if(custom[x]=="RE"&&Interactive::keydown(x)){flag=1;break;}
			if(flag) break;
		}
	}
}

}

#endif
