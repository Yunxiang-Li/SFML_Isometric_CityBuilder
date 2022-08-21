#include "Gui.hpp"
#include <assert.h>

sf::Vector2f Gui::get_gui_size() const
{
	return sf::Vector2f(m_entry_shape_dimension.x, m_entry_shape_dimension.y * m_Gui_entry_vec.size());
}

int Gui::get_gui_entry_idx(const sf::Vector2f& mouse_pos) const
{
	// If no GuiEntry objects exist, or the GUI is not visible, then return -1.
	if (m_Gui_entry_vec.empty())
		return -1;
	if (m_is_visible == false)
		return -1;

	// Traverse each GuiEntry object.
	for (size_t i = 0; i < m_Gui_entry_vec.size(); ++i)
	{
		// Initialize relative position with original mouse position.
		sf::Vector2f relative_pos(mouse_pos);

		// Convert mouse position from GUI view's screen coordinate into each GuiEntry object's local coordinate.

		/* The origin of GuiEntry object's shape defines the local point for all transformations. Default value is
		 * (0, 0) but can be updated. Therefore, we add this value into current mouse position as offset. Now we are
         * comparing between origin.x to (origin.x + width) and origin.y to (origin.y + height). */
		relative_pos += m_Gui_entry_vec[i].m_entry_shape.getOrigin();
		// Remove position offset since we only want to compare between 0 to width and 0 to height.
		relative_pos -= m_Gui_entry_vec[i].m_entry_shape.getPosition();

		// Ignore all positions that are not within the relative GuiEntry object's shape.
		if (relative_pos.x < 0 || relative_pos.x > (m_Gui_entry_vec[i].m_entry_shape.getScale().x *
		m_entry_shape_dimension.x))
			continue;
		if (relative_pos.y < 0 || relative_pos.y > (m_Gui_entry_vec[i].m_entry_shape.getScale().y *
		m_entry_shape_dimension.y))
			continue;
		return i;
	}

	// If not find, then return -1.
	return -1;
}

void Gui::set_gui_entry_text(int entry_idx, const std::string& entry_text)
{
	// Check if entry_idx is valid or not.
	if (entry_idx < 0 || entry_idx >= m_Gui_entry_vec.size())
		return;

	// Set the text content.
	m_Gui_entry_vec[entry_idx].m_text.setString(entry_text);
}

void Gui::set_each_gui_entry_dimension(const sf::Vector2f& dimension)
{
	// Store the new dimension.
	m_entry_shape_dimension = dimension;

	// Update each GuiEntry object's shape's dimension and text's character size.
	for (auto& each_entry : m_Gui_entry_vec)
	{
		each_entry.m_entry_shape.setSize(dimension);
		each_entry.m_text.setCharacterSize(m_entry_shape_dimension.y - m_gui_style_ptr->m_outline_size -
		m_text_padding);
	}
}

void Gui::draw(sf::RenderTarget& render_target, sf::RenderStates render_states) const
{
	// Check if GUI is visible or not.
	if (m_is_visible == false)
		return;

	// Draw each GuiEntry object.
	for (auto& each_entry : m_Gui_entry_vec)
	{
		render_target.draw(each_entry.m_entry_shape);
		render_target.draw(each_entry.m_text);
	}
}

void Gui::show()
{
	m_is_visible = true;

	// Used to place each GuiEntry object at the correct position.
	sf::Vector2f origin_offset(0.f, 0.f);

	// Get and store GUI's local origin.
	sf::Vector2f Gui_origin(this->getOrigin());

	// Traverse through each GuiEntry object.
	for (auto& each_entry : m_Gui_entry_vec)
	{
		// Get current GuiEntry object's local origin. This value's absolute position is always fixed.
		sf::Vector2f curr_entry_origin(Gui_origin - origin_offset);

		// Set current GuiEntry object's shape and text's local origins.
		each_entry.m_entry_shape.setOrigin(curr_entry_origin);
		each_entry.m_text.setOrigin(curr_entry_origin);

		// Set current GuiEntry object's shape and text's positions.
		each_entry.m_entry_shape.setPosition(this->getPosition());
		each_entry.m_text.setPosition(this->getPosition());

		// Update offset according to Gui's layout(horizontally or vertically).
		if (m_is_horizontal)
			origin_offset.x += m_entry_shape_dimension.x;
		else
			origin_offset.y += m_entry_shape_dimension.y;
	}
}

void Gui::hide()
{
	m_is_visible = false;
}

void Gui::highlight_entry(const unsigned int entry_idx)
{
	for (size_t i = 0; i < m_Gui_entry_vec.size(); ++i)
	{
		// When find the specified GuiEntry object, then use highlighted colors.
		if (i == entry_idx)
		{
			m_Gui_entry_vec[i].m_entry_shape.setFillColor(m_gui_style_ptr->m_background_highlight_color);
			m_Gui_entry_vec[i].m_entry_shape.setOutlineColor(m_gui_style_ptr->m_outline_highlight_color);
			m_Gui_entry_vec[i].m_text.setFillColor(m_gui_style_ptr->m_text_highlight_color);
		}
		// Otherwise, use normal colors.
		else
		{
			m_Gui_entry_vec[i].m_entry_shape.setFillColor(m_gui_style_ptr->m_background_color);
			m_Gui_entry_vec[i].m_entry_shape.setOutlineColor(m_gui_style_ptr->m_outline_color);
			m_Gui_entry_vec[i].m_text.setFillColor(m_gui_style_ptr->m_text_color);
		}
	}
}

std::string Gui::get_entry_msg(const int entry_idx) const
{
	if (entry_idx == -1)
		return "NULL";
	return (m_Gui_entry_vec[entry_idx].m_activated_msg);
}

std::string Gui::get_mouse_pos_entry_msg(const sf::Vector2f& mouse_pos) const
{
	const int entry_idx = this->get_gui_entry_idx(mouse_pos);
	return this->get_entry_msg(entry_idx);
}