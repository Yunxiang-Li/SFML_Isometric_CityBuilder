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
	if (tile.m_population > max_tile_population)
	{
		// Restore exceeding part back into available residents group.
		residents_ref += (tile.m_population - max_tile_population);
		// Set tile's population to its maximum limit.
		tile.m_population = max_tile_population;
	}

	return tile.m_population;
}

void City::bulldoze(const Tile& tile)
{
	for (size_t i = 0; i < (m_map_ptr->get_width() * m_map_ptr->get_height()); ++i)
	{
		// Check if this tile is selected.
		if (m_map_ptr->get_selected_tile_condition(i) == 1)
		{
			// If this tile is Residential tile, then increase homeless number by tile's population.
			if (m_map_ptr->get_tile(i).m_tileType == TileTypeEnum::RESIDENTIAL)
				m_homeless_num += m_map_ptr->get_tile(i).m_population;
			// If this tile is Industrial/Commercial tile, then increase unemployment number by tile's population.
			else if ( (m_map_ptr->get_tile(i).m_tileType == TileTypeEnum::INDUSTRIAL) ||
			(m_map_ptr->get_tile(i).m_tileType == TileTypeEnum::COMMERCIAL))
				m_unemployment_num += m_map_ptr->get_tile(i).m_population;

			// Replace the related original Tile with the new one.
			m_map_ptr->set_tile(i, tile);
		}
	}
}

void City::shuffleTiles()
{
	// Ensure m_update_order_related_tile_index_vec has same number of elements as game map's tile objects.
	while(m_update_order_related_tile_index_vec.size() < m_map_ptr->get_tiles_amount())
		m_update_order_related_tile_index_vec.emplace_back(0);

	// Assigns every element of m_update_order_related_tile_index_vec successive values starting from 0.
	std::iota(m_update_order_related_tile_index_vec.begin(), m_update_order_related_tile_index_vec.end(), 0);
	// Shuffle the m_update_order_related_tile_index_vec in random order.
	std::random_shuffle(m_update_order_related_tile_index_vec.begin(), m_update_order_related_tile_index_vec.end());
}

void City::updateTiles()
{
	// Update each Road Tile object's direction.
	m_map_ptr->updateDirection(TileTypeEnum::ROAD);
	// Calculate and store the region amount.
	m_map_ptr->calculateConnectedRegionsNum({ TileTypeEnum::ROAD, TileTypeEnum::RESIDENTIAL,
											 TileTypeEnum::COMMERCIAL, TileTypeEnum::INDUSTRIAL }, 0);
}

void City::load(const std::string& city_name_str, std::unordered_map<std::string, Tile>& str_tile_map)
{
	unsigned int city_width{0};
	unsigned int city_height{0};

	// Create the loaded file.
	std::ifstream input_file(city_name_str, std::ios::in);

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
				else if(each_property_name == CITY_POPULATION_STR)
					m_city_population = std::stod(each_property_value);
				else if(each_property_name == CITY_EMPLOYABLE_STR)
					m_employable = std::stod(each_property_value);
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
				else if(each_property_name == CITY_CURR_MONTH_EARNINGS_STR)
					m_curr_month_earnings = std::stod(each_property_value);
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
	m_map_ptr = std::make_shared<Map>();
	m_map_ptr->load(MAP_BINARY_FILE_PATH, city_width, city_height, str_tile_map);
}

void City::save(const std::string& city_name_str)
{
	// Create the saved file.
	std::ofstream output_file(CITY_NAME_STR, std::ios::out);

	// Save each property name and value.
	output_file << CITY_WIDTH_STR << '=' << m_map_ptr->get_width() << '\n';
	output_file << CITY_HEIGHT_STR << '=' << m_map_ptr->get_height() << '\n';
	output_file << CITY_DAY_STR << '=' << m_day << '\n';
	output_file << CITY_HOMELESS_NUM_STR << '=' << m_homeless_num << '\n';
	output_file << CITY_UNEMPLOYMENT_NUM_STR << '=' << m_unemployment_num << '\n';
	output_file << CITY_POPULATION_STR << '=' << m_city_population << '\n';
	output_file << CITY_EMPLOYABLE_STR << '=' << m_employable << '\n';
	output_file << CITY_BIRTH_RATE_PER_DAY_STR << '=' << m_birth_rate_per_day << '\n';
	output_file << CITY_DEATH_RATE_PER_DAY_STR << '=' << m_death_rate_per_day << '\n';
	output_file << CITY_RESIDENTIAL_TAX_RATE_STR << '=' << m_residential_tax_rate << '\n';
	output_file << CITY_COMMERCIAL_TAX_RATE_STR << '=' << m_commercial_tax_rate << '\n';
	output_file << CITY_INDUSTRIAL_TAX_RATE_STR << '=' << m_industrial_tax_rate << '\n';
	output_file << CITY_FUND_STR << '=' << m_fund << '\n';
	output_file << CITY_CURR_MONTH_EARNINGS_STR << '=' << m_curr_month_earnings << '\n';

	// Close the file and save the game map.
	output_file.close();
	m_map_ptr->save(MAP_BINARY_FILE_PATH);
}

