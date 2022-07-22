// The main entrance of the game.
#include "MainMenuGameState.hpp"

// Create and launch the game.
int main()
{
	Game game;
	// Let game start with main menu scene first.
	game.pushState(std::make_unique<MainMenuGameState>(std::unique_ptr<Game>(&game)));
	game.gameLoop();
	return 0;
}
