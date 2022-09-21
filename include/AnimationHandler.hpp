#pragma once
#ifndef ANIMATIONHANDLER_HPP
#define ANIMATIONHANDLER_HPP

#include "Animation.hpp"
#include <vector>
#include <memory>
#include "SFML/Graphics.hpp"

// Forward declaration.
class Animation;

/**
 * This class help manage each animation within the game.
 */
class AnimationHandler
{
 public:
	/**
	 * Constructor of AnimationHandler class.
	 */
	AnimationHandler() = default;

	/**
	 * One parameter constructor of AnimationHandler class.
	 * @param each_frame_size_rect A reference of const sf::IntRect object indicates the input frame size.
	 */
	explicit AnimationHandler(const sf::IntRect& each_frame_size_rect) : m_each_frame_size_rect
	(each_frame_size_rect)
	{};

	/**
	 * Add a new animation into the animation vec.
	 * @param anim_ref A reference of Animation object indicates the input new Animation.
	 */
	void addAnim(Animation& anim_ref);

	/**
	 * Update the current frame of animation. dt is the elapsed time since last chanceLevelUp function call.
	 * @param dt A const float indicates the elapsed time since last chanceLevelUp call
	 */
	void update(float dt);

	/**
	 * Change animation from current one with specified one.
	 * @param anim_idx A unsigned integer indicates the specified animation's index.
	 */
	void changeAnim(unsigned int anim_idx);

	/**
	 * Get and return m_each_frame_texture_rect member.
	 * @return A sf::IntRect indicates the m_each_frame_texture_rect member.
	 */
	sf::IntRect get_each_frame_texture_rect() const;

	/**
	 * Set each_frame_size_rect_ptr.
	 * @param int_rect A reference of const sf::IntRect indicates the new each_frame_size_rect.
	 */
	void SetEachFrameSizeRect(const sf::IntRect& int_rect);

 private:

	// A section of the texture that should be displayed(both position and dimension are used).
	sf::IntRect m_each_frame_texture_rect;

	// Stores each frame's specified size rect(only width and height are used).
	sf::IntRect m_each_frame_size_rect;

	// Holds a vector of all animation objects for the game.
	std::vector<Animation> m_animations_vec_ptr;
	// The total time elapsed since the animation loop started.
	float m_elapsed_time{0.f};
	// The index of current animation object, -1 indicates not start animation loop yet.
	int m_curr_animation_idx{-1};

};

#endif //ANIMATIONHANDLER_HPP
