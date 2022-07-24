#pragma once
#ifndef ANIMATION_HPP
#define ANIMATION_HPP

/**
 * This class represents one animation.
 */
class Animation
{
 public:
	/**
	 * Constructor of Animation class.
	 * @param start_frame An unsigned integer indicates the animation's start frame.
	 * @param end_frame An unsigned integer indicates the animation's end frame.
	 * @param each_frame_duration A float indicates each frame 's duration.
	 */
	Animation(unsigned int start_frame, unsigned int end_frame, float each_frame_duration)
	{
		m_start_frame = start_frame;
		m_end_frame = end_frame;
		m_duration = each_frame_duration;
	}

	/**
	 * Get the total frame length between start and end frame.
	 * @return  An unsigned integer indicates the animation's total frame length.
	 */
	unsigned int get_total_frame() const;

	// Members that store each animation's start frame, end frame and animation duration.
	unsigned int m_start_frame;
	unsigned int m_end_frame;
	float m_duration;
};

#endif //ANIMATION_HPP
