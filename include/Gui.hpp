#pragma once
#ifndef GUI_HPP
#define GUI_HPP

#include "GuiEntry.hpp"
#include "GuiStyle.hpp"
#include <vector>

/**
 * The main part of our GUI. Inherits from both sf::Transformable and sf::Drawable classes which allows our GUI to be
 * moved and rendered.
 */
 class Gui : public sf::Transformable, public sf::Drawable
{
  public:
	/**
	 * Five parameters' Constructor.
	 * @param entry_shape_dimension A sf::Vector2f object indicates the Gui object's shape dimension.
	 * @param text_padding A unsigned integer indicates the padding size of Gui object's text contents.
	 * @param is_horizontal A bool indicates if the Gui object is displayed horizontally(if not, then vertically).
	 * @param gui_style A reference of const GuiStyle object indicates the current Gui object's style.
	 * @param entries_text_msg_vec A vector of pair of string, first element indicates the related Gui Entry object's
	 * name, second element indicates this Gui Entry object's activated message.
	 */
	Gui(sf::Vector2f entry_shape_dimension, unsigned int text_padding, bool is_horizontal, const GuiStyle& gui_style,
		std::vector<std::pair<std::string, std::string>> entries_text_msg_vec) :m_is_visible(false),
		m_is_horizontal(is_horizontal), m_gui_style(gui_style), m_entry_shape_dimension(entry_shape_dimension),
		m_text_padding(text_padding)
	{
		// Set up Gui entry shape's properties.
		sf::RectangleShape gui_entry_shape;
		gui_entry_shape.setSize(m_entry_shape_dimension);
		gui_entry_shape.setFillColor(m_gui_style.m_background_color);
		// Outline expands toward the inner part of the shape.
		gui_entry_shape.setOutlineThickness(-m_gui_style.m_outline_size);
		gui_entry_shape.setOutlineColor(m_gui_style.m_outline_color);

		// Traverse each pair inside entries_text_msg_vec.
		for (const auto& each_pair : entries_text_msg_vec)
		{
			// Set up each Gui entry's text.
			sf::Text gui_entry_text;
			gui_entry_text.setString(each_pair.first);
			gui_entry_text.setFont(*(m_gui_style.m_font_ptr));
			gui_entry_text.setFillColor(m_gui_style.m_text_color);
			gui_entry_text.setCharacterSize(m_entry_shape_dimension.y - m_gui_style.m_outline_size - m_text_padding);

			// Store each GuiEntry object.
			m_Gui_entry_vec.emplace_back(GuiEntry(each_pair.second, gui_entry_shape, gui_entry_text));
		}
	}

	/**
	 * Get the gui's total size. First value(x) means the width of m_entry_shape_dimension, second value(y) means the
	 * multiplication of m_entry_shape_dimension's height and amount of GuiEntry objects.
	 * @return A sf::Vector2f object indicates the gui's total size.
	 */
	sf::Vector2f get_gui_size() const;

	/**
	 * If one GuiEntry object is hovered over by mouse, returns its index. Otherwise, return -1.
	 * @param mouse_pos A reference of const sf::Vector2f object indicates mouse position in the screen coordinates for
	 * the sf::View object that the Gui is displayed on.
	 * @return A non-negative integer indicates the index of current hovered GuiEntry object or -1 if no GuiEntry
	 * object is hovered over.
	 */
	int get_gui_entry_idx(const sf::Vector2f& mouse_pos) const;

	/**
	 * Set entry_idxth GuiEntry object's text.
	 * @param entry_idx An integer indicates the specified GuiEntry object's index(may be invalid).
	 * @param entry_text A reference of const std::string indicates the related GuiEntry object's text content.
	 */
	void set_gui_entry_text(int entry_idx, const std::string& entry_text);

	/**
	 * Modify each GuiEntry object's dimension(width and height).
	 * @param dimension A reference of sf::Vector2f object indicates each GuiEntry object's dimension.
	 */
	void set_each_gui_entry_dimension(const sf::Vector2f& dimension);

	/**
	 * Override function from sf::Drawable. Draw the object to a render target.
	 * @param render_target A reference of sf::RenderTarget object indicates render target to draw to.
	 * @param render_states A sf::RenderStates object indicates current render states.
	 */
	virtual void draw(sf::RenderTarget& render_target, sf::RenderStates render_states) const override;

	/**
	 * Update each GuiEntry object and set visibility to true.
	 */
	void show();

	/**
	 * Set visibility to false.
	 */
	void hide();

	/**
	 * Highlight specified GuiEntry object.
	 * @param entry_idx A const unsigned integer indicates the index of specified GuiEntry object.
	 */
	void highlight_entry(const unsigned int entry_idx);

	/**
	 * Return the activated message of specified GuiEntry object if exist. Otherwise, return "NULL".
	 * @param entry_idx A const integer indicates the index of specified GuiEntry object if exist. -1 means such
	 * GuiEntry object does not exist.
	 * @return A std::string indicates the message content of specified GuiEntry object if exist. Otherwise, return
	 * "NULL".
	 */
	std::string get_entry_msg(const int entry_idx) const;

	/**
	 * Return the activated message of specified GuiEntry object if exist. Otherwise, return "NULL".
	 * @param mouse_pos A reference of const sf::Vector2f object indicates the current mouse position.
	 * @return A std::string indicates the message content of specified GuiEntry object if exist. Otherwise, return
	 * "NULL".
	 */
	std::string get_mouse_pos_entry_msg(const sf::Vector2f& mouse_pos) const;

  private:
	// A vector of multiple GuiEntry objects.
	std::vector<GuiEntry> m_Gui_entry_vec;
	// A bool indicates if the Gui should be visible or not.
	bool m_is_visible{false};
	// Indicates if the menu entry is horizontal or not. If not, then vertically.
	bool m_is_horizontal{true};
  	// A GuiStyle object indicates the style(size, color, font) of our Gui.
  	GuiStyle m_gui_style;
	// Indicates the Gui entry shape's width and height(dimension).
	sf::Vector2f m_entry_shape_dimension;
	// Margin that surrounds the text to stop it from overlapping the edges in pixels.
	unsigned int m_text_padding{0};
};

#endif //GUI_HPP
