#pragma once
#ifndef MAINMENUGAMESTATE_HPP
#define MAINMENUGAMESTATE_HPP

#include "GameState.hpp"

/**
 * This class inherits from the virtual base GameState class and represents the main menu game state.
 */
class MainMenuGameState : public GameState
{
	public:
	/**
	 * Constructor of MainMenuGameState class.
	 * @param game_ptr A std::shared_ptr<Game> object indicates the pointer of game object.
	 */
	MainMenuGameState(std::shared_ptr<Game> game_ptr);

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
    /**
    * Load the main game.
    */
    void loadGame();

	// A sf::View object indicates the game's view.
	sf::View m_game_view;
};
#endif // MAINMENUGAMESTATE_HPP
