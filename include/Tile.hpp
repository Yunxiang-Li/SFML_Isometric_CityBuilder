#pragma once
#ifndef TILE_HPP
#define TILE_HPP

#include "AnimationHandler.hpp"
#include "TileTypeEnum.hpp"

/**
 * This class represents all behaviors of one Tile object.
 */
class Tile
{

 public:
	/**
	 *	Default constructor.
	 */
	Tile() = default;
	/**
	 * Eight parameters constructor.
	 * @param tile_half_width_pixel A const unsigned int indicates the half width pixel size of the tile object.
	 * @param tile_height_num A const unsigned int represents the number of half tile width indicates the tile's height.
	 * @param texture A reference of sf::Texture indicates the texture of all related different levels' tiles.
	 * @param animation_vec A std::vector<Animation> indicates the tile object's related animation.
	 * @param tileType A reference of enum TileTypeEnum object indicates the tile type.
	 * @param cost A const unsigned int indicates the cost to placement the tile object.
	 * @param curr_level_population_limit A const unsigned int indicates the maximum possible population value for
	 * current level of the tile.
	 * @param max_level A const unsigned int indicates the maximum possible level of the tile.
	 */
	Tile(const unsigned int tile_half_width_pixel, const unsigned int tile_height_num, sf::Texture& texture,
		std::vector<Animation> animation_vec, const TileTypeEnum& tileType, const unsigned int cost,
		const unsigned int curr_level_population_limit, const unsigned int max_level) : m_tileType(tileType),
		m_cost(cost), m_population_limit_per_level(curr_level_population_limit), m_max_level(max_level),
		m_animationHandler_ptr(std::make_shared<AnimationHandler>())
		{
			/* Since one texture contains at most 4 tiles, we need to set up the top left point of the current
			 * tile sprite according to the whole texture.*/
			m_sprite.setOrigin(sf::Vector2f(0.f, (tile_height_num - 1) * tile_half_width_pixel));
			m_sprite.setTexture(texture);

			// Set up animation handler's m_each_frame_size_rect(we only care about width and height here).
			m_animationHandler_ptr->SetEachFrameSizeRect((sf::IntRect(0,0,
				2 * tile_half_width_pixel, tile_half_width_pixel * tile_height_num)));

			// Store all tile related animation.
			for (auto& each_anim : animation_vec)
			{
				m_animationHandler_ptr->addAnim(each_anim);
			}
			// Reset animation handler's elapsed time to 0.
			m_animationHandler_ptr->update(0.f);
		};

	/**
	 * Draw the tile sprite within the render window and play the related animation.
	 * @param render_window A reference of sf::RenderWindow indicates the place to render the tile sprite.
	 * @param dt A float indicates the elapsed time since last render call(also last AnimationHandler::chanceLevelUp
	 * call).
	 */
	void render(sf::RenderWindow& render_window, float dt);

	/**
	 * If the population is at the maximum value for the tile, there is a small chance that the tile will increase its
     * building level.
	 */
	void chanceLevelUp();

	/**
	 * Return the cost of placing current tile object in string form.
	 * @return A unsigned integer indicates the cost.
	 */
	unsigned int getCost() const;

	/**
	 * Set current tile object's level.
	 * @param level A unsigned integer indicates the level value to be set.
	 */
	void set_level(unsigned int level);

	/**
	 * Retrieve the tile's maximum population per level.
	 * @return A unsigned integer indicates Tile object's maximum population per level.
	 */
	unsigned int get_population_limit_per_level() const;

	/**
	 * Set new production_per_worker_per_day value.
	 * @param production_per_day A float indicates hte new production_per_worker_per_day value.
	 */
	void set_production_per_day(float production_per_day);

	/**
	 * Retrieve the tile's production per worker per day.
	 * @return A float indicates the tile's production per worker per day.
	 */
	float get_production_per_day() const;

	// Current Tile object's tile type.
	TileTypeEnum m_tileType{};
	/* The current level of Tile object which affects Road Tile object's appearance. For Zone(Residential, Industrial
	 * and Commercial) tiles, this value will also affect tile's maximum population. */
	unsigned int m_level{0};
	/* Indicates current tile object's related region's index, tiles in the same region are connected and can have
	 * different tile types. First element is used for transport.  m_region_arr[region_type] = region_idx means current
	 * tile object is inside the region_idxth's region of region_type. */
	unsigned int m_region_arr[1]{0};
	// Current residents inside the tile.
	double m_population{0.0};
	// Overall production of the tile.
	float m_total_production{0.f};
	// The sprite of the current tile object.
	sf::Sprite m_sprite;

 private:
	// Placement cost of each tile.
	unsigned int m_cost{0};
	// Indicates the population limit per level. This value will be updated according to Tile's current level.
	unsigned int m_population_limit_per_level{0};
	// Maximum level of current Tile object.
	unsigned int m_max_level{0};
	// Current tile's overall production per work and per day.
	float m_production_per_day{ 0.f};
	// An AnimationHandler object
	std::shared_ptr<AnimationHandler> m_animationHandler_ptr;
};

#endif //TILE_HPP
