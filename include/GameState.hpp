#pragma once
#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "Game.hpp"

/**
 * A virtual base class handles render, chanceLevelUp, input process under each game state.
 */
class GameState
{
 public:
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

	/**
	 * Virtual destructor.
	 */
	virtual ~GameState() = default;

 protected:

	/**
	 * Retrieve the game pointer.
	 * @return A std::shared_ptr<Game> indicates the game pointer.
	 */
	inline std::shared_ptr<Game> get_game_ptr() const
	{
		return m_game_ptr;
	};

	/**
	 * Set the game pointer.
	 * @param game_ptr A std::shared_ptr<Game> indicates the new game pointer to be set.
	 */
	inline void set_game_ptr(std::shared_ptr<Game> game_ptr)
	{
		m_game_ptr = game_ptr;
	};

 private:
	// A member indicates the shared pointer of the game object.
	std::shared_ptr<Game> m_game_ptr;
};

#endif // GAMESTATE_HPP
