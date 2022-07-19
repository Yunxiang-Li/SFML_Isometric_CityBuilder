#include "Game.hpp"
#include "GameState.hpp"

void Game::pushState(std::unique_ptr<GameState> state_ptr)
{
	this->state_stack.emplace(std::move(state_ptr));
}

void Game::popState()
{
	this->state_stack.pop();
}

void Game::switchState(std::unique_ptr<GameState> state_ptr)
{
	if (!(this->state_stack.empty()))
		this->popState();
	this->pushState(std::move(state_ptr));
}

std::unique_ptr<GameState> Game::peekState()
{
	if (this->state_stack.empty())
		return nullptr;
	return std::move(this->state_stack.top());
}

void Game::gameLoop()
{
	// Create a sf::Clock object to help track the time.
	sf::Clock game_clock;
	while (this->game_window.isOpen())
	{
		// Get the elapsed time since last frame and convert it to seconds inside a float value.
		sf::Time elapsed_time = game_clock.restart();
		float delta_time = elapsed_time.asSeconds();
		// Store the game's current state.
		std::unique_ptr<GameState> curr_game_state = this->peekState();
		// If the game is not in any states, then continue to next frame.
		if (curr_game_state == nullptr)
			continue;
		// Else handle player's input and update current frame's behavior within current game state.
		curr_game_state->inputProcess();
		curr_game_state->update(delta_time);
		// Clear the game window with black color.
		this->game_window.clear(sf::Color::Black);
		// Render and display the current frame content.
		curr_game_state->render(delta_time);
		this->game_window.display();
	}
}

Game::Game()
{
	// Create a 1280 X 720 game window with the title "City Builder" and set max frame rate to be 60fps.
	this->game_window.create(sf::VideoMode(1280, 720), "City Builder");
	this->game_window.setFramerateLimit(60);
}

Game::~Game()
{
	while (!(this->state_stack.empty()))
		this->popState();
}