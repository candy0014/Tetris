#include "init.h"
#include "game.h"
#include "config.h"
#include <string.h>
int main(){
	system("chcp 65001");
	int lines=mapHeightP+mapHeightN+3,cols=Margin*2+mapWidth*2;
	std::string _l="",_c="";
	while(lines) _l=char(lines%10+'0')+_l,lines/=10;
	while(cols) _c=char(cols%10+'0')+_c,cols/=10;
	std::string tmp="mode con cols="+_c+" lines="+_l;
	system(tmp.c_str());
	Init::cur_hide();
	Init::rgb_init();
	while(1){
		Init::init();
		Game::game();
	}
	return 0;
}