void City::update(const float dt)
{
	// Declare variables for city's total population.
	double total_population(0);

	// Declare revenue for commercial and industrial tiles.
	double commercial_revenue(0);
	double industrial_revenue(0);

	// Update the game time.
	m_current_day_elapsed_time += dt;
	// Do nothing if it's still the same day.
	if (m_current_day_elapsed_time < m_time_per_day)
		return;
	// Process if time reaches next day.
	++m_day;
	// Reset current day's elapsed time.
	m_current_day_elapsed_time = 0;

	// Check if reaches next month(I assume that each month contains exactly 30 days).
	if (m_day % DAYS_NUM_PER_MONTH == 0)
	{
		// Update fund and reset current month earning.
		m_fund += m_curr_month_earnings;
		m_curr_month_earnings = 0;
	}

	// Traverse through each Tile object within the game map to handle residents distribution.
	for (size_t i = 0; i < m_map_ptr->get_tiles_amount(); ++i)
	{
		// Start from Tile object that should be updated first.
		int curr_tile_index = m_update_order_related_tile_index_vec[i];
		Tile& curr_tile = m_map_ptr->get_tile_ref(curr_tile_index);

		/* If current tile is a Residential tile, then distribute residents if possible and add current tile's
		 * population into city's total population. */
		if (curr_tile.m_tileType == TileTypeEnum::RESIDENTIAL)
		{
			this->distributeResidents(m_homeless_num, curr_tile, (m_birth_rate_per_day -
			m_death_rate_per_day));
			total_population += curr_tile.m_population;
		}
		/* If current tile is a Commercial tile, then hire more people according to commercial tax rate. The lower the
		 * tax rate is, the higher possibility people will be hired. */
		else if (curr_tile.m_tileType == TileTypeEnum::COMMERCIAL)
		{
			if ((rand() % CITY_RANDOM_BASE) < (TAX_RATE_FACTOR * (1 - m_commercial_tax_rate)))
				this->distributeResidents(m_unemployment_num, curr_tile, 0.0);
		}
		/* If current tile is an Industrial tile, then try to extract resource from the ground and also hire more people
		 * according to industrial tax rate. The lower the tax rate is, the higher possibility people will be hired. */
		else if (curr_tile.m_tileType == TileTypeEnum::INDUSTRIAL)
		{
			// Check if there is still resource underground and random number check passes.
			if ((m_map_ptr->get_tile_resource(curr_tile_index) > 0) && (rand() % CITY_RANDOM_BASE <
			    m_city_population))
			{
				// Increment current tile's production per worker per day by one.
				curr_tile.set_production_per_day(curr_tile.get_production_per_day() + 1);
				// Decrement current tile's resource by one.
				m_map_ptr->decrement_tile_resource(curr_tile_index);
			}

			// Try to hire more people.
			if ((rand() % CITY_RANDOM_BASE) < (TAX_RATE_FACTOR * (1 - m_industrial_tax_rate)))
				this->distributeResidents(m_unemployment_num, curr_tile, 0.0);
		}
		curr_tile.chanceLevelUp();
	}

	// Second pass to handle industrial tiles' total production update.
	for (size_t i = 0; i < m_map_ptr->get_tiles_amount(); ++i)
	{
		// Start from Tile object that should be updated first.
		int curr_tile_index = m_update_order_related_tile_index_vec[i];
		Tile& curr_tile = m_map_ptr->get_tile_ref(curr_tile_index );

		// Only industrial tiles have features on production.
		if (curr_tile.m_tileType == TileTypeEnum::INDUSTRIAL)
		{
			// Store each Tile object's received production.
			unsigned int received_production(0);
			// Traverse through each Tile object within the game map.
			for (auto& each_tile : m_map_ptr->get_tiles_vec_ref())
			{
				// Try to receive production from industrial tiles within the same region.
				if ((each_tile.m_region_arr[0] == curr_tile.m_region_arr[0]) && each_tile.m_tileType ==
				TileTypeEnum::INDUSTRIAL)
				{
					// Check if each tile still has production.
					if (each_tile.get_production_per_day() > 0)
					{
						// Transfer 1 production.
						++received_production;
						each_tile.set_production_per_day(each_tile.get_production_per_day() - 1);
					}
					/* Current tile's m_level will be used to check if received production is too large(low level tiles
					 * cannot receive too much production). Received production's maximum value cannot exceed m_level
					 * by 1. */
					if (received_production == (curr_tile.m_level + 1))
						break;
				}
			}
			// Update current Tile object's total production.
			curr_tile.m_total_production += (received_production + curr_tile.get_production_per_day()) *
				(curr_tile.m_level + 1);
		}
	}

	// Third pass to handle commercial tiles' production distribution.
	for (size_t i = 0; i < m_map_ptr->get_tiles_amount(); ++i)
	{
		// Start from Tile object that should be updated first.
		int curr_tile_index = m_update_order_related_tile_index_vec[i];
		Tile& curr_tile = m_map_ptr->get_tile_ref(curr_tile_index );

		// Only commercial tiles have features on production distribution.
		if (curr_tile.m_tileType == TileTypeEnum::COMMERCIAL)
		{
			// Store each Tile object's received production.
			unsigned int received_production(0);
			// Store maximum production customers' number.
			unsigned int max_customers_num(0);
			// Traverse through each Tile object within the game map.
			for (auto& each_tile : m_map_ptr->get_tiles_vec_ref())
			{
				/* Process when each_tile is of industrial type, its production is larger than zero and both curr_tile
				 * and each_tile are within the same region. */
				if ((each_tile.m_region_arr[0] == curr_tile.m_region_arr[0]) && (each_tile.m_tileType ==
					TileTypeEnum::INDUSTRIAL) && (each_tile.m_total_production > 0))
				{
					/* Check if each_tile still has production and already received production does not exceed current
					 * Tile object's level plus one. */
					while ((each_tile.m_total_production > 0) && (received_production != (curr_tile.m_level + 1)))
					{
						// Keep transferring 1 production.
						each_tile.set_production_per_day(each_tile.get_production_per_day() - 1);
						++received_production;
						//
						industrial_revenue += INDUSTRIAL_REVENUE_PER_PRODUCTION_FACTOR * (1 - m_industrial_tax_rate);
					}
				}
				/*
				 * Process when each_tile is of residential type and both curr_tile
				 * and each_tile are within the same region.
				 */
				else if ((each_tile.m_region_arr[0] == curr_tile.m_region_arr[0]) && each_tile.m_tileType ==
					TileTypeEnum::RESIDENTIAL)
				{
					// Increment maximum customers' number.
					max_customers_num += each_tile.m_population;
				}

				/* Current tile's m_level will be used to check if received production is too large(low level tiles
				 * cannot receive too much production). Received production's maximum value cannot exceed m_level
				 * by 1. */
				if (received_production == (curr_tile.m_level + 1))
					break;
			}

			// Calculate the overall production for current Tile object.
			curr_tile.m_total_production = (received_production * RESIDENTIAL_REVENUE_PER_PRODUCTION_FACTOR + rand() %
				RESIDENTIAL_RANDOM_BASE) * (1 - m_commercial_tax_rate);

			// Update the overall commercial revenue.
			commercial_revenue += curr_tile.m_total_production * max_customers_num * curr_tile.m_population /
				CITY_RANDOM_BASE;
		}
	}

	// Update homeless people's amount.
	m_homeless_num += m_homeless_num * (m_birth_rate_per_day - m_death_rate_per_day);
	// Include updated homeless people into total population count.
	total_population += m_homeless_num;

	// Count newly appeared workers' number according to additional population of the city.
	float new_workers_num = (total_population - m_city_population) * m_proportion_can_work;

	// Update employment condition.
	m_unemployment_num += new_workers_num;
	m_employable += new_workers_num;

	// Border checks.
	if (m_unemployment_num < 0)
		m_unemployment_num = 0;
	if (m_employable < 0)
		m_employable = 0;

	// Update city's total population.
	m_city_population = total_population;

	// Calculate city's  current month's tax income.
	m_curr_month_earnings = (m_city_population - m_homeless_num) * RESIDENTIAL_EARNING_FACTOR * m_residential_tax_rate;
	m_curr_month_earnings += commercial_revenue * m_commercial_tax_rate;
	m_curr_month_earnings += industrial_revenue * m_industrial_tax_rate;
}

std::shared_ptr<Map> City::get_map_ptr() const
{
	return m_map_ptr;
}

unsigned int City::get_day() const
{
	return m_day;
}

double City::get_fund() const
{
	return m_fund;
}

double City::get_population() const
{
	return m_city_population;
}

double City::get_employable() const
{
	return m_employable;
}

void City::set_fund(double new_fund)
{
	m_fund = new_fund;
}