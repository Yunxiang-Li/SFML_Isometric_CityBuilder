#include "Game.hpp"
#include "GameState.hpp"
#include "GuiStyle.hpp"

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

Game::Game()
{
	// Create a 1280 X 720 game window with the title "City Builder" and set max frame rate to be 60fps.
	m_game_window.create(sf::VideoMode(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT),
		GAME_TITLE);
	m_game_window.setFramerateLimit(FPS);

	// Load all game textures.
	this->load_game_textures();
	// Create and store all 7 types of tiles.
	this->load_tiles();
	// Set up game background's sprite object.
	m_background_sprite.setTexture(TextureManager::getInstance()->
	getTextureRef(GAME_BACKGROUND_TEXTURE_NAME));
	// Load font and Gui styles.
	this->load_fonts();
	this->load_Gui_styles();
}

Game::~Game()
{
	while (!(m_state_stack.empty()))
		this->pop_state();
}

void Game::push_state(std::unique_ptr<GameState> state_ptr)
{
	m_state_stack.emplace(std::move(state_ptr));
}

void Game::pop_state()
{
	m_state_stack.pop();
}

void Game::switch_state(std::unique_ptr<GameState> state_ptr)
{
	if (!(m_state_stack.empty()))
		this->pop_state();
	this->push_state(std::move(state_ptr));
}

GameState* Game::peek_state() const
{
	if (m_state_stack.empty())
		return nullptr;
	return m_state_stack.top().get();
}

void Game::game_loop()
{
	// Create a sf::Clock object to help track the time.
	sf::Clock game_clock;
	while (m_game_window.isOpen())
	{
		// Get the elapsed time since last frame and convert it to seconds inside a float value.
		sf::Time elapsed_time(game_clock.restart());
		float delta_time = elapsed_time.asSeconds();
		// If the game is not in any states, then continue to next frame.
		if (this->peek_state() == nullptr)
			continue;
		// Else handle player's input and chanceLevelUp current frame's behavior within current game state.
		this->peek_state()->inputProcess();
		this->peek_state()->update(delta_time);
		// Clear the game window with black color.
		m_game_window.clear(sf::Color::Black);
		// Render and display the current frame content.
		this->peek_state()->render(delta_time);
		m_game_window.display();
	}
}

void Game::load_game_textures()
{
	TextureManager::getInstance()->loadTexture(GAME_BACKGROUND_TEXTURE_NAME,
		GAME_BACKGROUND_TEXTURE_PATH);
	TextureManager::getInstance()->loadTexture(GRASS_TILE_TEXTURE_NAME, GRASS_TILE_TEXTURE_PATH);
	TextureManager::getInstance()->loadTexture(ROAD_TILE_TEXTURE_NAME, ROAD_TILE_TEXTURE_PATH);
	TextureManager::getInstance()->loadTexture(WATER_TILE_TEXTURE_NAME, WATER_TILE_TEXTURE_PATH);
	TextureManager::getInstance()->loadTexture(RESIDENTIAL_TILE_TEXTURE_NAME,
		RESIDENTIAL_TILE_TEXTURE_PATH);
	TextureManager::getInstance()->loadTexture(FOREST_TILE_TEXTURE_NAME, FOREST_TILE_TEXTURE_PATH);
	TextureManager::getInstance()->loadTexture(COMMERCIAL_TILE_TEXTURE_NAME,
		COMMERCIAL_TILE_TEXTURE_PATH);
	TextureManager::getInstance()->loadTexture(INDUSTRIAL_TILE_TEXTURE_NAME,
		INDUSTRIAL_TILE_TEXTURE_PATH);
}

