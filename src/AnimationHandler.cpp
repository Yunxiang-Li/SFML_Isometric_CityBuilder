#include "AnimationHandler.hpp"

void AnimationHandler::addAnim(Animation& anim_ref)
{
	m_animations_vec_ptr.emplace_back(anim_ref);
}

void AnimationHandler::update(const float dt)
{
	// Check if current animation index is valid or not.
	if (m_curr_animation_idx < 0 || m_curr_animation_idx >= m_animations_vec_ptr.size())
		return;
	// Store current animation's duration per frame.
	float each_frame_duration = m_animations_vec_ptr[m_curr_animation_idx].getDuration();
	// Check if elapsed time so far proceeds to next frame.
	if (static_cast<int>((m_elapsed_time + dt) / each_frame_duration) >
	static_cast<int>(m_elapsed_time / each_frame_duration))
	{
		// Get next frame index.
		int next_frame_idx = static_cast<int>((m_elapsed_time + dt) / each_frame_duration);
		// Use modulo operation to prevent index out of bound issue.
		next_frame_idx %= m_animations_vec_ptr[m_curr_animation_idx].getTotalFrame();

		// Create a temp sf::IntRect as a copy of each frame's rect.
		sf::IntRect temp_rect(m_each_frame_size_rect);
		// Update temp rect's left and top to be next frame's real texture rect.
		temp_rect.left = temp_rect.width * next_frame_idx;
		temp_rect.top = temp_rect.height * m_curr_animation_idx;
		// Set up next frame's texture rect.
		m_each_frame_texture_rect = temp_rect;
	}

	// Update elapsed time so far.
	m_elapsed_time += dt;
	// Reset m_elapsed_time if current animation is completed.
	if (m_elapsed_time > each_frame_duration * m_animations_vec_ptr[m_curr_animation_idx].getTotalFrame())
		m_elapsed_time = 0.f;
}

void AnimationHandler::changeAnim(const unsigned int anim_idx)
{
	// Handle input animation index is the current animation and invalid input animation index corner cases.
	if (m_curr_animation_idx == anim_idx || anim_idx >= m_animations_vec_ptr.size())
		return;
	// Set up current animation index.
	m_curr_animation_idx = anim_idx;

	// Create a temp sf::IntRect as a copy of each frame's rect.
	sf::IntRect temp_rect(m_each_frame_size_rect);
	// Update temp rect's left and top to be first frame's real texture rect.
	temp_rect.top = temp_rect.height * m_curr_animation_idx;
	// Set up next animation's first texture rect and reset total elapsed time.
	m_each_frame_texture_rect = static_cast<const sf::IntRect>(temp_rect);
	m_elapsed_time = 0.f;
}

sf::IntRect AnimationHandler::get_each_frame_texture_rect() const
{
	return m_each_frame_texture_rect;
}

void AnimationHandler::SetEachFrameSizeRect(const sf::IntRect& int_rect)
{
	m_each_frame_size_rect = int_rect;
}