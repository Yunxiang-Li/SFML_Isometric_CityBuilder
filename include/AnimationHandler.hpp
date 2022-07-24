#pragma once
#ifndef ANIMATIONHANDLER_HPP
#define ANIMATIONHANDLER_HPP

#include "Animation.hpp"
#include <vector>
#include "SFML/Graphics.hpp"

class AnimationHandler
{
 public:
	/**
	 * Constructor of AnimationHandler class.
	 */
	AnimationHandler() : m_elapsed_time(0.f), m_curr_animation_idx(-1)
	{
	};

	/**
	 * Copy constructor of AnimationHandler class.
	 * @param each_frame_size_rect A const reference of sf::IntRect indicates the input frame size.
	 */
	AnimationHandler(const sf::IntRect& each_frame_size_rect) : m_each_frame_size_rect(each_frame_size_rect),
	m_elapsed_time(0.f), m_curr_animation_idx(-1)
	{
	};

	/**
	 * Add a new animation into the animation vec.
	 * @param anim_ref A reference of an Animation object indicates the input new Animation.
	 */
	void add_anim(Animation& anim_ref);

	/**
	 * Update the current frame of animation. dt is the elapsed time since last update function call.
	 * @param dt A const float indicates the elapsed time since last update call
	 */
	void update(const float dt);

	/**
	 * Change animation from current one with specified one.
	 * @param anim_idx A const unsigned integer indicates the specified animation's index.
	 */
	void change_anim(const unsigned int anim_idx);

	// Each frame's section of the texture that should be displayed(both position and dimension are used).
	sf::IntRect m_each_frame_texture_rect;

	// Each frame's specified size rect(only width and height are used).
	sf::IntRect m_each_frame_size_rect;

 private:
	// Holds all animation objects for the game.
	std::vector<Animation> m_animations_vec;
	// The total time elapsed since the animation loop started.
	float m_elapsed_time;
	// The index of current animation object, -1 indicates not start animation loop yet.
	int m_curr_animation_idx;

};

#endif //ANIMATIONHANDLER_HPP
