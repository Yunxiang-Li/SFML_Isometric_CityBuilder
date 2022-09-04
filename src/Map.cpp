#include <fstream>
#include "Map.hpp"
#include "Utility.hpp"

Map::Map(const std::string& file_name, unsigned int width, unsigned int height,
	std::unordered_map<std::string, Tile>& str_tile_map)
{
	this->load(file_name, width, height, str_tile_map);
}

void Map::load(const std::string& file_name, unsigned int width, unsigned int height,
	std::unordered_map<std::string, Tile>& str_tile_map)
{
	// Open the store eternal file in binary form to read.
	std::ifstream input_file(file_name, std::ios::binary);

	// Store the dimension of the map.
	m_width = width;
	m_height = height;

	for (int pos = 0; pos < (m_width * m_height); ++pos)
	{
		// Set each tile object's initial production to be 255.
		m_resource_vec.emplace_back(EACH_TILE_PRODUCTION);
		// Initialize each selected tile's condition to zero(not selected).
		m_selected_tiles_condition_vec.emplace_back(TILE_NOT_SELECTED_FLAG);

		// Read and store each tile object's tile type.
		TileTypeEnum tileType;
		input_file.read(reinterpret_cast<char*>(&tileType), sizeof(TileTypeEnum));

		// Set up each tile object's texture according to its tile type.
		switch (tileType)
		{
		case TileTypeEnum::VOID:
		case TileTypeEnum::FOREST:
			m_tiles_vec.emplace_back(str_tile_map.at(FOREST_TILE_TEXTURE_NAME));
			break;
		case TileTypeEnum::WATER:
			m_tiles_vec.emplace_back(str_tile_map.at(WATER_TILE_TEXTURE_NAME));
			break;
		case TileTypeEnum::RESIDENTIAL:
			m_tiles_vec.emplace_back(str_tile_map.at(RESIDENTIAL_TILE_TEXTURE_NAME));
			break;
		case TileTypeEnum::COMMERCIAL:
			m_tiles_vec.emplace_back(str_tile_map.at(COMMERCIAL_TILE_TEXTURE_NAME));
			break;
		case TileTypeEnum::INDUSTRIAL:
			m_tiles_vec.emplace_back(str_tile_map.at(INDUSTRIAL_TILE_TEXTURE_NAME));
			break;
		case TileTypeEnum::ROAD:
			m_tiles_vec.emplace_back(str_tile_map.at(ROAD_TILE_TEXTURE_NAME));
			break;
		case TileTypeEnum::GRASS:
			m_tiles_vec.emplace_back(str_tile_map.at(RESIDENTIAL_TILE_TEXTURE_NAME));
			break;
		default:
			break;
		}

		// Set up each tile object's current level, region id array, current population and total production.
		Tile& curr_tile_ref = m_tiles_vec.back();
		input_file.read(reinterpret_cast<char*>(&(curr_tile_ref.m_level)), sizeof(unsigned int));
		input_file.read(reinterpret_cast<char*>(&(curr_tile_ref.m_region_arr)), sizeof(unsigned int) * 1);
		input_file.read(reinterpret_cast<char*>(&(curr_tile_ref.m_population)), sizeof(double));
		input_file.read(reinterpret_cast<char*>(&(curr_tile_ref.m_total_production)), sizeof(float));

		input_file.close();
	}
}

void Map::save(const std::string& file_name)
{
	// Open the eternal file in binary form to write.
	std::ofstream output_file(file_name, std::ios::binary);

	// Store each tile object's tile type, current level, region id array, current population and total production.
	for (auto& each_tile: m_tiles_vec)
	{
		output_file.write(reinterpret_cast<char*>(&(each_tile.m_tileType)), sizeof(TileTypeEnum));
		output_file.write(reinterpret_cast<char*>(&(each_tile.m_level)), sizeof(unsigned int));
		output_file.write(reinterpret_cast<char*>(&(each_tile.m_region_arr)), sizeof(unsigned int) * 3);
		output_file.write(reinterpret_cast<char*>(&(each_tile.m_population)), sizeof(double));
		output_file.write(reinterpret_cast<char*>(&(each_tile.m_total_production)), sizeof(float));
	}

	output_file.close();
}

