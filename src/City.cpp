#include <numeric>
#include <fstream>
#include "City.hpp"
#include "Tile.hpp"
#include <sstream>
#include <iostream>
#include "Utility.hpp"

double City::getHomelessNum() const
{
	return m_homeless_num;
}

double City::getUnemployedNum() const
{
	return m_unemployment_num;
}

double City::distributeResidents(double& residents_ref, Tile& tile, double rate)
{
	// Maximum amount of people can be moved each time.
	constexpr static unsigned int max_move_num = CITY_MAX_MOVE_NUM;

	// Store input Tile object's max population.
	unsigned int max_tile_population = tile.get_population_limit_per_level() * (tile.m_level + 1);

	// If there are still residents available, move 4 people into related tile zone.
	if (residents_ref > 0)
	{
		// Population to be moved cannot exceed move number limit each time.
		unsigned int population_to_move = std::min(static_cast<unsigned int>(max_tile_population - tile.m_population),
			max_move_num);

		// Population to be moved cannot also exceed available residents' amount.
		if (residents_ref - population_to_move < 0)
			population_to_move = residents_ref;

		// Decrease available residents' amount and increase specified tile's population.
		residents_ref -= population_to_move;
		tile.m_population += population_to_move;
	}
	// Adjust tile population according to input rate(increase if it's birth rate and decrease if it's death rate).
	tile.m_population += tile.m_population * rate;

	// Check if tile's population exceeds its limit.
	if (tile.m_population > tile.get_population_limit_per_level())
	{
		// Restore exceeding part back into available residents group.
		residents_ref += (tile.m_population - tile.get_population_limit_per_level());
		// Set tile's population to its maximum limit.
		tile.m_population = tile.get_population_limit_per_level();
	}

	return tile.m_population;
}

void City::bulldoze(const Tile& tile)
{
	for (size_t i = 0; i < (m_map.get_width() * m_map.get_height()); ++i)
	{
		// Check if this tile is selected.
		if (m_map.get_selected_tile_condition(i) == 1)
		{
			// If this tile is Residential tile, then increase homeless number by tile's population.
			if (m_map.get_tile(i).m_tileType == TileTypeEnum::RESIDENTIAL)
				m_homeless_num += m_map.get_tile(i).m_population;
			// If this tile is Industrial/Commercial tile, then increase unemployment number by tile's population.
			else if ( (m_map.get_tile(i).m_tileType == TileTypeEnum::INDUSTRIAL) ||
			(m_map.get_tile(i).m_tileType == TileTypeEnum::COMMERCIAL))
				m_unemployment_num += m_map.get_tile(i).m_population;

			// Replace the related original Tile with the new one.
			m_map.set_tile(i, tile);
		}
	}
}

void City::shuffleTiles()
{
	// Ensure m_tiles_update_order_vec has same number of elements as game map's tile objects.
	while(m_tiles_update_order_vec.size() < m_map.get_tiles_amount())
		m_tiles_update_order_vec.emplace_back(0);

	// Assigns every element of m_tiles_update_order_vec successive values starting from 1.
	std::iota(m_tiles_update_order_vec.begin(), m_tiles_update_order_vec.end(), 1);
	// Shuffle the m_tiles_update_order_vec in random order.
	std::random_shuffle(m_tiles_update_order_vec.begin(), m_tiles_update_order_vec.end());
}

void City::changeTiles()
{
	// Update each Road Tile object's direction.
	m_map.updateDirection(TileTypeEnum::ROAD);
	// Calculate and store the region amount.
	m_map.calculateConnectedRegionsNum({ TileTypeEnum::ROAD, TileTypeEnum::RESIDENTIAL,
										 TileTypeEnum::COMMERCIAL, TileTypeEnum::INDUSTRIAL }, 0);
}

