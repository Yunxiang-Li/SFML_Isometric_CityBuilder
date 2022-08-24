#include "Game.hpp"
#include "GameState.hpp"
#include "GuiStyle.hpp"
#include "Utility.hpp"

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
	m_str_tile_map[GRASS_TILE_TEXTURE_NAME] = Tile(m_tile_half_width_pixel,
		GRASS_HALF_TILE_WIDTH_NUM,TextureManager::getInstance()->
		getTextureRef(GRASS_TILE_TEXTURE_NAME),{default_anim},TileTypeEnum::GRASS,
		GRASS_TILE_COST, GRASS_CURR_LEVEL_POPULATION_LIMIT,GRASS_TILE_MAX_LEVEL);

	// For forest tile, initial height is 1 half tile width, initial cost is 100, max level is 1.
	m_str_tile_map[FOREST_TILE_TEXTURE_NAME] = Tile(m_tile_half_width_pixel,
		FOREST_HALF_TILE_WIDTH_NUM,TextureManager::getInstance()->
		getTextureRef(FOREST_TILE_TEXTURE_NAME),{default_anim},TileTypeEnum::FOREST,
		FOREST_TILE_COST, FOREST_CURR_LEVEL_POPULATION_LIMIT,FOREST_TILE_MAX_LEVEL);

	/* For water tile, there are 3 animation, each animation consists of 4 frames, each frame takes 0.5 seconds.
	 * Initial height is 1 half tile width, initial cost is 0, no population allowed, max level is 1.*/
	m_str_tile_map[WATER_TILE_TEXTURE_NAME] = Tile(m_tile_half_width_pixel,
		WATER_HALF_TILE_WIDTH_NUM,TextureManager::getInstance()->
		getTextureRef(WATER_TILE_TEXTURE_NAME),{Animation(WATER_ANIM_START_FRAME,
			WATER_ANIM_END_FRAME,WATER_ANIM_EACH_FRAME_DURATION),
												Animation(WATER_ANIM_START_FRAME,
			WATER_ANIM_END_FRAME, WATER_ANIM_EACH_FRAME_DURATION),
	 Animation(WATER_ANIM_START_FRAME, WATER_ANIM_END_FRAME,
		 WATER_ANIM_EACH_FRAME_DURATION)}, TileTypeEnum::WATER,
		WATER_TILE_COST, WATER_CURR_LEVEL_POPULATION_LIMIT, WATER_TILE_MAX_LEVEL);

	/* For residential tile, there are 6 instant animation. Initial height is 2 half tile width, initial cost is 300,
	 * initial population is 50, max level is 6.*/
	m_str_tile_map[RESIDENTIAL_TILE_TEXTURE_NAME] = Tile(m_tile_half_width_pixel,
		RESIDENTIAL_HALF_TILE_WIDTH_NUM,TextureManager::getInstance()->
		getTextureRef(RESIDENTIAL_TILE_TEXTURE_NAME),{default_anim, default_anim, default_anim,
	default_anim, default_anim, default_anim},TileTypeEnum::RESIDENTIAL,RESIDENTIAL_TILE_COST,
	RESIDENTIAL_CURR_LEVEL_POPULATION_LIMIT, RESIDENTIAL_TILE_MAX_LEVEL);

	/* For commercial tile, there are 4 instant animation. Initial height is 2 half tile width, initial cost is 300,
 	 * initial population is 50, max level is 4.*/
	m_str_tile_map[COMMERCIAL_TILE_TEXTURE_NAME] = Tile(m_tile_half_width_pixel,
		COMMERCIAL_HALF_TILE_WIDTH_NUM,TextureManager::getInstance()->
		getTextureRef(COMMERCIAL_TILE_TEXTURE_NAME),{default_anim, default_anim,
	default_anim, default_anim},TileTypeEnum::COMMERCIAL,COMMERCIAL_TILE_COST,
	COMMERCIAL_CURR_LEVEL_POPULATION_LIMIT, COMMERCIAL_TILE_MAX_LEVEL);

	/* For industrial tile, there are 3 instant animation. Initial height is 2 half tile width, initial cost is 300,
     * initial population is 50, max level is 4.*/
	m_str_tile_map[INDUSTRIAL_TILE_TEXTURE_NAME] = Tile(m_tile_half_width_pixel,
		INDUSTRIAL_HALF_TILE_WIDTH_NUM,TextureManager::getInstance()->
		getTextureRef(INDUSTRIAL_TILE_TEXTURE_NAME),{default_anim, default_anim,
	default_anim, default_anim},TileTypeEnum::INDUSTRIAL,INDUSTRIAL_TILE_COST,
	INDUSTRIAL_CURR_LEVEL_POPULATION_LIMIT, INDUSTRIAL_TILE_MAX_LEVEL);

	/* For road tile, there are 11 instant animation. Initial height is 1 half tile width, initial cost is 100,
	 * no population allowed, max level is 1.*/
	m_str_tile_map[ROAD_TILE_TEXTURE_NAME] = Tile(m_tile_half_width_pixel,
		ROAD_HALF_TILE_WIDTH_NUM,TextureManager::getInstance()->
		getTextureRef(ROAD_TILE_TEXTURE_NAME),{default_anim, default_anim, default_anim,
											   default_anim, default_anim, default_anim, default_anim,
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