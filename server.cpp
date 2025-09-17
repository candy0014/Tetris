#include "socket.h"
#ifdef _WIN32
#include <windows.h>
#endif
#include <bits/stdc++.h>
using namespace std;
mt19937 rd(time(0));
Socket::UDP sock;
Socket::Datagram pl[2];
void send(int id,string s){
	cout<<"send: "<<id<<" "<<s<<"\n";
	sock.send(pl[id].address,s);
}
string receive(){
	string s=sock.receive().data;
	return s;
}
string receive_(){
	string s;
	while((s=receive())=="fail");
	return s;
}
string receive_(string t){
	string s;
	while((s=receive())!=t);
	return s;
}
void receive_clear(){
	string s;
	while((s=receive())!="fail");
}
int main(){
	sock.bind(3000);
	sock.setNonBlocking();
	for(int i=0;i<2;i++){
		while((pl[i]=sock.receive()).data=="fail");
		cout<<pl[i].data<<"\n";
	}

	while(1){
		receive_clear();
		send(0,"start1"),send(1,"start1");
		int cnt_start=0,flag_end=0;
		while(cnt_start<2){
			string s=receive_();
			if(s=="end_"){flag_end=1;break;}
			if(s=="start2") cnt_start++,cout<<cnt_start<<"!\n";
		}
		if(flag_end) continue;
		send(0,"start3"),send(1,"start3");
		long long seed=rd();
		send(0,std::to_string(seed)),send(1,std::to_string(seed));
		receive_("start4"),receive_("start4");
		send(0,"start5"),send(1,"start5");
		cout<<"start\n";
		receive_("start6"),receive_("start6");
		while(1){
			Socket::Datagram tmp=sock.receive();
			if(tmp.data=="fail") continue;
			if(tmp.data=="end"){
				send(0,"end"),send(1,"end");
				receive_("end"),receive_("end");
				break;
			}
			int t=(tmp.address.port==pl[0].address.port);
			send(t,tmp.data);
		}
		cout<<"end\n";
	}
	return 0;
}