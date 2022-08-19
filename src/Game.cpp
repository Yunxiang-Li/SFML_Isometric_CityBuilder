#include "Game.hpp"
#include "GameState.hpp"
#include "GuiStyle.hpp"

// Store all game string literals.
const std::string GAME_TITLE = "City Builder";
const std::string GAME_BACKGROUND_TEXTURE_NAME = "background";
const std::string FOREST_TILE_TEXTURE_NAME = "forest";
const std::string WATER_TILE_TEXTURE_NAME = "water";
const std::string RESIDENTIAL_TILE_TEXTURE_NAME = "residential";
const std::string COMMERCIAL_TILE_TEXTURE_NAME = "commercial";
const std::string INDUSTRIAL_TILE_TEXTURE_NAME = "industrial";
const std::string ROAD_TILE_TEXTURE_NAME = "road";
const std::string GRASS_TILE_TEXTURE_NAME = "grass";

const std::string GAME_BACKGROUND_TEXTURE_PATH = "../resources/images/background.png";
const std::string FOREST_TILE_TEXTURE_PATH = "../resources/images/forest.png";
const std::string WATER_TILE_TEXTURE_PATH = "../resources/images/water.png";
const std::string RESIDENTIAL_TILE_TEXTURE_PATH = "../resources/images/residential.png";
const std::string COMMERCIAL_TILE_TEXTURE_PATH = "../resources/images/commercial.png";
const std::string INDUSTRIAL_TILE_TEXTURE_PATH = "../resources/images/industrial.png";
const std::string ROAD_TILE_TEXTURE_PATH = "../resources/images/road.png";
const std::string GRASS_TILE_TEXTURE_PATH = "../resources/images/grass.png";

const std::string FONT_PATH = "../resources/fonts/font.ttf";

Game::Game()
{
	// Create a 1280 X 720 game window with the title "City Builder" and set max frame rate to be 60fps.
	m_game_window.create(sf::VideoMode(1280, 720), GAME_TITLE);
	m_game_window.setFramerateLimit(60);

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
	Animation default_anim(0, 0, 1.f);

	/* For grass tile and forest tile, there is only 1 instant animation. Initial height is 1 half tile width,
	 * initial cost is 50, no population allowed, max level is 1.*/
	m_str_tile_map["grass"] = Tile(m_tile_half_width_pixel, 1,
	TextureManager::getInstance()->getTextureRef("grass"),{default_anim},
	TileTypeEnum::GRASS,50, 0, 1);
	// For forest tile, initial height is 1 half tile width, initial cost is 100, max level is 1.
	m_str_tile_map["forest"] = Tile(m_tile_half_width_pixel, 1,
	TextureManager::getInstance()->getTextureRef("forest"),{default_anim},
	TileTypeEnum::FOREST,100, 0, 1);

	/* For water tile, there are 3 animation, each animation consists of 4 frames, each frame takes 0.5 seconds.
	 * Initial height is 1 half tile width, initial cost is 0, no population allowed, max level is 1.*/
	m_str_tile_map["water"] = Tile(m_tile_half_width_pixel, 1,
	TextureManager::getInstance()->getTextureRef("water"),
	{Animation(0, 3, 0.5f), Animation(0, 3,
	0.5f), Animation(0, 3, 0.5f)}, TileTypeEnum::WATER,
	0, 0, 1);

	/* For residential tile, there are 6 instant animation. Initial height is 2 half tile width, initial cost is 300,
	 * initial population is 50, max level is 6.*/
	m_str_tile_map["residential"] = Tile(m_tile_half_width_pixel, 2,
	TextureManager::getInstance()->getTextureRef("residential"),{default_anim, default_anim,
	default_anim, default_anim, default_anim, default_anim},TileTypeEnum::RESIDENTIAL,300,
	50, 6);

	/* For commercial tile, there are 4 instant animation. Initial height is 2 half tile width, initial cost is 300,
 	 * initial population is 50, max level is 4.*/
	m_str_tile_map["commercial"] = Tile(m_tile_half_width_pixel, 2,
	TextureManager::getInstance()->getTextureRef("commercial"),{default_anim, default_anim,
	default_anim, default_anim},TileTypeEnum::COMMERCIAL,300, 50, 4);

	/* For industrial tile, there are 3 instant animation. Initial height is 2 half tile width, initial cost is 300,
     * initial population is 50, max level is 4.*/
	m_str_tile_map["industrial"] = Tile(m_tile_half_width_pixel, 2,
	TextureManager::getInstance()->getTextureRef("industrial"),{default_anim, default_anim,
	default_anim, default_anim},
	TileTypeEnum::INDUSTRIAL,300, 50, 4);

	/* For road tile, there are 11 instant animation. Initial height is 1 half tile width, initial cost is 100,
	 * no population allowed, max level is 1.*/
	m_str_tile_map["road"] = Tile(m_tile_half_width_pixel, 1,
	TextureManager::getInstance()->getTextureRef("road"),{default_anim, default_anim,
	default_anim, default_anim, default_anim, default_anim, default_anim, default_anim, default_anim, default_anim,
	default_anim},TileTypeEnum::ROAD,100, 0, 1);
}

void Game::load_Gui_styles()
{
	// Set up button and text's Gui styles.

	m_str_GuiStyle_ptr_map["button"] = std::make_shared<GuiStyle>(GuiStyle(m_str_font_ptr_map.at("main_font"),
		1.f,sf::Color(0xc6,0xc6,0xc6), sf::Color(0x94,0x94,
			0x94),sf::Color::Black, sf::Color(0x61,0x61,0x61),
			sf::Color(0x94,0x94,0x94),sf::Color::Black));
	m_str_GuiStyle_ptr_map["text"] = std::make_shared<GuiStyle>(GuiStyle(m_str_font_ptr_map.at("main_font"),
		0.f,sf::Color(0x00,0x00,0x00,0x00), sf::Color::Black,
		sf::Color::White,sf::Color(0x00,0x00,0x00,0x00),
		sf::Color::Black,sf::Color::Red));
}

void Game::load_fonts()
{
	sf::Font font;
	font.loadFromFile(FONT_PATH);
	m_str_font_ptr_map["main_font"] = std::make_shared<sf::Font>(font);
}

std::shared_ptr<GuiStyle> Game::getGuiStylePtr(const std::string& Gui_style_name) const
{
	return m_str_GuiStyle_ptr_map.at(Gui_style_name);
}