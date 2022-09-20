#pragma once
#ifndef MAINGAMESTATE_HPP
#define MAINGAMESTATE_HPP

#include "GameState.hpp"
#include "GameActionEnum.hpp"
#include "Map.hpp"
#include "Gui.hpp"

// Forward declaration.
class City;

/**
 * This class inherits from the virtual base GameState class and represents the main game state.
 */
class MainGameState : public GameState
{
	/**
 	* A friend function to transform input TileTypeEnum into related string.
 	* @param tile_type A TileTypeEnum object indicates the input TileTypeEnum.
 	* @return A std::string indicates the related string.
 	*/
	friend std::string tileTypeToStr(TileTypeEnum tile_type);
 public:
	/**
	 * Constructor of MainGameState class.
	 * @param game_ptr A std::unique_ptr<Game> object indicates the pointer of game object.
	 */
	explicit MainGameState(const std::shared_ptr<Game>& game_ptr);

	/**
	 * Virtual destructor.
	 */
	virtual ~MainGameState() = default;

	/**
	* Render the related scene according to delta time.
	* @param dt A const float value indicates the delta time.
	*/
	virtual void render(float dt) override;

	/**
	 * Update the related scene according to delta time.
	 * @param dt A const float value indicates the delta time.
	 */
	virtual void update(float dt) override;

	/**
	 * Handle user's input in the related scene.
	 */
	virtual void inputProcess() override;

 private:
	// A sf::View object indicates the game scene's 2D camera view.
	sf::View m_view;
	// A sf::View object indicates the game scene GUI's 2D camera view.
	sf::View m_gui_view;
	// Indicates player's current action inside the main game scene.
	GameActionEnum m_action_state;
	// A std::shared_ptr<City> indicates the pointer of game 's city(which contains a Map object inside).
	std::shared_ptr<City> m_game_city_ptr;
	/* Keep track of mouse screen position since last camera panning event. When player presses mouse middle button and
	 * moves mouse at the same time, the world(game view) should also move towards the opposite direction and this
	 * position should be updated. */
	sf::Vector2i m_prev_mouse_pos{0, 0};
	/* Indicates the view's zoom level which will double or halve as the player scrolls the mouse wheel forward or
	 * backward. Default value is 1.f which means the original size. */
	float m_zoom_level{1.f};

	// Start and end positions passed into map's select function. Default value is (0, 0).
	sf::Vector2i m_select_start_pos{0, 0};
	sf::Vector2i m_select_end_pos{0, 0};

	// A pointer of player's current selected tile object.
	std::shared_ptr<Tile> m_curr_selected_tile_ptr;

	// A std::unordered_map where each Gui object's string name is the key, related Gui object is the value.
	std::unordered_map<std::string, Gui> m_str_gui_map;
};

#endif //MAINGAMESTATE_HPP