void Game::load_tiles()
{
	// Create a local Animation object to help load all 7 types of tiles.
	Animation default_anim(GENERAL_ANIMATION_START_FRAME, GENERAL_ANIMATION_END_FRAME,
		GENERAL_ANIMATION_EACH_FRAME_DURATION);

	/* For grass tile and forest tile, there is only 1 instant animation. Initial height is 1 half tile width,
	 * initial cost is 50, no population allowed, max level is 1.*/
	m_str_tile_map["grass"] = Tile(m_tile_half_width_pixel, GRASS_HALF_TILE_WIDTH_NUM,
	TextureManager::getInstance()->getTextureRef(GRASS_TILE_TEXTURE_NAME),{default_anim},
	TileTypeEnum::GRASS,GRASS_TILE_COST, GRASS_CURR_LEVEL_POPULATION_LIMIT,
	GRASS_TILE_MAX_LEVEL);

	// For forest tile, initial height is 1 half tile width, initial cost is 100, max level is 1.
	m_str_tile_map["forest"] = Tile(m_tile_half_width_pixel, FOREST_HALF_TILE_WIDTH_NUM,
	TextureManager::getInstance()->getTextureRef(FOREST_TILE_TEXTURE_NAME),{default_anim},
	TileTypeEnum::FOREST,FOREST_TILE_COST, FOREST_CURR_LEVEL_POPULATION_LIMIT,
	FOREST_TILE_MAX_LEVEL);

	/* For water tile, there are 3 animation, each animation consists of 4 frames, each frame takes 0.5 seconds.
	 * Initial height is 1 half tile width, initial cost is 0, no population allowed, max level is 1.*/
	m_str_tile_map["water"] = Tile(m_tile_half_width_pixel, WATER_HALF_TILE_WIDTH_NUM,
	TextureManager::getInstance()->getTextureRef(WATER_TILE_TEXTURE_NAME),
	{Animation(WATER_ANIM_START_FRAME, WATER_ANIM_END_FRAME,
		WATER_ANIM_EACH_FRAME_DURATION),Animation(WATER_ANIM_START_FRAME,
			WATER_ANIM_END_FRAME, WATER_ANIM_EACH_FRAME_DURATION),
	 Animation(WATER_ANIM_START_FRAME, WATER_ANIM_END_FRAME,
		 WATER_ANIM_EACH_FRAME_DURATION)}, TileTypeEnum::WATER,
		WATER_TILE_COST, WATER_CURR_LEVEL_POPULATION_LIMIT, WATER_TILE_MAX_LEVEL);

	/* For residential tile, there are 6 instant animation. Initial height is 2 half tile width, initial cost is 300,
	 * initial population is 50, max level is 6.*/
	m_str_tile_map["residential"] = Tile(m_tile_half_width_pixel,
		RESIDENTIAL_HALF_TILE_WIDTH_NUM,TextureManager::getInstance()->
		getTextureRef(RESIDENTIAL_TILE_TEXTURE_NAME),{default_anim, default_anim, default_anim,
	default_anim, default_anim, default_anim},TileTypeEnum::RESIDENTIAL,RESIDENTIAL_TILE_COST,
	RESIDENTIAL_CURR_LEVEL_POPULATION_LIMIT, RESIDENTIAL_TILE_MAX_LEVEL);

	/* For commercial tile, there are 4 instant animation. Initial height is 2 half tile width, initial cost is 300,
 	 * initial population is 50, max level is 4.*/
	m_str_tile_map["commercial"] = Tile(m_tile_half_width_pixel,
		COMMERCIAL_HALF_TILE_WIDTH_NUM,TextureManager::getInstance()->
		getTextureRef(COMMERCIAL_TILE_TEXTURE_NAME),{default_anim, default_anim,
	default_anim, default_anim},TileTypeEnum::COMMERCIAL,COMMERCIAL_TILE_COST,
	COMMERCIAL_CURR_LEVEL_POPULATION_LIMIT, COMMERCIAL_TILE_MAX_LEVEL);

	/* For industrial tile, there are 3 instant animation. Initial height is 2 half tile width, initial cost is 300,
     * initial population is 50, max level is 4.*/
	m_str_tile_map["industrial"] = Tile(m_tile_half_width_pixel,
		INDUSTRIAL_HALF_TILE_WIDTH_NUM,TextureManager::getInstance()->
		getTextureRef(INDUSTRIAL_TILE_TEXTURE_NAME),{default_anim, default_anim,
	default_anim, default_anim},TileTypeEnum::INDUSTRIAL,INDUSTRIAL_TILE_COST,
	INDUSTRIAL_CURR_LEVEL_POPULATION_LIMIT, INDUSTRIAL_TILE_MAX_LEVEL);

	/* For road tile, there are 11 instant animation. Initial height is 1 half tile width, initial cost is 100,
	 * no population allowed, max level is 1.*/
	m_str_tile_map["road"] = Tile(m_tile_half_width_pixel, ROAD_HALF_TILE_WIDTH_NUM,
	TextureManager::getInstance()->getTextureRef(ROAD_TILE_TEXTURE_NAME),
	{default_anim, default_anim, default_anim, default_anim, default_anim, default_anim, default_anim,
	 default_anim, default_anim, default_anim, default_anim},TileTypeEnum::ROAD,ROAD_TILE_COST,
	 ROAD_CURR_LEVEL_POPULATION_LIMIT, ROAD_TILE_MAX_LEVEL);
}

void Game::load_Gui_styles()
{
	// Set up button and text's Gui styles.

	m_str_GuiStyle_ptr_map[BUTTON_NAME] = std::make_shared<GuiStyle>(GuiStyle(m_str_font_ptr_map.at(FONT_NAME),
		BUTTON_OUTLINE_SIZE,BUTTON_BACKGROUND_COLOR, BUTTON_OUTLINE_COLOR,
		sf::Color::Black, BUTTON_BACKGROUND_HIGHLIGHT_COLOR,
			BUTTON_OUTLINE_HIGHLIGHT_COLOR,sf::Color::Black));
	m_str_GuiStyle_ptr_map[TEXT_NAME] = std::make_shared<GuiStyle>(GuiStyle(m_str_font_ptr_map.at(FONT_NAME),
		0.f,TEXT_BACKGROUND_COLOR, sf::Color::Black,
		sf::Color::White,TEXT_BACKGROUND_HIGHLIGHT_COLOR,
		sf::Color::Black,sf::Color::Red));
}

void Game::load_fonts()
{
	sf::Font font;
	font.loadFromFile(FONT_PATH);
	m_str_font_ptr_map[FONT_NAME] = std::make_shared<sf::Font>(font);
}

std::shared_ptr<GuiStyle> Game::getGuiStylePtr(const std::string& Gui_style_name) const
{
	return m_str_GuiStyle_ptr_map.at(Gui_style_name);
}