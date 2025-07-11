#ifndef BLOCK_H
#define BLOCK_H

#include "map.h"
#include "interactive.h"
#include <iostream>

namespace Block{

int I[4][4][2]={{0,-1,0,0,0,1,0,2},{-1,1,0,1,1,1,2,1},{1,-1,1,0,1,1,1,2},{-1,0,0,0,1,0,2,0}};
int J[4][4][2]={{0,0,0,-1,0,1,-1,-1},{0,0,-1,0,1,0,-1,1},{0,0,0,-1,0,1,1,1},{0,0,-1,0,1,0,1,-1}};
int L[4][4][2]={{0,0,0,-1,0,1,-1,1},{0,0,-1,0,1,0,1,1},{0,0,0,-1,0,1,1,-1},{0,0,-1,0,1,0,-1,-1}};
int O[4][4][2]={{0,0,-1,0,-1,1,0,1},{0,0,-1,0,-1,1,0,1},{0,0,-1,0,-1,1,0,1},{0,0,-1,0,-1,1,0,1}};
int S[4][4][2]={{0,0,-1,0,-1,1,0,-1},{0,0,0,1,1,1,-1,0},{0,0,0,1,1,0,1,-1},{0,0,1,0,0,-1,-1,-1}};
int T[4][4][2]={{0,0,0,1,0,-1,-1,0},{0,0,-1,0,1,0,0,1},{0,0,0,1,0,-1,1,0},{0,0,-1,0,1,0,0,-1}};
int Z[4][4][2]={{0,0,0,1,-1,0,-1,-1},{0,0,0,1,-1,1,1,0},{0,0,1,0,0,-1,1,1},{0,0,-1,0,0,-1,1,-1}};
int chks[12][6][2]={
{0,0,-1,0,-1,1,0,-2,-1,-2,0,0},
{0,0,1,0,1,-1,0,2,1,2,0,0},
{0,0,1,0,1,-1,0,2,1,2,0,0},
{0,0,-1,0,-1,1,0,-2,-1,-2,0,0},
{0,0,1,0,1,1,0,-2,1,-2,0,0},
{0,0,-1,0,-1,-1,0,2,-1,2,0,0},
{0,0,-1,0,-1,-1,0,2,-1,2,0,0},
{0,0,1,0,1,1,0,-2,1,-2,0,0},
{0,0,0,1,1,1,-1,1,1,0,-1,0},
{0,0,0,-1,-1,-1,1,-1,-1,0,1,0},
{0,0,1,0,1,2,1,1,0,2,0,1},
{0,0,-1,0,-1,2,-1,1,0,2,0,1}
};
int chkI[12][6][2]={
{0,0,1,0,-2,0,-2,-1,1,2,0,0},
{0,0,-1,0,2,0,-1,-2,2,1,0,0},
{0,0,-1,0,2,0,-1,2,2,-1,0,0},
{0,0,-2,0,1,0,-2,1,1,-2,0,0},
{0,0,2,0,-1,0,2,1,-1,-2,0,0},
{0,0,1,0,-2,0,1,2,-2,-1,0,0},
{0,0,1,0,-2,0,2,-1,-2,1,0,0},
{0,0,-1,0,2,0,2,-1,-1,2,0,0},
{0,0,0,1,1,1,-1,1,1,0,-1,0},
{0,0,0,-1,-1,-1,1,-1,-1,0,1,0},
{0,0,1,0,1,2,1,1,0,2,0,1},
{0,0,-1,0,-1,2,-1,1,0,2,0,1}
};
struct block{
	int shape[4][4][2],chk[12][6][2],ty;
	bool check(int x,int y,int type,map mp){
		for(int i=0;i<4;i++){
			int tx=x+shape[type][i][0],ty=y+shape[type][i][1];
			if(ty<0||ty>=mapWidth||tx>=mapHeightP) return 0;
			if(mp[tx][ty]!=-1) return 0;
		}
		return 1;
	}
	void put_block(int x,int y,int type,int op=1){
		if(op==1) Interactive::setcol(ty);
		else Interactive::setcol(-1);
		for(int i=0;i<4;i++){
			Interactive::go(x+shape[type][i][0],y+shape[type][i][1]);
			if(op) std::cout<<"█ ";
			else std::cout<<"  ";
		}
		Interactive::go(mapHeightP+2,0);
	}
	void put(int x,int y,int type,map mp,int op=1){
		int xx=x;
		while(check(xx+1,y,type,mp)) xx++;
		if(Ghost) put_block(xx,y,type,-op);
		put_block(x,y,type,op);
	}
	void put_hold(int op=1){
		if(op==1) Interactive::setcol(ty);
		else Interactive::setcol(-1);
		for(int i=0;i<4;i++){
			Interactive::go_hold(shape[0][i][0],shape[0][i][1],ty);
			if(op) std::cout<<"█ ";
			else std::cout<<"  ";
		}
	}
	void put_next(int x,int op=1){
		if(Next_num==0) return;
		Interactive::setcol(ty);
		for(int i=0;i<4;i++){
			Interactive::go_next(x,shape[0][i][0],shape[0][i][1],ty);
			if(op) std::cout<<"█ ";
			else std::cout<<"  ";
		}
	}
	bool checks(int &x,int &y,int last_type,int type,map mp){
		int op=8;
		if(last_type==0&&type==1) op=0;
		if(last_type==1&&type==0) op=1;
		if(last_type==1&&type==2) op=2;
		if(last_type==2&&type==1) op=3;
		if(last_type==2&&type==3) op=4;
		if(last_type==3&&type==2) op=5;
		if(last_type==3&&type==0) op=6;
		if(last_type==0&&type==3) op=7;
		if(last_type==0&&type==2) op=8;
		if(last_type==2&&type==0) op=9;
		if(last_type==1&&type==3) op=10;
		if(last_type==3&&type==1) op=11;
		for(int i=0;i<6;i++) if(check(x-chk[op][i][1],y+chk[op][i][0],type,mp)){
			x-=chk[op][i][1],y+=chk[op][i][0];
			return 1;
		}
		return 0;
	}
};
block get_block(int type){
	block res;
	if(type==0) memcpy(res.shape,I,sizeof(I));
	if(type==1) memcpy(res.shape,J,sizeof(J));
	if(type==2) memcpy(res.shape,L,sizeof(L));
	if(type==3) memcpy(res.shape,O,sizeof(O));
	if(type==4) memcpy(res.shape,S,sizeof(S));
	if(type==5) memcpy(res.shape,T,sizeof(T));
	if(type==6) memcpy(res.shape,Z,sizeof(Z));
	if(type==0) memcpy(res.chk,chkI,sizeof(chkI));
	else memcpy(res.chk,chks,sizeof(chks));
	res.ty=type;
	return res;
}

}

#endif