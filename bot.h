#ifndef BOT_H
#define BOT_H

#include "map.h"
#include "block.h"
#include <vector>

namespace Bot{

void evaluate(map mp,Block::block B,int x,int y,int type,double &score){
	/* A Naive Example
		score=x;
		return;
	*/
	for(int i=0;i<4;i++) mp[x+B.shape[type][i][0]][y+B.shape[type][i][1]]=B.ty;
	int tag[105],cnt_clear=0,pc_flag=1;
	for(int i=-mapHeightN;i<mapHeight;i++){
		tag[i+mapHeightN]=1;
		for(int j=0;j<mapWidth;j++){
			if(mp[i][j]==-1){tag[i+mapHeightN]=0;break;}
		}
		for(int j=1;j<mapWidth&&pc_flag;j++) if(std::min(0,mp[i][j])!=std::min(0,mp[i][0])) pc_flag=0;
		cnt_clear+=tag[i+mapHeightN];
	}
	int now=mapHeight;
	if(AntiGravity){
		for(int i=mapHeight-1;i>=-mapHeightN;i--) if(tag[i+mapHeightN]){
			for(int j=0;j<mapWidth;j++) mp[i][j]=-1;
		}
	}
	else{
		for(int i=mapHeight-1;i>=-mapHeightN;i--){
			now--;
			while(now+mapHeightN>=1&&tag[now+mapHeightN]) now--;
			now=std::max(now,1-mapHeightN);
			mp[i]=mp[now];
		}
	}
	if(cnt_clear==1) score-=1.5;
	if(cnt_clear==2) score-=3;
	if(cnt_clear==3) score-=4;
	if(cnt_clear==4) score+=4.0;
	if(pc_flag) score+=1e9;
	int h[105],max_hole=0,cntstair[4]={0,0,0,0},max_h=0,min_h=mapHeight+mapHeightN;
	for(int i=0;i<mapWidth;i++){
		h[i]=0;
		int hole=0;
		double now=2.6,sub=1;
		for(int j=-mapHeightN;j<mapHeight;j++){
			if(mp[j][i]!=-1&&h[i]==0) h[i]=mapHeight-j;
			else if(j!=-mapHeightN&&mp[j][i]==-1&&h[i]){
				if(mp[j-1][i]!=-1) now=2.5,sub=1;
				score-=now,now-=sub,sub/=1.666;
				if(mp[j-1][i]!=-1&&(j==mapHeight-1||mp[j+1][i]!=-1)){
					if((i==0||mp[j][i-1]!=-1)&&(i==mapWidth-1||mp[j][i+1]!=-1)) score-=5;
					else if((i<2||mp[j][i-1]!=-1||mp[j][i-2]!=-1)&&(i>=mapWidth-2||mp[j][i+1]!=-1||mp[j][i+2]!=-1)) score-=4;
					else score-=3;
				}
				else if(mp[j-1][i]!=-1&&(i<2||mp[j][i-1]!=-1||mp[j][i-2]!=-1)&&(i>=mapWidth-2||mp[j][i+1]!=-1||mp[j][i+2]!=-1)) score-=4;
			}
		}
		max_h=std::max(max_h,h[i]),min_h=std::min(min_h,h[i]);
		score-=h[i]*0.15;
		if(i) score-=abs(h[i]-h[i-1])*0.2,cntstair[std::min(abs(h[i]-h[i-1]),3)]++;
	}
	if(max_h>=mapHeight*0.6) score-=(max_h-mapHeight*0.6)*2.5;
	if(max_h==mapHeight-4) score-=6;
	if(max_h==mapHeight-3) score-=10;
	if(max_h==mapHeight-2) score-=10000;
	if(max_h>=mapHeight-1) score-=(max_h-(mapHeight-1)+1)*20000;
	int max_hole2_pos=0,max_hole2=0;
	for(int i=0;i<mapWidth;i++){
		int mhole=0;
		int now=mapHeight-h[i];
		while(now>-mapHeightN&&(i==0||mp[now-1][i-1]!=-1)&&(i==mapWidth-1||mp[now-1][i+1]!=-1)) now--,mhole++;
		if(mhole>=max_hole2) max_hole2=mhole,max_hole2_pos=i;
		mhole=1e9;
		if(i) mhole=std::min(mhole,h[i-1]);
		if(i!=mapWidth-1) mhole=std::min(mhole,h[i+1]);
		mhole=std::max(0,mhole-h[i]);
		if(mhole>=3) score-=std::pow(2.1,mhole-5)+0.5;
		max_hole=std::max(max_hole,mhole);
	}
	score-=(h[max_hole2_pos]-min_h)*0.15;
	if(max_hole2==0) score-=2;
	if(max_hole2==1) score-=1.2;
	if(max_hole2==2) score-=0.8;
	if(max_hole2==3) score-=0.3;
	if(max_hole2>=3) score+=std::pow(2.1,max_hole2-5)+0.5;
	score+=max_hole2*0.4;
	if(max_hole2_pos&&max_hole2_pos!=mapWidth-1) score+=max_hole2*0.2;
	if(max_hole>mapHeight/2) score-=(max_hole-mapHeight/2)*0.4;
	if(cntstair[0]<4) score-=0.1*(4-cntstair[0]);
	if(cntstair[1]>5) score-=0.12*(cntstair[1]-5);
	if(cntstair[2]>3) score-=0.17*(cntstair[2]-3);
	if(cntstair[3]>1) score-=1*(cntstair[3]-1);
	if(cntstair[1]==0) score-=0.05;
	if(cntstair[2]==0) score-=0.05;
	std::sort(h,h+mapWidth);
	score-=std::pow(1.2,max_h-h[mapWidth-4]);
}

double ansscore;
int ansx,ansy,anstype,ansrot;
struct node{
	int x,y,type,rot_op,step;
}q[400005];
int headq,tailq;
int st[105][105][4];
void update(int x,int y,int type,int rot_op,int step){
	if(rot_op>st[x+mapHeightN][y+1][type]){
		st[x+mapHeightN][y+1][type]=rot_op,q[++tailq]=node{x,y,type,rot_op,step};
	}
}
void bfs(map &mp,Block::block B,int sx,int sy,int stype){
	ansscore=-1e9;
	for(int i=0;i<mapHeight+mapHeightN;i++) for(int j=0;j<=mapWidth;j++) for(int k=0;k<4;k++) st[i][j][k]=-1;
	headq=1,tailq=0,update(sx,sy,stype,0,0);
	while(headq<=tailq){
		node u=q[headq++];
		int x=u.x,y=u.y,type=u.type,step=u.step;
		if(u.rot_op!=st[x+mapHeightN][y+1][type]) continue;
		if(!B.check(x+1,y,type,mp)){
			double score=0;
			evaluate(mp,B,x,y,type,score);
			if(score>ansscore||fabs(score-ansscore)<=1e-8&&ansrot<u.rot_op) ansscore=score,ansx=x,ansy=y,anstype=type,ansrot=u.rot_op;
		}
		int xx=x;
		while(B.check(xx+1,y,type,mp)) xx++;
		if(xx!=x) update(xx,y,type,0,step+1);
		if(B.check(x,y-1,type,mp)) update(x,y-1,type,0,step+1);
		if(B.check(x,y+1,type,mp)) update(x,y+1,type,0,step+1);
		int rotate_op;
		for(int i=1;i<=3;i++){
			int tx=x,ty=y,ttype=(type+i)%4;
			if(B.checks(tx,ty,type,ttype,mp,rotate_op)) update(tx,ty,ttype,rotate_op+1,step+1);
		}
	}
}
int flag_hold=0;
bool getpos(map mp,std::vector<int>nex,int sx,int sy,int stype,int hold,int &ans_x,int &ans_y,int &ans_type,int &ans_rot,int flag_first=0){
	if(flag_first) flag_hold=0;
	if(flag_hold){
		ans_x=ansx,ans_y=ansy,ans_type=anstype,flag_hold=0;
		return 0;
	}
	bfs(mp,Block::get_block(nex[0]),sx,sy,stype);
	ans_x=ansx,ans_y=ansy,ans_type=anstype,flag_hold=0;
	double tmp=ansscore;
	if(hold!=-2){
		if(hold==-1) return 1;
		sx=-2,sy=(mapWidth-1)/2,stype=0;
		if(Block::get_block(hold).check(sx,sy,stype,mp)){
			if(fabs(Speed)<1e-8) while(Block::get_block(hold).check(sx+1,sy,stype,mp)) sx++;
			bfs(mp,Block::get_block(hold),sx,sy,0);
			if(ansscore>tmp){
				flag_hold=1;
				return 1;
			}
		}
	}
	return 0;
}

}

#endif