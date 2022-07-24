#pragma once
#ifndef MAINGAMESTATE_HPP
#define MAINGAMESTATE_HPP

#include "GameState.hpp"
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
	sf::View m_main_game_view;
	// A sf::View object indicates the game scene GUI's 2D camera view.
	sf::View m_gui_view;
};

#endif //MAINGAMESTATE_HPP
