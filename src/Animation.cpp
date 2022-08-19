#include "Animation.hpp"

unsigned int Animation::getTotalFrame() const
{
	return (m_end_frame - m_start_frame + 1);
}

float Animation::getDuration() const
{
	return m_duration;
}