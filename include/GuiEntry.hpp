#pragma once
#ifndef GUIENTRY_HPP
#define GUIENTRY_HPP

#include "SFML/Graphics.hpp"
#include <string>

/**
 * This class represents the entry part of the GUI.
 */
class GuiEntry
{
 public:
	/**
	 * Default constructor.
	 */
	GuiEntry() = default;

	/**
	 * Three parameters' constructor.
	 * @param activated_msg A reference of const string indicates the message when the Gui entry is activated.
	 * @param entry_shape A reference of sf::RectangleShape indicates the Gui entry's appearance.
	 * @param text A reference of sf::Text object indicates the text displayed on the Gui entry scene.
	 */
	GuiEntry(const std::string& activated_msg, sf::RectangleShape& entry_shape, sf::Text& text) :
		m_activated_msg(activated_msg), m_entry_shape(entry_shape), m_text(text) {};

	// Message(string) returned when the Gui entry is activated.
	std::string m_activated_msg;
	// Store the GUI entry's appearance, can be rendered like a sf::Sprite.
	sf::RectangleShape m_entry_shape;
	// A sf::Text object indicates each GuiEntry object's content displayed on the Gui entry scene.
 	sf::Text m_text;
};
#endif //GUIENTRY_HPP
