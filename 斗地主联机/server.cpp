#include "socket.hpp"
#include "check.hpp"
#include <bits/stdc++.h>
using namespace std;
string int_to_string(int x){
	if(x<0) return "-"+int_to_string(-x);
	if(x<=9){
		string tmp="";tmp+=char(x+'0');
		return tmp;
	}
	return int_to_string(x/10)+char(x%10+'0');
}
int string_to_int(string s){
	int x=0;
	for(int i=0;s[i];i++) if('0'<=s[i]&&s[i]<='9') x=x*10+s[i]-'0';
	if(s[0]=='-') x=-x;
	return x;
}
string c="3456789TJQKA2XY";
int cc[256];
vector<int> string_to_vector(string str){
	vector<int>s;
	for(int i=0;str[i];i++) s.emplace_back(cc[str[i]]);
	return s;
}
Socket::UDP sock;
Socket::Datagram pl[3];
string get(int id=-1){
	Socket::Datagram tmp=sock.receive();
	// assert(id==-1||tmp.address.ip==pl[id].address.ip);
	return tmp.data;
}
void send(int id,string s){
	sock.send(pl[id].address,s);
}
vector<int>Card;
multiset<int>card[3];
int ld=0;
int len[3];
map<string,int> score;
int main(){
	memset(cc,-1,sizeof(cc));
	for(int i=0;i<=14;i++) cc[c[i]]=i;
	sock.bind(3000);
	for(int i=0;i<3;i++){
		pl[i]=sock.receive();
	}
	while(1){
		Card.clear();
		for(int i=0;i<3;i++) card[i].clear();
		shuffle(pl,pl+3,rd);
		for(int i=0;i<3;i++) for(int j=0;j<3;j++) send(i,pl[j].data);
		for(int i=0;i<3;i++) send(i,int_to_string(i));
		for(int i=0;i<=12;i++) for(int j=0;j<4;j++) Card.emplace_back(i);
		Card.emplace_back(13),Card.emplace_back(14);
		shuffle(Card.begin(),Card.end(),rd);
		for(int i=0;i<51;i++) send(i%3,int_to_string(Card[i])),card[i%3].insert(Card[i]);
		for(int i=0;i<3;i++) send(i,int_to_string(ld));
		for(int i=51;i<54;i++) send(ld,int_to_string(Card[i])),card[ld].insert(Card[i]);
		len[0]=len[1]=len[2]=17,len[ld]=20;
		int now=0,la=0;
		vector<int>last;
		while(1){
			if(!len[0]||!len[1]||!len[2]){
				for(int i=0;i<3;i++) send(i,"end");
				break;
			}
			if(la==now) last.clear();
			for(int i=0;i<3;i++) send(i,"continue");
			vector<int>s;
			string str;
			while(1){
				str=get(now);
				s.clear();
				if(str=="DC"&&last.size()) break;
				for(int i=0;str[i];i++) s.emplace_back(cc[str[i]]);
				if(Check::check(s).type==-1){send(now,"no");continue;}
				multiset<int>tmp=card[now];
				int flag=0;
				for(auto x:s){
					if(tmp.find(x)!=tmp.end()) tmp.erase(tmp.find(x));
					else{flag=1;break;}
				}
				if(flag){send(now,"no");continue;}
				if(Check::cmp(last,s)) break;
				send(now,"no");
			}
			for(auto x:s) card[now].erase(card[now].find(x));
			send(now,"yes");
			for(int i=0;i<3;i++) send(i,str);
			if(str=="DC"){now=(now+1)%3;continue;}
			len[now]-=str.length();
			la=now,last.clear();
			for(auto x:s) last.emplace_back(x);
			now=(now+1)%3;
		}
		if(!len[0]){
			score[pl[0].data]+=3;
		}
		else{
			score[pl[1].data]++;
			score[pl[2].data]++;
		}
		for(int i=0;i<3;i++) for(int j=0;j<3;j++) send(j,int_to_string(score[pl[i].data]));
		get(),get(),get();
		for(int i=0;i<3;i++) send(i,"continue");
	}
	sock.close();
	return 0;
}