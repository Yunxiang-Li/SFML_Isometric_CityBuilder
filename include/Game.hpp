#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include <stack>
#include "TextureManager.hpp"
#include "Tile.hpp"
#include "Utility.hpp"

// Forward declaration.
class GameState;
class GuiStyle;

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
	void push_state(std::unique_ptr<GameState> state_ptr);

	/**
	 * Pop the top(current) game state out of the state stack.
	 */
	void pop_state();

	/**
	 * Switch from current game state to input one.
	 * @param state_ptr A std::unique_ptr indicates the input game state.
	 */
	void switch_state(std::unique_ptr<GameState> state_ptr);

	/**
	 * Retrieve the top(current) game state as a pointer of GameState object.
	 * @return A pointer of GameState indicates the top(current) game state.
	 */
	GameState* peek_state() const;

	/**
	 * Process the main game loop.
	 */
	void game_loop();

	// A sf:RenderWindow object indicates the game window.
	sf::RenderWindow m_game_window;
	// A sf::Sprite object indicates the background sprite.
	sf::Sprite m_background_sprite;
	// Indicates each tile object's half width in pixel.
	constexpr static int m_tile_half_width_pixel{TILE_HALF_WIDTH};
	// Holds each tile type's string name as key, related tile object as value.
	std::unordered_map<std::string, Tile> m_str_tile_map;

	/**
	 * Find the input specified GuiStyle object.
	 * @param Gui_style_name A reference of const string indicates the specified GuiStyle object's name.
	 * @return A pointer of specified GuiStyle object.
	 */
	std::shared_ptr<GuiStyle> getGuiStylePtr(const std::string& Gui_style_name) const;

 private:
	/**
	 * Load all game required textures.
	 */
	void load_game_textures();

	/**
	 * Create and store all 7 types of tile objects.
	 */
	void load_tiles();

	/**
	 * Load all game required GuiStyle objects.
	 */
	void load_Gui_styles();

	/**
	 * load all game required fonts.
	 */
	void load_fonts();

	// A stack holds all GameState unique pointers.
	std::stack<std::unique_ptr<GameState>> m_state_stack;
	// An unordered map which key is a GuiStyle object's string name, value is the related GuiStyle object's pointer.
	std::unordered_map<std::string, std::shared_ptr<GuiStyle>> m_str_GuiStyle_ptr_map;
	// An unordered map which key is a sf::font object's string name, value is the related font object's pointer.
	std::unordered_map<std::string, std::shared_ptr<sf::Font>> m_str_font_ptr_map;
};

#endif // GAME_HPP