void City::load(const std::string& city_name_str, std::unordered_map<std::string, Tile>& str_tile_map)
{
	unsigned int city_width{0};
	unsigned int city_height{0};

	// Create the loaded file.
	std::ifstream input_file(city_name_str + CITY_FILE_POSTFIX, std::ios::in);

	std::string each_line_str;
	// Process each line of the input file.
	while (std::getline(input_file, each_line_str))
	{
		std::istringstream input_line_stream(each_line_str);
		std::string each_property_name;
		// Store each property name, delimited by '='.
		if (std::getline(input_line_stream, each_property_name, '='))
		{
			std::string each_property_value;
			// Store each property's value.
			if (std::getline(input_line_stream, each_property_value))
			{
				if(each_property_name == CITY_WIDTH_STR)
					city_width = std::stoi(each_property_value);
				else if(each_property_name == CITY_HEIGHT_STR)
					city_height = std::stoi(each_property_value);
				else if(each_property_name == CITY_DAY_STR)
					m_day = std::stoi(each_property_value);
				else if(each_property_name == CITY_HOMELESS_NUM_STR)
					m_homeless_num = std::stod(each_property_value);
				else if(each_property_name == CITY_UNEMPLOYMENT_NUM_STR)
					m_unemployment_num = std::stod(each_property_value);
				else if(each_property_name == CITY_IN_HOUSE_POPULATION_STR)
					m_in_house_population = std::stod(each_property_value);
				else if(each_property_name == CITY_EMPLOYMENT_NUM_STR)
					m_employment_num = std::stod(each_property_value);
				else if(each_property_name == CITY_BIRTH_RATE_PER_DAY_STR)
					m_birth_rate_per_day = std::stod(each_property_value);
				else if(each_property_name == CITY_DEATH_RATE_PER_DAY_STR)
					m_death_rate_per_day = std::stod(each_property_value);
				else if(each_property_name == CITY_RESIDENTIAL_TAX_RATE_STR)
					m_residential_tax_rate = std::stod(each_property_value);
				else if(each_property_name == CITY_COMMERCIAL_TAX_RATE_STR)
					m_commercial_tax_rate = std::stod(each_property_value);
				else if(each_property_name == CITY_INDUSTRIAL_TAX_RATE_STR)
					m_industrial_tax_rate = std::stod(each_property_value);
				else if(each_property_name == CITY_FUND_STR)
					m_fund = std::stod(each_property_value);
				else if(each_property_name == CITY_LAST_MONTH_EARNINGS_STR)
					m_last_month_earnings = std::stod(each_property_value);
			}
			else
			{
				std::cerr << "Error, no property value related with specified property name " << each_property_name
				<< '\n';
			}
		}
	}

	// Close the file and load the game map.
	input_file.close();
	m_map.load(city_name_str + MAP_FILE_POSTFIX, city_width, city_height, str_tile_map);
}

void City::save(const std::string& city_name_str)
{
	// Create the saved file.
	std::ofstream output_file(city_name_str + CITY_FILE_POSTFIX, std::ios::out);

	// Save each property name and value.
	output_file << CITY_WIDTH_STR << '=' << m_map.get_width() << '\n';
	output_file << CITY_HEIGHT_STR << '=' << m_map.get_height() << '\n';
	output_file << CITY_DAY_STR << '=' << m_day << '\n';
	output_file << CITY_HOMELESS_NUM_STR << '=' << m_homeless_num << '\n';
	output_file << CITY_UNEMPLOYMENT_NUM_STR << '=' << m_unemployment_num << '\n';
	output_file << CITY_IN_HOUSE_POPULATION_STR << '=' << m_in_house_population << '\n';
	output_file << CITY_EMPLOYMENT_NUM_STR << '=' << m_employment_num << '\n';
	output_file << CITY_BIRTH_RATE_PER_DAY_STR << '=' << m_birth_rate_per_day << '\n';
	output_file << CITY_DEATH_RATE_PER_DAY_STR << '=' << m_death_rate_per_day << '\n';
	output_file << CITY_RESIDENTIAL_TAX_RATE_STR << '=' << m_residential_tax_rate << '\n';
	output_file << CITY_COMMERCIAL_TAX_RATE_STR << '=' << m_commercial_tax_rate << '\n';
	output_file << CITY_INDUSTRIAL_TAX_RATE_STR << '=' << m_industrial_tax_rate << '\n';
	output_file << CITY_FUND_STR << '=' << m_fund << '\n';
	output_file << CITY_LAST_MONTH_EARNINGS_STR << '=' << m_last_month_earnings << '\n';

	// Close the file and save the game map.
	output_file.close();
	m_map.save(city_name_str + MAP_FILE_POSTFIX);
}