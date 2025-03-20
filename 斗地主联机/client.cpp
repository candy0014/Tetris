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
Socket::UDP sock;
string server_id;
multiset<int>card;
int num,ld;
string get(){
	return sock.receive().data;
}
void send(string s){
	sock.send(server_id,3000,s);
}
int len[3];
string _name,name[3];
void print(){
	#ifdef _WIN32
	system("cls");
	#endif
	#ifdef __linux__
	system("clear");
	#endif
	for(int i=0;i<3;i++){
		cout<<"Player "<<i<<" ("<<name[i]<<"): ";
		if(i==num){
			for(auto x:card) cout<<c[x];cout<<"\n";
		}
		else{
			for(int j=0;j<len[i];j++) cout<<"∎";cout<<"\n";
		}
	}
}
int main(){
	#ifdef _WIN32
	system("chcp 65001");system("cls");
	#endif
	memset(cc,-1,sizeof(cc));
	for(int i=0;i<=14;i++) cc[c[i]]=i;
	int my_port=rd()%5000+3000;
	sock.bind(my_port);
	cout<<"Enter the IP address of the server:\n";
	// server_id="";
	cin>>server_id;
	cout<<"Enter your username:\n";
	cin>>_name;
	send(_name);
	while(1){
		#ifdef _WIN32
		system("cls");
		#endif
		#ifdef __linux__
		system("clear");
		#endif
		card.clear();
		for(int i=0;i<3;i++) name[i]=get();
		num=string_to_int(get());
		for(int i=0;i<17;i++){
			card.insert(string_to_int(get()));
		}
		ld=string_to_int(get());
		if(ld==num){
			for(int i=0;i<3;i++) card.insert(string_to_int(get()));
		}
		len[0]=len[1]=len[2]=17,len[ld]=20;
		int now=0,la=0;
		vector<int>last;
		while(1){
			print();
			if(get()=="end") break;
			cout<<"Now it's number "<<now<<" playing\n\n";
			if(la==now) last.clear();
			if(last.size()){
				cout<<"Player "<<la<<": ";
				sort(last.begin(),last.end());
				for(auto x:last) cout<<c[x];
				cout<<"\n";
			}
			if(now==num){
				cout<<"You (Enter \"dc\" discard): ";
				string str;
				while(1){
					cin>>str;
					send(str);
					string tmp=get();
					if(tmp=="yes") break;
				}
				if(str!="dc"){
					for(auto x:str) card.erase(card.find(cc[x]));
				}
			}
			string str=get();
			if(str=="dc"){now=(now+1)%3;continue;}
			len[now]-=str.length();
			la=now;
			last.clear();
			for(auto x:str) last.emplace_back(cc[x]);
			now=(now+1)%3;
		}
		cout<<"Player "<<la<<": ";
		sort(last.begin(),last.end());
		for(auto x:last) cout<<c[x];
		cout<<"\n\n";
		if(!len[0]) cout<<"Landlord Wins!\n";
		else cout<<"Farmers Win!\n";
		cout<<"\n";
		for(int i=0;i<3;i++){
			cout<<name[i]<<"\'s score: "<<get()<<"\n";
		}
		cout<<"\n"<<"Enter anything to continue: ";
		string str;
		cin>>str;
		send(str);
		get();
	}
	sock.close();
	return 0;
}