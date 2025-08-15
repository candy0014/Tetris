#include "init.h"
#include "game.h"
#include "config.h"
#include "function.h"
#include <string.h>
int main(){
	system("chcp 65001");
	Function::cur_hide();
	Function::rgb_init();
	while(1){
		Init::init();
		Game::game();
	}
	return 0;
}