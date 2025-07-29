#include "init.h"
#include "game.h"
#include "config.h"
#include <string.h>
int main(){
	system("chcp 65001");
	Init::cur_hide();
	Init::rgb_init();
	while(1){
		Init::init();
		Game::game();
	}
	return 0;
}