void Map::render(sf::RenderWindow& renderWindow, float dt)
{

	// Iterate horizontally and then vertically to render each isometric tile.
	for (int y = 0; y < m_height; ++y)
		for (int x = 0; x < m_width; ++x)
		{
			// Get each isometric tile's position(tile coordinate to screen coordinate).
			sf::Vector2f pos(m_tile_half_width * (x - y) + m_width * m_tile_half_width,
				m_tile_half_width * (x + y) * 0.5);
			// Bind each tile's position with related element inside m_tiles_vec.
			m_tiles_vec[y * m_width + x].m_sprite.setPosition(pos);

			// If current tile is selected, set its color to be dark, white color otherwise.
			if (m_selected_tiles_condition_vec.at(y * m_width + x) == TILE_SELECTED_FLAG)
				m_tiles_vec[y * m_width + x].m_sprite.setColor(TILE_SELECTED_COLOR);
			else
				m_tiles_vec[y * m_width + x].m_sprite.setColor(sf::Color::White);

			// Draw each tile object.
			m_tiles_vec[y * m_width + x].render(renderWindow, dt);
		}
}

void Map::calculateConnectedRegionsNum(const std::vector<TileTypeEnum>& region_tiles_type_vec, unsigned int region_index)
{
	// Indicates the number of input region type, starts from one.
	unsigned int region_num{1};

	/* Reset each tile object's region array's input region_type's related value to zero( which means not inside related
	 * region for now). */
	for (auto& tile : m_tiles_vec)
		tile.m_region_arr[region_index] = 0;

	// Iterate through all tile objects.
	for (int y = 0; y < m_height; ++y)
		for (int x = 0; x < m_width; ++x)
		{
			// Check if current tile object's tile type is in the white list(can make up a region).
			bool is_tile_type_match{false};
			for (const auto& tileType : region_tiles_type_vec)
				if (tileType ==  m_tiles_vec[y * m_width + x].m_tileType)
				{
					is_tile_type_match = true;
					break;
				}
			/* If the current Tile object has not yet been assigned a region_num and the tile type matches,
			 * call DFS function. This means each DFS call will create a new region. */
			if (m_tiles_vec[y * m_width + x].m_region_arr[region_index] == 0 && is_tile_type_match)
				DFS(region_tiles_type_vec, sf::Vector2i{ x, y}, region_num++,
					region_index);
		}

	// Store the input region type's region number.
	m_region_num_arr[region_index] = region_num;
}

