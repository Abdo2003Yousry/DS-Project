#include "Battle.h"
#include "GUI\GUI.h"

int main()
{
	Battle* game = new Battle;
	game->RunSimulation();
	
	delete game;
	
	return 0;
}
