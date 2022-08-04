#include <fstream>
#include "Map.hpp"

// Define each tile object's production.
constexpr int EACH_TILE_PRODUCTION = 255;

// Define all possible directions.
constexpr int TOP_RIGHT_BOTTOM_LEFT_DIR = 0;
constexpr int TOP_RIGHT_DIR = TOP_RIGHT_BOTTOM_LEFT_DIR;
constexpr int BOTTOM_LEFT_DIR = TOP_RIGHT_BOTTOM_LEFT_DIR;

constexpr int TOP_LEFT_BOTTOM_RIGHT_DIR = 1;
constexpr int TOP_LEFT_DIR = TOP_LEFT_BOTTOM_RIGHT_DIR;
constexpr int BOTTOM_RIGHT_DIR = TOP_LEFT_BOTTOM_RIGHT_DIR;

constexpr int TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT_DIR = 2;
constexpr int TOP_RIGHT_BOTTOM_RIGHT_DIR = 3;
constexpr int TOP_LEFT_BOTTOM_LEFT_DIR = 4;
constexpr int TOP_LEFT_TOP_RIGHT_DIR = 5;
constexpr int BOTTOM_LEFT_BOTTOM_RIGHT_DIR = 6;
constexpr int TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT_DIR = 7;
constexpr int TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT_DIR = 8;
constexpr int TOP_LEFT_TOP_RIGHT_BOTTOM_RIGHT_DIR = 9;
constexpr int TOP_LEFT_BOTTOM_LEFT_BOTTOM_RIGHT_DIR = 10;


Map::Map(const std::string& file_name, unsigned int width, unsigned int height,
	std::unordered_map<std::string, Tile>& str_tile_map) : m_tile_half_width(TILE_HALF_WIDTH)
{
	this->load(file_name, width, height, str_tile_map);
}

void Map::load(const std::string& file_name, unsigned int width, unsigned int height,
	std::unordered_map<std::string, Tile>& str_tile_map)
{
	// Open the store eternal file in binary form to read.
	std::ifstream input_file;
	input_file.open(file_name, std::ios::binary);

	// Store the dimension of the map.
	m_width = width;
	m_height = height;

	for (int pos = 0; pos < (m_width * m_height); ++pos)
	{
		// Set each tile object's initial production to be 255.
		m_resource_vec.emplace_back(EACH_TILE_PRODUCTION);

		// Read and store each tile object's tile type.
		TileType tileType;
		input_file.read(reinterpret_cast<char*>(&tileType), sizeof(TileType));

		// Set up each tile object's texture according to its tile type.
		switch(tileType)
		{
			case TileType::VOID:
			case TileType::GRASS:
				m_tiles_vec.emplace_back(str_tile_map.at("grass"));
				break;
			case TileType::FOREST:
				m_tiles_vec.emplace_back(str_tile_map.at("forest"));
				break;
			case TileType::WATER:
				m_tiles_vec.emplace_back(str_tile_map.at("water"));
				break;
			case TileType::RESIDENTIAL:
				m_tiles_vec.emplace_back(str_tile_map.at("residential"));
				break;
			case TileType::COMMERCIAL:
				m_tiles_vec.emplace_back(str_tile_map.at("commercial"));
				break;
			case TileType::INDUSTRIAL:
				m_tiles_vec.emplace_back(str_tile_map.at("industrial"));
				break;
			case TileType::ROAD:
				m_tiles_vec.emplace_back(str_tile_map.at("road"));
				break;
			default:
				break;
		}

		// Set up each tile object's current level, region id array, current population and total production.
		Tile& curr_tile_ref = m_tiles_vec.back();
		input_file.read(reinterpret_cast<char*>(&(curr_tile_ref.m_level)), sizeof(unsigned int));
		input_file.read(reinterpret_cast<char*>(&(curr_tile_ref.m_region_id_arr)), sizeof(unsigned int) * 1);
		input_file.read(reinterpret_cast<char*>(&(curr_tile_ref.m_population)), sizeof(double));
		input_file.read(reinterpret_cast<char*>(&(curr_tile_ref.m_total_production)), sizeof(float));

		input_file.close();
	}
}

void Map::save(const std::string& file_name)
{
	// Open the eternal file in binary form to write.
	std::ofstream output_file;
	output_file.open(file_name, std::ios::binary);

	// Store each tile object's tile type, current level, region id array, current population and total production.
	for (auto& each_tile : m_tiles_vec)
	{
		output_file.write(reinterpret_cast<char*>(&(each_tile.m_tileType)), sizeof(TileType));
		output_file.write(reinterpret_cast<char*>(&(each_tile.m_level)), sizeof(unsigned int));
		output_file.write(reinterpret_cast<char*>(&(each_tile.m_region_id_arr)), sizeof(unsigned int) * 3);
		output_file.write(reinterpret_cast<char*>(&(each_tile.m_population)), sizeof(double));
		output_file.write(reinterpret_cast<char*>(&(each_tile.m_total_production)), sizeof(float));
	}

	output_file.close();
}

void Map::render(sf::RenderWindow& renderWindow, float dt)
{
	sf::Vector2f pos;
	// Iterate horizontally and then vertically to render each isometric tile.
	for (int y = 0; y < m_height; ++y)
		for (int x = 0; x < m_width; ++x)
		{
			// Get each isometric tile's position.
			pos.x = m_tile_half_width * (x - y);
			pos.y = m_tile_half_width * (x + y) * 0.5;
			// Bind each tile's position with related element inside m_tiles_vec.
			m_tiles_vec[y * m_width + x].m_sprite.setPosition(pos);
			// Draw each tile object.
			m_tiles_vec[y * m_width + x].render(renderWindow, dt);
		}
}