void Map::updateDirection(TileTypeEnum tileType)
{
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			// Retrieve current position.
			int pos = y * m_width + x;

			// Check if current tile object's type matches the input.
			if (m_tiles_vec[pos].m_tileType != tileType)
				continue;

			/* Create a 2D 3X3 array(as a mimic of 9 isometric tiles' structure) to help the check.
			 * Check current tile object's 8 adjacent tiles. Set related array element to true if the adjacent Tile
			 * object is of the same type as the current tile object and false otherwise. Current tile(y, x) is also
			 * the center element (1, 1) of the adjacent tile array. */

			/* 					(0, 0)
			 * 			   (0, 1)    (1, 0)
			 * 			 (0, 2) (1, 1) (2, 0)
			 * 			   (1, 2)    (2, 1)
			 * 			        (2, 2)
			 * The isometric sample represented by the adjacent array.
			 * */

			std::array<std::array<int, 3>, 3> adjacent_tiles_arr{{{ 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }}};

			// Match with top tile object.
			if (x > 0 && y > 0)
				adjacent_tiles_arr[0][0] = (m_tiles_vec[(y - 1) * m_width + (x - 1)].m_tileType == tileType);
			// Match with top left tile object.
			if (y > 0)
				adjacent_tiles_arr[0][1] = (m_tiles_vec[(y - 1) * m_width + x].m_tileType == tileType);
			// Match with left tile object.
			if (x < m_width - 1 && y > 0)
				adjacent_tiles_arr[0][2] = (m_tiles_vec[(y - 1) * m_width + (x + 1)].m_tileType == tileType);
			// Match with top right tile object.
			if (x > 0)
				adjacent_tiles_arr[1][0] = (m_tiles_vec[y * m_width + (x - 1)].m_tileType == tileType);
			// Match with bottom left tile object.
			if (x < m_width - 1)
				adjacent_tiles_arr[1][2] = (m_tiles_vec[y * m_width + (x + 1)].m_tileType == tileType);
			// Match with right tile object.
			if (x > 0 && y < m_height - 1)
				adjacent_tiles_arr[2][0] = (m_tiles_vec[(y + 1) * m_width + (x - 1)].m_tileType == tileType);
			// Match with bottom right tile object.
			if (y < m_height - 1)
				adjacent_tiles_arr[2][1] = (m_tiles_vec[(y + 1) * m_width + x].m_tileType == tileType);
			// Match with bottom tile object.
			if (x < m_width - 1 && y < m_height - 1)
				adjacent_tiles_arr[2][2] = (m_tiles_vec[(y + 1) * m_width + (x + 1)].m_tileType == tileType);

			// Change the tile level depending on related adjacent elements' values.
			if (adjacent_tiles_arr[1][0] && adjacent_tiles_arr[1][2] && adjacent_tiles_arr[0][1] &&
				adjacent_tiles_arr[2][1])
				m_tiles_vec[pos].set_level(TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT_DIR);
			else if (adjacent_tiles_arr[1][0] && adjacent_tiles_arr[1][2] && adjacent_tiles_arr[0][1])
				m_tiles_vec[pos].set_level(TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT_DIR);
			else if (adjacent_tiles_arr[1][0] && adjacent_tiles_arr[1][2] && adjacent_tiles_arr[2][1])
				m_tiles_vec[pos].set_level(TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT_DIR);
			else if (adjacent_tiles_arr[0][1] && adjacent_tiles_arr[2][1] && adjacent_tiles_arr[1][0])
				m_tiles_vec[pos].set_level(TOP_LEFT_TOP_RIGHT_BOTTOM_RIGHT_DIR);
			else if (adjacent_tiles_arr[0][1] && adjacent_tiles_arr[2][1] && adjacent_tiles_arr[1][2])
				m_tiles_vec[pos].set_level(TOP_LEFT_BOTTOM_LEFT_BOTTOM_RIGHT_DIR);
			else if (adjacent_tiles_arr[1][0] && adjacent_tiles_arr[1][2])
				m_tiles_vec[pos].set_level(TOP_RIGHT_BOTTOM_LEFT_DIR);
			else if (adjacent_tiles_arr[0][1] && adjacent_tiles_arr[2][1])
				m_tiles_vec[pos].set_level(TOP_LEFT_BOTTOM_RIGHT_DIR);
			else if (adjacent_tiles_arr[2][1] && adjacent_tiles_arr[1][0])
				m_tiles_vec[pos].set_level(TOP_RIGHT_BOTTOM_RIGHT_DIR);
			else if (adjacent_tiles_arr[0][1] && adjacent_tiles_arr[1][2])
				m_tiles_vec[pos].set_level(TOP_LEFT_BOTTOM_LEFT_DIR);
			else if (adjacent_tiles_arr[1][0] && adjacent_tiles_arr[0][1])
				m_tiles_vec[pos].set_level(TOP_LEFT_TOP_RIGHT_DIR);
			else if (adjacent_tiles_arr[2][1] && adjacent_tiles_arr[1][2])
				m_tiles_vec[pos].set_level(BOTTOM_LEFT_BOTTOM_RIGHT_DIR);
			else if (adjacent_tiles_arr[1][0])
				m_tiles_vec[pos].set_level(TOP_RIGHT_DIR);
			else if (adjacent_tiles_arr[1][2])
				m_tiles_vec[pos].set_level(BOTTOM_LEFT_DIR);
			else if (adjacent_tiles_arr[0][1])
				m_tiles_vec[pos].set_level(TOP_LEFT_DIR);
			else if (adjacent_tiles_arr[2][1])
				m_tiles_vec[pos].set_level(BOTTOM_RIGHT_DIR);
		}
	}
}

