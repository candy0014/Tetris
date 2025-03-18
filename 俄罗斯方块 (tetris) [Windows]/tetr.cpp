#include "init.h"
#include "game.h"
int main(){
	Init::init();
	Game::game();
	while(1);
	return 0;
}