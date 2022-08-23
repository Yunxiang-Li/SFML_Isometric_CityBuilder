#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include <stack>
#include "TextureManager.hpp"
#include "Tile.hpp"

// Forward declaration.
class GameState;
class GuiStyle;

// Store all game string literals.

// Game title and background texture names.
const std::string GAME_TITLE("City Builder");
const std::string GAME_BACKGROUND_TEXTURE_NAME("background");

// Store all seven types of tiles' names.
const std::string FOREST_TILE_TEXTURE_NAME("forest");
const std::string WATER_TILE_TEXTURE_NAME("water");
const std::string RESIDENTIAL_TILE_TEXTURE_NAME("residential");
const std::string COMMERCIAL_TILE_TEXTURE_NAME("commercial");
const std::string INDUSTRIAL_TILE_TEXTURE_NAME("industrial");
const std::string ROAD_TILE_TEXTURE_NAME("road");
const std::string GRASS_TILE_TEXTURE_NAME("grass");

// Store the image files' path of seven types of tiles and the background.
const std::string GAME_BACKGROUND_TEXTURE_PATH("../resources/images/background.png");
const std::string FOREST_TILE_TEXTURE_PATH("../resources/images/forest.png");
const std::string WATER_TILE_TEXTURE_PATH("../resources/images/water.png");
const std::string RESIDENTIAL_TILE_TEXTURE_PATH("../resources/images/residential.png");
const std::string COMMERCIAL_TILE_TEXTURE_PATH("../resources/images/commercial.png");
const std::string INDUSTRIAL_TILE_TEXTURE_PATH("../resources/images/industrial.png");
const std::string ROAD_TILE_TEXTURE_PATH("../resources/images/road.png");
const std::string GRASS_TILE_TEXTURE_PATH("../resources/images/grass.png");

// Store the font file's path and name.
const std::string FONT_PATH("../resources/fonts/font.ttf");
const std::string FONT_NAME("main_font");

// Store game window's original width and height.
constexpr unsigned int GAME_WINDOW_WIDTH(1280);
constexpr unsigned int GAME_WINDOW_HEIGHT(720);

// Store game's frame rate limit.
constexpr unsigned int FPS(60);

// Store general Animation object's start frame, end frame and each frame's duration.
constexpr unsigned int GENERAL_ANIMATION_START_FRAME(0);
constexpr unsigned int GENERAL_ANIMATION_END_FRAME(0);
constexpr float GENERAL_ANIMATION_EACH_FRAME_DURATION(1.f);

// Store all seven types of tiles' half tile width numbers as related tile height.
constexpr unsigned int GRASS_HALF_TILE_WIDTH_NUM(1);
constexpr unsigned int FOREST_HALF_TILE_WIDTH_NUM(1);
constexpr unsigned int WATER_HALF_TILE_WIDTH_NUM(1);
constexpr unsigned int RESIDENTIAL_HALF_TILE_WIDTH_NUM(2);
constexpr unsigned int COMMERCIAL_HALF_TILE_WIDTH_NUM(2);
constexpr unsigned int INDUSTRIAL_HALF_TILE_WIDTH_NUM(2);
constexpr unsigned int ROAD_HALF_TILE_WIDTH_NUM(1);

// Store all seven types of tiles' cost.
constexpr unsigned int GRASS_TILE_COST(50);
constexpr unsigned int FOREST_TILE_COST(100);
constexpr unsigned int WATER_TILE_COST(0);
constexpr unsigned int RESIDENTIAL_TILE_COST(300);
constexpr unsigned int COMMERCIAL_TILE_COST(300);
constexpr unsigned int INDUSTRIAL_TILE_COST(300);
constexpr unsigned int ROAD_TILE_COST(100);

// Store all seven types of tiles' current level population limit.
constexpr unsigned int GRASS_CURR_LEVEL_POPULATION_LIMIT(0);
constexpr unsigned int FOREST_CURR_LEVEL_POPULATION_LIMIT(0);
constexpr unsigned int WATER_CURR_LEVEL_POPULATION_LIMIT(0);
constexpr unsigned int RESIDENTIAL_CURR_LEVEL_POPULATION_LIMIT(50);
constexpr unsigned int COMMERCIAL_CURR_LEVEL_POPULATION_LIMIT(50);
constexpr unsigned int INDUSTRIAL_CURR_LEVEL_POPULATION_LIMIT(50);
constexpr unsigned int ROAD_CURR_LEVEL_POPULATION_LIMIT(0);

// Store all seven types of tiles' max level.
constexpr unsigned int GRASS_TILE_MAX_LEVEL(1);
constexpr unsigned int FOREST_TILE_MAX_LEVEL(1);
constexpr unsigned int WATER_TILE_MAX_LEVEL(1);
constexpr unsigned int RESIDENTIAL_TILE_MAX_LEVEL(6);
constexpr unsigned int COMMERCIAL_TILE_MAX_LEVEL(4);
constexpr unsigned int INDUSTRIAL_TILE_MAX_LEVEL(4);
constexpr unsigned int ROAD_TILE_MAX_LEVEL(1);

// Store water tile's animation properties.
constexpr unsigned int WATER_ANIM_START_FRAME(0);
constexpr unsigned int WATER_ANIM_END_FRAME(3);
constexpr float WATER_ANIM_EACH_FRAME_DURATION(0.5f);

// Store button GuiStyle object's properties.
const std::string BUTTON_NAME("button");
constexpr float BUTTON_OUTLINE_SIZE(1.f);
const sf::Color BUTTON_BACKGROUND_COLOR(sf::Color(0xc6,0xc6,0xc6));
const sf::Color BUTTON_OUTLINE_COLOR(sf::Color(0x94,0x94,0x94));
const sf::Color BUTTON_BACKGROUND_HIGHLIGHT_COLOR(sf::Color(0x61,0x61,0x61));
const sf::Color BUTTON_OUTLINE_HIGHLIGHT_COLOR(sf::Color(0x94,0x94,0x94));

// Store text GuiStyle object's properties.
const std::string TEXT_NAME("text");
constexpr float TEXT_OUTLINE_SIZE(0.f);
const sf::Color TEXT_BACKGROUND_COLOR(sf::Color(0x00,0x00,0x00,0x00));
const sf::Color TEXT_BACKGROUND_HIGHLIGHT_COLOR(sf::Color(0x00,0x00,0x00,0x00));

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
	 * @return A std::unique_ptr indicates the top(current) game state.
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
	constexpr static int m_tile_half_width_pixel{8};
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