void Map::DFS(const std::vector<TileTypeEnum>& whitelist_vec, sf::Vector2i pos, unsigned int region_idx,
	unsigned int region_type)
{
	// Check if input position is valid or not.
	if (pos.x < 0 || pos.x >= m_width)
		return;
	if (pos.y < 0 || pos.y >= m_height)
		return;

	// Check if related tile was already visited before or not.
	if (m_tiles_vec[pos.y * m_width + pos.x].m_region_arr[0] != 0)
		return;

	// Check if current tile object's tile type is in the white list(can make up a region).
	bool is_tile_type_match{false};
	for (const auto& tileType : whitelist_vec)
		if (tileType == m_tiles_vec[pos.y * m_width + pos.x].m_tileType)
		{
			is_tile_type_match = true;
			break;
		}
	// If not, no longer needs subsequent process.
	if (!is_tile_type_match)
		return;

	// Otherwise assigns current Tile's region array's related region type's region a unique region_idx.
	m_tiles_vec[pos.y * m_width + pos.x].m_region_arr[region_type] = region_idx;

	// Call DFS function recursively on current tile object's four adjacent tile objects.
	DFS(whitelist_vec, pos + sf::Vector2i(0, 1), region_idx, region_type);
	DFS(whitelist_vec, pos + sf::Vector2i(1, 0), region_idx, region_type);
	DFS(whitelist_vec, pos + sf::Vector2i(-1, 0), region_idx, region_type);
	DFS(whitelist_vec, pos + sf::Vector2i(0, -1), region_idx, region_type);
}

void Map::select(sf::Vector2i& start_pos, sf::Vector2i& end_pos, const std::vector<TileTypeEnum>& blacklist_vec)
{
	// Ensure that start pos is always less than end pos on both axis.
	if (start_pos.x > end_pos.x)
		std::swap(start_pos.x, end_pos.x);
	if (start_pos.y > end_pos.y)
		std::swap(start_pos.y, end_pos.y);

	// Clamp both start pos and end pos in range.
	if (start_pos.x >= m_width)
		start_pos.x = (m_width - 1);
	else if (start_pos.x < 0)
		start_pos.x = 0;
	if (start_pos.y >= m_height)
		start_pos.y = (m_height - 1);
	else if (start_pos.y < 0)
		start_pos.y = 0;

	if (end_pos.x >= m_width)
		end_pos.x = m_width - 1;
	else if (end_pos.x < 0)
		end_pos.x = 0;
	if (end_pos.y >= m_height)
		end_pos.y = m_height - 1;
	else if (end_pos.y < 0)
		end_pos.y = 0;

	// Traverse each position within the rectangle boundary of the map.
	for (int y = start_pos.y; y <= end_pos.y; ++y)
		for (int x = start_pos.x; x <= end_pos.x; ++x)
		{
			// Mark each selected tile object's condition to 1(selected) first.
			m_selected_tiles_condition_vec[y * m_width + x] = TILE_SELECTED_FLAG;
			// Increment the selected tiles' number first.
			++m_selected_tiles_num;

			// Iterate through each invalid tile type(should not be selected/de-selected).
			for (const auto& tile_type : blacklist_vec)
				// Check if current selected tile has an invalid type to be selected/de-selected.
				if (m_tiles_vec[y * m_width + x].m_tileType == tile_type)
				{
					// Set related selected tile's condition and decrement the selected tiles' number.
					m_selected_tiles_condition_vec[y * m_width + x] = TILE_INVALID_FLAG;
					--m_selected_tiles_num;
					break;
				}
		}
}

void Map::deselect_tiles()
{
	// De-select each tile and reset the selected tiles' number to zero.
	for (auto val : m_selected_tiles_condition_vec)
		val = 0;
	m_selected_tiles_num = 0;
}

unsigned int Map::get_width() const
{
	return m_width;
}

unsigned int Map::get_height() const
{
	return m_height;
}

unsigned int Map::get_tile_half_width() const
{
	return m_tile_half_width;
}

void Map::set_tile_half_width(unsigned int tile_half_width)
{
	m_tile_half_width = tile_half_width;
}

char Map::get_selected_tile_condition(int idx) const
{
	return m_selected_tiles_condition_vec[idx];
}

Tile Map::get_tile(int idx) const
{
	return m_tiles_vec[idx];
}

void Map::set_tile(int idx, const Tile& new_tile)
{
	m_tiles_vec[idx] = new_tile;
}

unsigned int Map::get_tiles_amount() const
{
	return m_tiles_vec.size();
}