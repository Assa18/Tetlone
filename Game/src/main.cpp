#include "Game.h"

int main()
{
	Game game;

	game.Initialize();
	game.Run();
	game.Cleanup();

	return 0;
}