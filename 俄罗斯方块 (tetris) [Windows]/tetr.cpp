#include "init.h"
#include "game.h"
int main(){
	Init::cur_hide();
	Init::rgb_init();
	system("chcp 65001");
	while(1){
		Init::init();
		Game::game();
	}
	return 0;
}
