#pragma once
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>
#include <SFML/Graphics.hpp>

/**
 * This header stores all in-game data(include number, string, file path and so on).
 */

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
const std::string VOID_TILE_TEXTURE_NAME("void");

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
constexpr unsigned int WATER_TILE_COST(50);
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
constexpr unsigned int RESIDENTIAL_TILE_MAX_LEVEL(5);
constexpr unsigned int COMMERCIAL_TILE_MAX_LEVEL(3);
constexpr unsigned int INDUSTRIAL_TILE_MAX_LEVEL(3);
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

// Define each tile object's production.
constexpr int EACH_TILE_PRODUCTION(255);

// Define all possible directions.
constexpr int TOP_RIGHT_BOTTOM_LEFT_DIR(0);
constexpr int TOP_RIGHT_DIR(TOP_RIGHT_BOTTOM_LEFT_DIR);
constexpr int BOTTOM_LEFT_DIR(TOP_RIGHT_BOTTOM_LEFT_DIR);
constexpr int TOP_LEFT_BOTTOM_RIGHT_DIR(1);
constexpr int TOP_LEFT_DIR(TOP_LEFT_BOTTOM_RIGHT_DIR);
constexpr int BOTTOM_RIGHT_DIR(TOP_LEFT_BOTTOM_RIGHT_DIR);
constexpr int TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT_DIR(2);
constexpr int TOP_RIGHT_BOTTOM_RIGHT_DIR(3);
constexpr int TOP_LEFT_BOTTOM_LEFT_DIR(4);
constexpr int TOP_LEFT_TOP_RIGHT_DIR(5);
constexpr int BOTTOM_LEFT_BOTTOM_RIGHT_DIR(6);
constexpr int TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT_DIR(7);
constexpr int TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT_DIR(8);
constexpr int TOP_LEFT_TOP_RIGHT_BOTTOM_RIGHT_DIR(9);
constexpr int TOP_LEFT_BOTTOM_LEFT_BOTTOM_RIGHT_DIR(10);

// Store game map's dimension.
constexpr unsigned int GAME_MAP_WIDTH(64);
constexpr unsigned int GAME_MAP_HEIGHT(64);

// Store zoom factor according to mouse scroll.
constexpr float DOWNWARD_SCROLL_FACTOR(2.f);
constexpr float UPWARD_SCROLL_FACTOR(0.5f);

// Store integer truncation compensation.
constexpr float INT_TRUNCATION_OFFSET(0.5f);

// Store the dimension of each GuiEntry object's shape.
constexpr unsigned int GUI_ENTRY_WIDTH(192);
constexpr unsigned int GUI_ENTRY_HEIGHT(32);

constexpr unsigned int GUI_ORIGIN_WIDTH(GUI_ENTRY_WIDTH/2);
constexpr unsigned int GUI_ORIGIN_HEIGHT(GUI_ENTRY_HEIGHT/2);

// Store GUI object' s text padding size.
constexpr unsigned int GUI_TEXT_PADDING(4);

// Store button GuiStyle object's name.
const std::string BUTTON_GUI_STYLE_NAME("button");

// Store load game GuiEntry object's name and activated message.
const std::string LOAD_GAME_GUI_ENTRY_NAME("Load Game");
const std::string LOAD_GAME_GUI_ENTRY_MSG("message:load_game");

// Store each Tile object's related GuiEntry object's name and activated message.
const std::string GRASS_GUI_ENTRY_NAME("Grass $" + std::to_string(GRASS_TILE_COST));
const std::string GRASS_GUI_ENTRY_MSG(GRASS_TILE_TEXTURE_NAME);
const std::string WATER_GUI_ENTRY_NAME("Water $" + std::to_string(WATER_TILE_COST));
const std::string WATER_GUI_ENTRY_MSG(WATER_TILE_TEXTURE_NAME);
const std::string FOREST_GUI_ENTRY_NAME("Forest $" + std::to_string(FOREST_TILE_COST));
const std::string FOREST_GUI_ENTRY_MSG(FOREST_TILE_TEXTURE_NAME);
const std::string RESIDENTIAL_GUI_ENTRY_NAME("Residential Zone $" + std::to_string(RESIDENTIAL_TILE_COST));
const std::string RESIDENTIAL_GUI_ENTRY_MSG(RESIDENTIAL_TILE_TEXTURE_NAME);
const std::string COMMERCIAL_GUI_ENTRY_NAME("Commercial Zone $" + std::to_string(COMMERCIAL_TILE_COST));
const std::string COMMERCIAL_GUI_ENTRY_MSG(COMMERCIAL_TILE_TEXTURE_NAME);
const std::string INDUSTRIAL_GUI_ENTRY_NAME("Industrial Zone $" + std::to_string(INDUSTRIAL_TILE_COST));
const std::string INDUSTRIAL_GUI_ENTRY_MSG(INDUSTRIAL_TILE_TEXTURE_NAME);
const std::string ROAD_GUI_ENTRY_NAME("Road $" + std::to_string(ROAD_TILE_COST));
const std::string ROAD_GUI_ENTRY_MSG(ROAD_TILE_TEXTURE_NAME);

// Store information bar Gui object's all Gui entries' names and related activated messages.
const std::string TIME_GUI_ENTRY_STR("time");
const std::string TIME_GUI_ENTRY_MSG("time");
const std::string FUND_GUI_ENTRY_STR("funds");
const std::string FUND_GUI_ENTRY_MSG("funds");
const std::string POPULATION_GUI_ENTRY_STR("population");
const std::string POPULATION_GUI_ENTRY_MSG("population");
const std::string EMPLOYMENT_GUI_ENTRY_STR("employment");
const std::string EMPLOYMENT_GUI_ENTRY_MSG("employment");
const std::string CURR_TILE_GUI_ENTRY_STR("current tile");
const std::string CURR_TILE_GUI_ENTRY_MSG("tile");


