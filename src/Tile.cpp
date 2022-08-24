#include "Tile.hpp"
#include "Utility.hpp"

void Tile::render(sf::RenderWindow& render_window, float dt)
{
	// Switch to current tile object's animation.
	m_animationHandler_ptr->changeAnim(m_level);
	// Update the current frame of animation.
	m_animationHandler_ptr->update(dt);
	// Set new current frame of animation's texture rect.
	m_sprite.setTextureRect(m_animationHandler_ptr->get_each_frame_texture_rect());
	// Draw current tile sprite on the render window.
	render_window.draw(m_sprite);
}

void Tile::chanceLevelUp()
{
	/* Check if current tile is a zone tile, and its current population reaches current maximum population,
	 * and also current tile's level is still lower than the max level. */
	if ((m_tileType == TileTypeEnum::RESIDENTIAL || m_tileType == TileTypeEnum::COMMERCIAL
	|| m_tileType == TileTypeEnum::INDUSTRIAL) && (m_population == m_curr_level_population_limit * (m_level + 1))
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

std::string tileTypeToStr(TileTypeEnum tile_type)
{
	// A string store the result.
	std::string res_str{};
	// Check each case, for unlisted input, use Void to represent.
	switch (tile_type)
	{
		case TileTypeEnum::INDUSTRIAL:
			res_str = "Industrial";
			break;
		case TileTypeEnum::COMMERCIAL:
			res_str = "Commercial";
			break;
		case TileTypeEnum::RESIDENTIAL:
			res_str = "Residential";
			break;
		case TileTypeEnum::GRASS:
			res_str = "Grass";
			break;
		case TileTypeEnum::FOREST:
			res_str = "Forest";
			break;
		case TileTypeEnum::WATER:
			res_str = "Water";
			break;
		default:
			res_str = "Void";
			break;
	}
	return res_str;
}

void Tile::set_level(unsigned int level)
{
	m_level = level;
}
