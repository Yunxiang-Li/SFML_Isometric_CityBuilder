#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include <stack>
#include "TextureManager.hpp"
#include "Tile.hpp"

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
	 * Retrieve the top(current) game state as a pointer of GameState object.
	 * @return A std::unique_ptr indicates the top(current) game state.
	 */
	GameState* peekState() const;

	/**
	 * Process the main game loop.
	 */
	void gameLoop();

	// A sf:RenderWindow object indicates the game window.
	sf::RenderWindow m_game_window;
	// A sf::Sprite object indicates the background sprite.
	sf::Sprite m_background_sprite;
	// Indicates each tile object's half width in pixel.
	constexpr static int m_tile_half_width_pixel{8};
	// Holds each tile type's string name as key, related tile object as value.
	std::unordered_map<std::string, Tile> m_str_tile_map;

 private:
	/**
	 * Load all game required textures.
	 */
	void loadGameTextures();

	/**
	 * Create and store all 7 types of tile objects.
	 */
	void loadTiles();

	// A stack holds all GameState unique pointers.
	std::stack<std::unique_ptr<GameState>> m_state_stack;
};

#endif // GAME_HPP
