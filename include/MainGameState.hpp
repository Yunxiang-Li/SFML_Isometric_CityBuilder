#pragma once
#ifndef MAINGAMESTATE_HPP
#define MAINGAMESTATE_HPP

#include "GameState.hpp"
#include "GameActionEnum.h"
#include "Map.hpp"

/**
 * This class inherits from the virtual base GameState class and represents the main game state.
 */
class MainGameState : public GameState
{
 public:
	/**
	 * Constructor of MainGameState class.
	 * @param game_ptr A std::unique_ptr<Game> object indicates the pointer of game object.
	 */
	MainGameState(std::shared_ptr<Game> game_ptr);

	/**
	* Render the related scene according to delta time.
	* @param dt A const float value indicates the delta time.
	*/
	virtual void render(const float dt) override;

	/**
	 * Update the related scene according to delta time.
	 * @param dt A const float value indicates the delta time.
	 */
	virtual void update(const float dt) override;

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
	// Indicates the main game scene's map.
	Map m_game_map;
	/* Keep track of mouse position since last camera panning event. When player presses mouse middle button and moves
	 * mouse at the same time, the world(game view) should also move towards the opposite direction and this position
	 * should be updated. */
	sf::Vector2i m_prev_mouse_pos;
	/* Indicates the view's zoom level which will double or halve as the player scrolls the mouse wheel forward or
	 * backward. */
	float m_zoom_level;
};

#endif //MAINGAMESTATE_HPP
