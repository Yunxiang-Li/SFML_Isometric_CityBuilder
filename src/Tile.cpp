#include "Tile.hpp"

// Help generate random number.
constexpr unsigned int RAND_BASE_NUM = 1e3;
constexpr unsigned int LEVEL_BASE_NUM = 1e2;

void Tile::render(sf::RenderWindow& render_window, float dt)
{
	// Switch to current tile object's animation.
	m_animationHandler.change_anim(m_level);
	// Update the current frame of animation.
	m_animationHandler.update(dt);
	// Set new current frame of animation's texture rect.
	m_sprite.setTextureRect(m_animationHandler.m_each_frame_texture_rect);
	// Draw current tile sprite on the render window.
	render_window.draw(m_sprite);
}

void Tile::chanceLevelUp()
{
	/* Check if current tile is a zone tile, and its current population reaches current maximum population,
	 * and also current tile's level is still lower than the max level. */
	if ((m_tileType == TileType::RESIDENTIAL || m_tileType == TileType::COMMERCIAL
	|| m_tileType == TileType::INDUSTRIAL) && (m_population == m_curr_level_population_limit * (m_level + 1))
	&& m_level < m_max_level)
	{
		/* (rand() % int(RAND_BASE_NUM) will range between 0 and 999. For LEVEL_BASE_NUM / (m_level + 1),
		 * result will be 100 for level 0, thus 10% chance to level up. Result will be 50 for level 1 thus 5% chance
		 * to level up. Result will be 33 for level 2 thus 3.3% chance to level up.
		 * Result will be 25 for level 3 thus 2.5% chance to level up.*/
		if((rand() % int(RAND_BASE_NUM)) < (LEVEL_BASE_NUM / (m_level + 1)))
			++m_level;

	}
}

std::string Tile::getCost() const
{
	return std::to_string(m_cost);
}

std::string tileTypeToStr(TileType tile_type)
{
	// A string store the result.
	std::string res_str{};
	// Check each case, for unlisted input, use Void to represent.
	switch (tile_type)
	{
		case TileType::INDUSTRIAL:
			res_str = "Industrial";
			break;
		case TileType::COMMERCIAL:
			res_str = "Commercial";
			break;
		case TileType::RESIDENTIAL:
			res_str = "Residential";
			break;
		case TileType::GRASS:
			res_str = "Grass";
			break;
		case TileType::FOREST:
			res_str = "Forest";
			break;
		case TileType::WATER:
			res_str = "Water";
			break;
		default:
			res_str = "Void";
			break;
	}
	return res_str;
}