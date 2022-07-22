#pragma once
#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "Game.hpp"

/**
 * A virtual base class handles render, update, input process under each game state.
 */
class GameState
{
	public:
	// A member indicates the unique pointer of the game object.
	std::unique_ptr<Game> m_game_ptr;

	/**
	 * Render the related scene according to delta time.
	 * @param dt A const float value indicates the delta time.
	 */
	virtual void render(const float dt) = 0;

	/**
	 * Update the related scene according to delta time.
	 * @param dt A const float value indicates the delta time.
	 */
	virtual void update(const float dt) = 0;

	/**
	 * Handle user's input in the related scene.
	 */
	virtual void inputProcess() = 0;
};

#endif // GAMESTATE_HPP
