#include "Animation.hpp"

unsigned int Animation::get_total_frame() const
{
	return (m_end_frame - m_start_frame + 1);
}

