#include <fstream>
#include "Map.hpp"

constexpr int EACH_TILE_PRODUCTION = 255;

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