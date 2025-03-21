#ifndef CHECK_HPP
#define CHECK_HPP

#include <bits/stdc++.h>
using namespace std;
mt19937 rd(chrono::steady_clock::now().time_since_epoch().count());
namespace Check{
	struct Type{
		int type,len;
		/*
		0: 炸弹
		1: 单牌
		2: 对子
		3: 三张
		4: 三带一
		5: 三带二
		6: 顺子
		7: 连对
		8: 三顺
		9: 四带二
		10: 单翅膀飞机
		11: 双翅膀飞机
		*/
		int val;
	};
	Type check(vector<int>p){
		if(!p.size()) return Type{-1,0,0};
		sort(p.begin(),p.end());
		if(p[0]==-1) return Type{-1,0,0};
		int sz=p.size();
		int cnt[20];memset(cnt,0,sizeof(cnt));
		for(auto x:p) cnt[x]++;
		if(sz==2){
			if(cnt[13]&&cnt[14]) return Type{0,0,13};
		}
		if(sz==4){
			if(p[0]==p[3]) return Type{0,0,p[0]};
		}
		if(sz==1) return Type{1,0,p[0]};
		if(sz==2){
			if(p[0]==p[1]) return Type{2,0,p[0]};
		}
		if(sz==3){
			if(p[0]==p[2]) return Type{3,0,p[0]};
		}
		if(sz==4){
			int flag1=0,flag3=0,la=-1;
			for(int i=0;i<=14;i++) flag1|=(cnt[i]==1),flag3|=(cnt[i]==3),la=(cnt[i]==3?i:la);
			if(flag1&&flag3) return Type{4,0,la};
		}
		if(sz==5){
			int flag2=0,flag3=0,la=-1;
			for(int i=0;i<=14;i++) flag2|=(cnt[i]==2),flag3|=(cnt[i]==3),la=(cnt[i]==3?i:la);
			if(flag2&&flag3) return Type{5,0,la};
		}
		if(sz>=5){
			int flag=0;
			for(int i=0;i<(int)sz;i++) flag|=(p[i]!=p[0]+i);
			if(!flag&&p.back()<=11) return Type{6,sz,p[0]};
		}
		if(sz>=6&&sz%2==0){
			int flag=0;
			for(int i=0;i<(int)sz;i++) flag|=(p[i]!=p[0]+i/2);
			if(!flag&&p.back()<=11) return Type{7,sz/2,p[0]};
		}
		if(sz>=6&&sz%3==0){
			int flag=0;
			for(int i=0;i<(int)sz;i++) flag|=(p[i]!=p[0]+i/3);
			if(!flag&&p.back()<=11) return Type{8,sz/3,p[0]};
		}
		if(sz==6){
			for(int i=0;i<=14;i++) if(cnt[i]==4) return Type{9,0,i};
		}
		if(sz>=8&&sz%4==0){
			for(int i=0;i<=11;i++) if(cnt[i]>=3){
				int j=i;
				while(j<11&&cnt[j+1]>=3) j++;
				if(j-i+1==sz/4) return Type{10,sz/4,i};
			}
		}
		if(sz>=10&&sz%5==0){
			int cnt2=0;
			for(int i=0;i<=12;i++) cnt2+=(cnt[i]==2);
			for(int i=0;i<=11;i++) if(cnt[i]==3){
				int j=i;
				while(j<11&&cnt[j+1]==3) j++;
				if(j-i+1==sz/5&&cnt2==sz/5) return Type{11,sz/5,i};
			}
		}
		return Type{-1,0,0};
	}
	bool cmp(vector<int>u,vector<int>v){
		Type p=check(u),q=check(v);
		if(q.type==-1) return 0;
		if(p.type==-1) return 1;
		if(p.type&&!q.type) return 1;
		if(p.type!=q.type||p.len!=q.len) return 0;
		return p.val<q.val;
	}
}

#endif