void Map::updateDirection(TileType tileType)
{
	for(int y = 0; y < m_height; ++y)
	{
		for(int x = 0; x < m_width; ++x)
		{
			// Retrieve current position.
			int pos = y * m_width + x;

			// Check if current tile object's type matches the input.
			if(m_tiles_vec[pos].m_tileType != tileType)
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

			std::array<std::array<int, 3>, 3> adjacent_tiles_arr{{{0,0,0},{0,0,0},{0,0,0}}};

			// Match with top tile object.
			if(x > 0 && y > 0)
				adjacent_tiles_arr[0][0] = (m_tiles_vec[(y - 1) * m_width + (x - 1)].m_tileType == tileType);
			// Match with top left tile object.
			if(y > 0)
				adjacent_tiles_arr[0][1] = (m_tiles_vec[(y - 1) * m_width + x].m_tileType == tileType);
			// Match with left tile object.
			if(x < m_width - 1 && y > 0)
				adjacent_tiles_arr[0][2] = (m_tiles_vec[(y - 1) * m_width + (x + 1)].m_tileType == tileType);
			// Match with top right tile object.
			if(x > 0)
				adjacent_tiles_arr[1][0] = (m_tiles_vec[y * m_width + (x - 1)].m_tileType == tileType);
			// Match with bottom left tile object.
			if(x < m_width-1)
				adjacent_tiles_arr[1][2] = (m_tiles_vec[y * m_width + (x + 1)].m_tileType == tileType);
			// Match with right tile object.
			if(x > 0 && y < m_height - 1)
				adjacent_tiles_arr[2][0] = (m_tiles_vec[(y + 1) * m_width + (x - 1)].m_tileType == tileType);
			// Match with bottom right tile object.
			if(y < m_height - 1)
				adjacent_tiles_arr[2][1] = (m_tiles_vec[(y + 1) * m_width + x].m_tileType == tileType);
			// Match with bottom tile object.
			if(x < m_width-1 && y < m_height - 1)
				adjacent_tiles_arr[2][2] = (m_tiles_vec[(y + 1) * m_width + (x + 1)].m_tileType == tileType);

			// Change the tile level depending on related adjacent elements' values.
			if(adjacent_tiles_arr[1][0] && adjacent_tiles_arr[1][2] && adjacent_tiles_arr[0][1] &&
			adjacent_tiles_arr[2][1])
				m_tiles_vec[pos].m_level = TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT_DIR;
			else if(adjacent_tiles_arr[1][0] && adjacent_tiles_arr[1][2] && adjacent_tiles_arr[0][1])
				m_tiles_vec[pos].m_level = TOP_LEFT_TOP_RIGHT_BOTTOM_LEFT_DIR;
			else if(adjacent_tiles_arr[1][0] && adjacent_tiles_arr[1][2] && adjacent_tiles_arr[2][1])
				m_tiles_vec[pos].m_level = TOP_RIGHT_BOTTOM_LEFT_BOTTOM_RIGHT_DIR ;
			else if(adjacent_tiles_arr[0][1] && adjacent_tiles_arr[2][1] && adjacent_tiles_arr[1][0])
				m_tiles_vec[pos].m_level = TOP_LEFT_TOP_RIGHT_BOTTOM_RIGHT_DIR;
			else if(adjacent_tiles_arr[0][1] && adjacent_tiles_arr[2][1] && adjacent_tiles_arr[1][2])
				m_tiles_vec[pos].m_level = TOP_LEFT_BOTTOM_LEFT_BOTTOM_RIGHT_DIR;
			else if(adjacent_tiles_arr[1][0] && adjacent_tiles_arr[1][2])
				m_tiles_vec[pos].m_level = TOP_RIGHT_BOTTOM_LEFT_DIR;
			else if(adjacent_tiles_arr[0][1] && adjacent_tiles_arr[2][1])
				m_tiles_vec[pos].m_level = TOP_LEFT_BOTTOM_RIGHT_DIR;
			else if(adjacent_tiles_arr[2][1] && adjacent_tiles_arr[1][0])
				m_tiles_vec[pos].m_level = TOP_RIGHT_BOTTOM_RIGHT_DIR;
			else if(adjacent_tiles_arr[0][1] && adjacent_tiles_arr[1][2])
				m_tiles_vec[pos].m_level = TOP_LEFT_BOTTOM_LEFT_DIR;
			else if(adjacent_tiles_arr[1][0] && adjacent_tiles_arr[0][1])
				m_tiles_vec[pos].m_level = TOP_LEFT_TOP_RIGHT_DIR;
			else if(adjacent_tiles_arr[2][1] && adjacent_tiles_arr[1][2])
				m_tiles_vec[pos].m_level = BOTTOM_LEFT_BOTTOM_RIGHT_DIR;
			else if(adjacent_tiles_arr[1][0])
				m_tiles_vec[pos].m_level = TOP_RIGHT_DIR;
			else if(adjacent_tiles_arr[1][2])
				m_tiles_vec[pos].m_level = BOTTOM_LEFT_DIR;
			else if(adjacent_tiles_arr[0][1])
				m_tiles_vec[pos].m_level = TOP_LEFT_DIR ;
			else if(adjacent_tiles_arr[2][1])
				m_tiles_vec[pos].m_level = BOTTOM_RIGHT_DIR;
		}
	}
}

void Map::DFS(std::vector<TileType>& whitelist_vec, sf::Vector2i pos, int label, unsigned int type)
{
	// Check if input position is valid or not.
	if (pos.x < 0 || pos.x >= m_width)
		return;
	if (pos.y < 0 || pos.y >= m_height)
		return;
}
