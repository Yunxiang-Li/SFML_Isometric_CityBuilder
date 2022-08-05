#pragma once
#ifndef MAP_HPP
#define MAP_HPP

#include "Tile.hpp"
#include "unordered_map"

// Define each tile object's half width in pixels.
constexpr unsigned int TILE_HALF_WIDTH = 8;
/**
 * This class contains a vector of Tile objects which is responsible for all game map behaviors.
 */
class Map
{
 public:
	/**
	 * Constructor of Map class. Create a blank map.
	 */
	Map() : m_width(0), m_height(0), m_tile_half_width(TILE_HALF_WIDTH), m_region_num{0} {};

	/**
	 * Four parameter constructor of Map class. Create the map from a file.
	 * @param file_name A const reference of const std::string indicates the file name of the binary file to be re
	 * @param width An unsigned integer indicates the width of map.
	 * @param height An unsigned integer indicates the height of map.
	 * @param str_tile_map A reference of unordered_map indicates contains mapping from each tile's name to relater tile
	 * object.
	 */
	Map(const std::string& file_name, unsigned int width, unsigned int height, std::unordered_map<std::string,
		Tile>& str_tile_map);

	/**
	 * Load map data from disk.
	 * @param file_name A const reference of const std::string indicates the file name of the binary file to be read
	 * from.
	 * @param width An unsigned integer indicates the width of map.
	 * @param height An unsigned integer indicates the height of map.
	 * @param str_tile_map A reference of unordered_map indicates contains mapping from each tile's name to relater tile
	 * object.
	 */
	void load(const std::string& file_name, unsigned int width, unsigned int height,
		std::unordered_map<std::string, Tile>& str_tile_map);

	/**
	 * Save map data into disk.
	 * @param file_name  A const reference of const std::string indicates the file name of the binary file to be read
	 * from.
	 */
	void save(const std::string& file_name);

	/**
	 * Render the map.
	 * @param renderWindow A reference of a sf::RenderWindow object indicates the render window.
	 * @param dt A float indicates the elapsed time since last call.
	 */
	void render(sf::RenderWindow& renderWindow, float dt);

	/**
	 * Checks if one position in the map is connected to another by only traversing tiles in the whitelist.
	 * @param whitelist_vec A reference of const vector contains all different TileTypes that can make up the regions.
	 * @param region_type An unsigned integer indicates the index of region's array stored inside Tile class.
	 * Default value is 0.
	 */
	void findConnectedRegions(const std::vector<TileType>& whitelist_vec, unsigned int region_type = 0);

	/**
	 * Update the direction of directional tiles(roads, water) so that they face the correct direction.
	 * @param tileType A TileType object indicates the type of tile object.
	 */
	void updateDirection(TileType tileType);

	// Dimension of the map.
	unsigned int m_width;
	unsigned int m_height;
	// Holds all Tile objects to construct the map.
	std::vector<Tile> m_tiles_vec;
	// Contains each industrial tile object's limited production.
	std::vector<unsigned int> m_resource_vec;
	// Each tile object has 16 pixels' width(thus half width is 8 pixels) and 8 pixels' height.
	unsigned int m_tile_half_width;
	// TODO:
	unsigned int m_selected_num;
	// An unsigned int array, index indicates the region type, result indicates the related region.
	unsigned int m_region_num[1];

 private:
	/**
	 * Split the map into different regions and region_idx them according to what region they fall in.
	 * @param whitelist_vec A const reference of const vector contains all different TileTypes that can make up the regions.
	 * @param pos A sf::Vector2i object indicates the position of current tile object.
	 * @param region_idx An unsigned integer indicates a certain region.
	 * @param region_type An unsigned integer indicates a specific region type which is also the index of region id
	 * array that stored inside Tile class. The default value is zero.
	 */
	void DFS(const std::vector<TileType>& whitelist_vec, sf::Vector2i pos, unsigned int region_idx,
		unsigned int region_type = 0);
};

#endif //MAP_HPP