// Store GUI object's name.
const std::string GUI_NAME("Load Game");

// Help generate random number.
constexpr unsigned int RAND_BASE_NUM(1e3);
constexpr unsigned int LEVEL_BASE_NUM(1e2);

// Store game map's binary file's path.
const std::string MAP_BINARY_FILE_PATH("../resources/binary/city_map.dat");
// Store the city's name.
const std::string CITY_NAME_STR("../resources/binary/city_cfg.dat");

// Store the Tile object's selected color.
const sf::Color TILE_SELECTED_COLOR(sf::Color (0x7d, 0x7d, 0x7d));

// Store flags about whether a tile is selected or valid.
constexpr unsigned int TILE_NOT_SELECTED_FLAG(0);
constexpr unsigned int TILE_SELECTED_FLAG(1);
constexpr unsigned int TILE_INVALID_FLAG(2);

// Define each tile object's half width in pixels.
constexpr unsigned int TILE_HALF_WIDTH = 8;

// Define the maximum amount of people can be moved into related Tile each time.
constexpr unsigned int CITY_MAX_MOVE_NUM(4);

// Store city's birth and death rate per day.
constexpr double CITY_BIRTH_RATE_PER_DAY(0.003);
constexpr double CITY_DEATH_RATE_PER_DAY(0.0025);

// Store city's proportion of population that can work.
constexpr float CITY_PROPORTION_CAN_WORK(0.5f);

// Store city's 3 tax rates.
constexpr double CITY_RESIDENTIAL_TAX_RATE(0.05);
constexpr double CITY_COMMERCIAL_TAX_RATE(0.05);
constexpr double CITY_INDUSTRIAL_TAX_RATE(0.05);

// Store all City class's property names' strings.
const std::string CITY_WIDTH_STR("city_width");
const std::string CITY_HEIGHT_STR("city_height");
const std::string CITY_DAY_STR("m_day");
const std::string CITY_HOMELESS_NUM_STR("m_homeless_num");
const std::string CITY_UNEMPLOYMENT_NUM_STR("m_unemployment_num");
const std::string CITY_POPULATION_STR("m_city_population");
const std::string CITY_EMPLOYABLE_STR("m_employable_num");
const std::string CITY_BIRTH_RATE_PER_DAY_STR("m_birth_rate_per_day");
const std::string CITY_DEATH_RATE_PER_DAY_STR("m_death_rate_per_day");
const std::string CITY_RESIDENTIAL_TAX_RATE_STR("m_residential_tax_rate");
const std::string CITY_COMMERCIAL_TAX_RATE_STR("m_commercial_tax_rate");
const std::string CITY_INDUSTRIAL_TAX_RATE_STR("m_industrial_tax_rate");
const std::string CITY_FUND_STR("m_fund");
const std::string CITY_CURR_MONTH_EARNINGS_STR("m_curr_month_earnings");

// Store each month's days' amount(assume each month has exactly 30 days).
constexpr unsigned int DAYS_NUM_PER_MONTH(30);

// Store a base number for random number generation inside City::update function.
constexpr unsigned int CITY_RANDOM_BASE(100);
// Store a factor for tax rate comparison inside City::update function.
constexpr unsigned int TAX_RATE_FACTOR(15);

// Store a factor for per production's industrial revenue.
constexpr unsigned int INDUSTRIAL_REVENUE_PER_PRODUCTION_FACTOR(100);

// Store a factor for per production's residential revenue.
constexpr unsigned int RESIDENTIAL_REVENUE_PER_PRODUCTION_FACTOR(100);

// Store a base number for random number generation for residential tile's revenue calculation.
constexpr unsigned int RESIDENTIAL_RANDOM_BASE(20);

// Store the factor used to count city's current month's residential earnings.
constexpr unsigned int RESIDENTIAL_EARNING_FACTOR(15);

// Store the name of menu Gui object.
const std::string MENU_GUI_STR("rightClickMenu");

// Store the dimension of menu Gui object's entry shape.
constexpr unsigned int MENU_GUI_ENTRY_SHAPE_WIDTH(196);
constexpr unsigned int MENU_GUI_ENTRY_SHAPE_HEIGHT(16);

// Store menu Gui object's text padding.
constexpr unsigned int MENU_GUI_TEXT_PADDING(2);

// Store the name of select tile cost text Gui object.
const std::string SELECT_TILE_COST_TEXT_GUI_STR("selectCostText");

// Store the dimension of select tile cost text Gui object's entry shape.
constexpr unsigned int SELECT_TILE_COST_TEXT_GUI_ENTRY_SHAPE_WIDTH(196);
constexpr unsigned int SELECT_TILE_COST_TEXT_GUI_ENTRY_SHAPE_HEIGHT(16);

// Store the display offset of select tile cost text Gui object.
constexpr unsigned int SELECT_TILE_COST_TEXT_GUI_X_OFFSET(16);
constexpr unsigned int SELECT_TILE_COST_TEXT_GUI_Y_OFFSET(-16);

// Store the name of information bar Gui object.
const std::string INFO_BAR_GUI_STR("infoBar");

// Store the dimension of information bar Gui object's entry shape.
constexpr unsigned int INFO_BAR_GUI_ENTRY_SHAPE_WIDTH(GAME_WINDOW_WIDTH/5);
constexpr unsigned int INFO_BAR_GUI_ENTRY_SHAPE_HEIGHT(16);

// Store Info bar Gui object's text padding.
constexpr unsigned int INFO_BAR_GUI_TEXT_PADDING(2);

#endif //UTILITY_HPP

