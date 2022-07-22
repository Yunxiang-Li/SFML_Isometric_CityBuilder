#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include <stack>
// Include for smart pointers.
#include <memory>
// Include for sf::RenderWindow.
#include <SFML/Graphics.hpp>

// Forward declaration.
class GameState;

/**
 * This class represent the instance of the city builder game.
 */
class Game
{
	public:
	/**
	 * Constructor of Game class.
	 */
	Game();

	/**
	 * Destructor of Game class.
	 */
	~Game();

	/**
	 * Push one input game state's pointer into the state stack.
	 * @param state_ptr A std::unique_ptr indicates the input game state.
	 */
	void pushState(std::unique_ptr<GameState> state_ptr);

	/**
	 * Pop the top(current) game state out of the state stack.
	 */
	void popState();

	/**
	 * Switch from current game state to input one.
	 * @param state_ptr A std::unique_ptr indicates the input game state.
	 */
	void switchState(std::unique_ptr<GameState> state_ptr);

	/**
	 * Retrieve the top(current) game state as a unique pointer.
	 * @return A std::unique_ptr indicates the top(current) game state.
	 */
	std::unique_ptr<GameState> peekState();

	/**
	 * Process the main game loop.
	 */
	void gameLoop();

	// A sf:RenderWindow object indicates the game window.
	sf::RenderWindow m_game_window;

	private:
	// A stack holds all GameState unique pointers.
	std::stack<std::unique_ptr<GameState>> m_state_stack;
};

#endif // GAME_HPP
