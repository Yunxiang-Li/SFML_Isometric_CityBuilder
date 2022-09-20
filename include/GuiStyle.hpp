#pragma once
#ifndef GUISTYLE_HPP
#define GUISTYLE_HPP

#include "SFML/Graphics.hpp"
#include <memory>
#include <utility>

/**
 * This class represents the style(size, color, font) of our GUI.
 */
class GuiStyle
{
 public:
	/**
	 * Default constructor.
	 */
	GuiStyle() = default;

	/**
	 * Eight parameters' constructor.
	 * @param font_ptr A std::shared_ptr<sf::Font> points to the font object used for gui.
	 * @param outline_size A float indicates the size of GUI entry's outline.
	 * @param background_color A sf::Color object indicates the color of GUI entry's background.
	 * @param outline_color A sf::Color object indicates the color of GUI entry's outline.
	 * @param text_color A sf::Color object indicates the color of GUI entry's text.
	 * @param background_highlight_color A sf::Color object indicates the color of GUI entry's background when
	 * highlighted.
	 * @param outline_highlight_color A sf::Color object indicates the color of GUI entry's outline when highlighted.
	 * @param text_highlight_color A sf::Color object indicates the color of GUI entry's text when highlighted.
	 */
	GuiStyle(std::shared_ptr<sf::Font> font_ptr, float outline_size, sf::Color background_color,
		sf::Color outline_color, sf::Color text_color, sf::Color background_highlight_color,
		sf::Color outline_highlight_color, sf::Color text_highlight_color) : m_font_ptr(std::move(font_ptr)),
		m_outline_size(outline_size), m_background_color(background_color), m_outline_color(outline_color),
		m_text_color(text_color), m_background_highlight_color(background_highlight_color),
		m_outline_highlight_color(outline_highlight_color), m_text_highlight_color(text_highlight_color){};

	std::shared_ptr<sf::Font> m_font_ptr;
	float m_outline_size{0.f};
	sf::Color m_background_color;
	sf::Color m_outline_color;
	sf::Color m_text_color;
	sf::Color m_background_highlight_color;
	sf::Color m_outline_highlight_color;
	sf::Color m_text_highlight_color;
};

#endif //GUISTYLE_HPP
