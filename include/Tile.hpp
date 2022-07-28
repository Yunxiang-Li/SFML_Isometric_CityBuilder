#pragma once
#ifndef TILE_HPP
#define TILE_HPP

#include "AnimationHandler.hpp"
#include "TileType.hpp"
#include <array>

/**
 * This class represents all behaviors of one Tile object.
 */
class Tile
{
	/**
	 * A friend function to transform input TileType into related string.
	 * @param tile_type A TileType object indicates the input TileType.
	 * @return A std::string indicates the related string.
	 */
	friend std::string tileTypeToStr(TileType tile_type);
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
	 * @param tileType A reference of enum TileType object indicates the tile type.
	 * @param cost A const unsigned int indicates the cost to placement the tile object.
	 * @param curr_level_population_limit A const unsigned int indicates the maximum possible population value for
	 * current level of the tile.
	 * @param max_level A const unsigned int indicates the maximum possible level of the tile.
	 */
	Tile(const unsigned int tile_half_width_pixel, const unsigned int tile_height_num, sf::Texture& texture,
		std::vector<Animation> animation_vec, const TileType& tileType, const unsigned int cost,
		const unsigned int curr_level_population_limit, const unsigned int max_level) : m_tileType(tileType),
		m_region_id_arr({{0}}), m_cost(cost), m_curr_level_population_limit(curr_level_population_limit),
		m_max_level(max_level)
		{
			/* Since one texture contains at most 4 tiles, we need to set up the top left point of the current
			 * tile sprite according to the whole texture.*/
			m_sprite.setOrigin(sf::Vector2f(0.f, (tile_height_num - 1) * tile_half_width_pixel));
			m_sprite.setTexture(texture);

			// Set up animation handler's m_each_frame_size_rect(we only care about width and height here).
			m_animationHandler.m_each_frame_size_rect = sf::IntRect(0, 0,
				2 * tile_half_width_pixel, tile_half_width_pixel * tile_height_num);
			// Store all tile related animation.
			for (auto& each_anim : animation_vec)
			{
				m_animationHandler.add_anim(each_anim);
			}
			// Reset animation handler's elapsed time to 0.
			m_animationHandler.update(0.f);
		};

	/**
	 * Draw the tile sprite within the render window and play the related animation.
	 * @param render_window A reference of sf::RenderWindow indicates the place to render the tile sprite.
	 * @param dt A float indicates the elapsed time since last render call(also last AnimationHandler::chanceLevelUp call).
	 */
	void render(sf::RenderWindow& render_window, float dt);

	/**
	 * If the population is at the maximum value for the tile,
     * there is a small chance that the tile will increase its
     * building stage.
	 */
	void chanceLevelUp();

	/**
	 * Return the cost of placing current tile object in string form.
	 * @return A std::string indicates the string form of the cost.
	 */
	std::string getCost() const;

	// Current Tile object's tile type.
	TileType m_tileType{};
	/* The current level of Tile object which affects Tile object's appearance. For Zone(Road, Industrial and
	 * Commercial) tiles, this value will also affect tile's maximum population. */
	unsigned int m_level{0};
	/* An array of multiple elements indicate region IDs of the Tile object. These IDs indicate different meanings
	 * such as road connection, electricity available and has water or not.*/
	std::array<unsigned int, 2> m_region_id_arr;
	// Placement cost of each tile.
	unsigned int m_cost;
	// Current residents inside the tile.
	double m_population{0.0};
	// Indicates the population limit of current level. This value will be updated according to Tile's current level.
	unsigned int m_curr_level_population_limit;
	// Maximum level of current Tile object.
	unsigned int m_max_level;
	// Current tile's overall production per work and per day.
	float m_production_per_worker_per_day{0.f};
	// Overall production of the tile.
	float m_total_production{0.f};
	// The sprite of the current tile object.
	sf::Sprite m_sprite;
	// An AnimationHandler object
	AnimationHandler m_animationHandler;
};

#endif //TILE_HPP
