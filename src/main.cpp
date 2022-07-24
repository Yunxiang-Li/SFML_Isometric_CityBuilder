// The main entrance of the game.
#include "MainMenuGameState.hpp"

// Main entrance of the game.
int main()
{
  	// Create the game
	Game game;
	std::shared_ptr<Game> game_ptr(&game);
  	MainMenuGameState main_menu_game_state(game_ptr);
	// Let game start with main menu scene first.
	game.pushState(std::make_unique<MainMenuGameState>(main_menu_game_state));
	game.gameLoop();
	return 0;
}
