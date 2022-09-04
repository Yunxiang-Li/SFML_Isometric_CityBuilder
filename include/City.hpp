#pragma once
#ifndef CITY_HPP
#define CITY_HPP

#include <vector>
#include "Map.hpp"

// Forward declarations.
class Tile;

/**
 * Manages the actual gameplay.
 */
class City
{
 public:
	/**
	 * No parameter constructor of City.
	 */
	City() : m_birth_rate_per_day(CITY_BIRTH_RATE_PER_DAY), m_death_rate_per_day(CITY_DEATH_RATE_PER_DAY),
			 m_proportion_can_work(CITY_PROPORTION_CAN_WORK), m_homeless_num(0), m_in_house_population(m_homeless_num),
			 m_unemployment_num(0), m_employment_num(m_unemployment_num),
			 m_residential_tax_rate(CITY_RESIDENTIAL_TAX_RATE), m_commercial_tax_rate(CITY_COMMERCIAL_TAX_RATE),
			 m_industrial_tax_rate(CITY_INDUSTRIAL_TAX_RATE), m_last_month_earnings(0), m_fund(0),
			 m_current_day_elapsed_time(0), m_time_per_day(0.f), m_day(0){};

	/**
	 * Three parameters' constructor of City.
	 * @param city_name_str A reference of const string indicates the city's name.
	 * @param tile_half_width A unsigned integer indicates each Tile object's half width length.
	 * @param str_tile_map A reference of an unordered_map indicates mappings of each type Tile object's name and
	 * related Tile object.
	 */
	City(const std::string& city_name_str, unsigned int tile_half_width,
		std::unordered_map<std::string, Tile>& str_tile_map) : City()
	{
		// Set each tile's half width length and load the tile map.
		m_map.set_tile_half_width(tile_half_width);
		load(city_name_str, str_tile_map);
	};

	/**
	 * Load all city contents(include the game map).
	 * @param city_name_str A reference of const string indicates the city's name.
	 * @param str_tile_map A reference of unordered_map indicates the mappings of each type Tile object's name and
	 * related Tile object.
	 */
	void load(const std::string& city_name_str, std::unordered_map<std::string, Tile>& str_tile_map);

	/**
	 * Save all city contents(include the game map).
	 * @param city_name_str A reference of const string indicates the city's name.
	 */
	void save(const std::string& city_name_str);

	/**
	 * Update the city according to elapsed time. For instance, let people move around, calculate new income,
	 * move goods around tiles and so on.
	 * @param dt A float indicates the elapsed time in seconds since last update call.
	 */
	void update(float dt);

	/**
	 * Replace the mouse selected tile(if valid) with specified tile.
	 * @param tile A reference of const Tile indicates the input specified Tile object. Only residential, commercial or industrial tile
	 */
	void bulldoze(const Tile& tile);

	/**
	 * Generate each Tile object's update order in random inside m_tiles_update_order_vec.
	 */
	void shuffleTiles();

	/**
	 * updates all of the roads to face the correct way. Then creating regions where roads and zones are connected.
	 */
	void changeTiles();

	/**
	 * Retrieve amount of homeless people.
	 * @return A double indicates the number of homeless people.
	 */
	double getHomelessNum() const;

	/**
	 * Retrieve amount of unemployment(employment pool)
	 * @return
	 */
	double getUnemployedNum() const;

 private:
	/* Proportion of residents who die/give birth each day. Estimate for death rate = 1 / (life expectancy * 360).
  	 * Set us 100 times the real world(US) value in order to speed up gameplay. */
	double m_birth_rate_per_day{0};
	double m_death_rate_per_day{0};

	// Proportion of the population that can be employed.
	float m_proportion_can_work{0.f};
	// Amount of residents who do not have homes(homeless people who are not inside residential tiles).
	double m_homeless_num{0};
	// Indicates all residents who have homes.
	double m_in_house_population{0};
	// Amount of people who are not employees(not inside industrial/commercial tiles) for now.
	double m_unemployment_num{0};
	// Indicates the amount of all employees.
	double m_employment_num{0};

	// Store three zones' tax rate.
	double m_residential_tax_rate{0};
	double m_commercial_tax_rate{0};
	double m_industrial_tax_rate{0};

	// Earnings (from tax) from the previous month, will not be added to fund until next month.
	double m_last_month_earnings{0};
	// City's funds for building new tiles.
	double m_fund{0};
	// Indicates the seconds elapsed within current day.
	float m_current_day_elapsed_time{0.f};
	// The amount of seconds each game day should last.
 	float m_time_per_day{0.f};
	// Indicates current day is the ith day of the month. We assume that each month has at most 30 days.
	unsigned int m_day{0};

	// Contains update orders of each Tile object within the game map.
	std::vector<int> m_tiles_update_order_vec;

	// A Map object indicates the game map.
	Map m_map;

	/**
	 * Try to move specified Tile's 4 people into related regions. Then adjust this Tile's population according to
	 * specified rate.
	 * @param residents_ref A reference of double indicates the specified resident group(m_homeless_num or
	 * m_unemployment_num).
	 * @param tile A reference of Tile indicates the specified tile.
	 * @param rate A double indicates the rate. Birth rate if it's positive and death rate if it's negative. Default
	 * value is zero.
	 * @return A double indicates this Tile object's updated population.
	 */
	double distributeResidents(double& residents_ref, Tile& tile, double rate = 0.0);
};

#endif //CITY_HPP