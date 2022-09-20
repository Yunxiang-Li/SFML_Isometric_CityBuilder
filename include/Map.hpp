#pragma once
#ifndef MAP_HPP
#define MAP_HPP

#include "Tile.hpp"
#include "Utility.hpp"
#include <unordered_map>
#include <string>

/**
 * This class contains a vector of Tile objects which is responsible for all game map behaviors.
 */
class Map
{
 public:
	/**
	 * Constructor of Map class. Create a blank map.
	 */
	Map() = default;

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
	 * @param region_tiles_type_vec A reference of const vector contains all different TileTypes that can make up the
	 * regions(ROAD, RESIDENTIAL, COMMERCIAL, INDUSTRIAL).
	 * @param region_index An unsigned integer indicates the index of region(also index of Tile class's member
	 * m_region_arr). Default value is 0. A certain region can have tiles of different tile types.
	 */
	void calculateConnectedRegionsNum(const std::vector<TileTypeEnum>& region_tiles_type_vec, unsigned int region_index
	= 0);

	/**
	 * Update the direction of directional tiles(roads, water) so that they face the correct direction.
	 * @param tileType A TileTypeEnum object indicates the type of tile object.
	 */
	void updateDirection(TileTypeEnum tileType);

	/**
	 * Selects all tiles within the bounding rectangle of start_pos and end_pos and sets all tiles within that rectangle
	 * which are also in the blacklist to invalid condition.
	 * @param start_pos A reference of sf::Vector2i indicates the smaller position on map indicates the top left corner
	 * of the bounding rectangle.
	 * @param end_pos A reference of sf::Vector2i indicates bigger position on map indicates the bottom right corner of
	 * the bounding rectangle.
	 * @param blacklist_vec A const reference of std::vector<TileTypeEnum> indicates all invalid tile types(cannot be
	 * selected/de-selected).
	 */
	void select(sf::Vector2i& start_pos, sf::Vector2i& end_pos, const std::vector<TileTypeEnum>& blacklist_vec);

	/**
	 * De-selected all valid tiles.
	 */
	void deselect_tiles();

	/**
	 * Retrieve the map's width.
	 * @return A unsigned integer indicates the map's width.
	 */
	unsigned int get_width() const;

	/**
	 * Retrieve the map's height.
	 * @return A unsigned integer indicates the map's height.
	 */
	unsigned int get_height() const;

	/**
	 * Retrieve the length of Tile object's half width.
	 * @return A unsigned integer indicates the length of Tile object's half width.
	 */
	unsigned int get_tile_half_width() const;

	/**
	 * Set the tile object's half width size.
	 * @param tile_half_width A unsigned integer indicates the tile object's half width size.
	 */
	void set_tile_half_width(unsigned int tile_half_width);

	/**
	 * Retrieve the idx-th selected tile's condition.
	 * @param idx An integer indicates the index of the tile.
	 * @return A char indicates the specified selected tile's condition(0 for de-selected, 1 for selected and 2 for
	 * invalid to be selected/deselected).
	 */
	char get_selected_tile_condition(int idx) const;

	/**
	 * Get specified Tile object.
	 * @param idx An integer indicates the index of the tile.
	 * @return A copy of specified Tile object.
	 */
	Tile get_tile(int idx) const;

	/**
 	* Get specified Tile object's reference.
 	* @param idx An integer indicates the index of the tile.
 	* @return A reference of specified Tile object.
 	*/
	Tile& get_tile_ref(int idx);

	/**
	 * Set specified Tile object to be identical with input Tile object.
	 * @param idx An integer indicates the index of the tile.
	 * @param new_tile A reference of const Tile object indicates the input Tile object.
	 */
	void set_tile(int idx, const Tile& new_tile);

	/**
	 * Retrieve the amount of Tile objects that construct the map.
	 * @return A unsigned integer indicates the amount.
	 */
	unsigned int get_tiles_amount() const;

	/**
	 * Retrieve the specified Tile object's underground resource.
	 * @param idx An integer indicates the index of the tile.
	 * @return A unsigned integer indicates the resource.
	 */
	unsigned int get_tile_resource(int idx) const;

	/**
	 * Decrement related Tile object's resource by one.
	 * @param idx An integer indicates the index of the tile.
	 */
	void decrement_tile_resource(int idx);

	/**
	 * Retrieve the reference of m_tiles_vec.
	 * @return A reference of m_tiles_vec.
	 */
	std::vector<Tile>& get_tiles_vec_ref();

	/**
	 * Retrieve the amount of Tile objects that selected by player.
	 * @return A unsigned integer indicates the amount.
	 */
	unsigned int get_selected_tiles_num() const;

 private:

	// Dimension of the map.
	unsigned int m_width{0};
	unsigned int m_height{0};
	// Holds all Tile objects to construct the map.
	std::vector<Tile> m_tiles_vec;
	// Contains each tile object's limited production.
	std::vector<unsigned int> m_resource_vec;
	// Each tile object has 16 pixels' width(thus half width is 8 pixels) and 8 pixels' height.
	unsigned int m_tile_half_width{TILE_HALF_WIDTH};
	// An unsigned int array, index indicates the region type, result indicates the related regions' number.
	unsigned int m_region_num_arr[1]{0};
	// Indicates number of selected tiles.
	unsigned int m_selected_tiles_num{0};
	/* Indicates current map's all selected tiles' conditions. Value 0 means related tile is de-selected, 1 means
	 * related tile is selected and 2 means related tile is invalid to be selected/de-selected. */
	std::vector<char> m_selected_tiles_condition_vec{};

	/**
	 * Split the map into different regions and region_idx them according to what region they fall in.
	 * @param whitelist_vec A const reference of const vector contains all different TileTypes that can make up the
	 * regions(ROAD, RESIDENTIAL, COMMERCIAL, INDUSTRIAL).
	 * @param pos A sf::Vector2i object indicates the position of current tile object.
	 * @param region_idx An unsigned integer indicates a certain region.
	 * @param region_type An unsigned integer indicates the index of Tile class's member m_region_arr. The default value
	 * is zero.  A certain region can have tiles of different tile types.
	 */
	void DFS(const std::vector<TileTypeEnum>& whitelist_vec, sf::Vector2i pos, unsigned int region_idx,
		unsigned int region_type = 0);

	void generate_river();
};

#endif //MAP_HPP
