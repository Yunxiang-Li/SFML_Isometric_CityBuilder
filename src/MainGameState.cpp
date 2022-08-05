#include <fstream>
#include <iostream>
#include "MainGameState.hpp"

MainGameState::MainGameState(std::shared_ptr<Game> game_ptr)
{
	// Store the game pointer.
	m_game_ptr = std::move(game_ptr);
	// Set main game view and GUI view's size.
	sf::Vector2f game_view_size = sf::Vector2f(m_game_ptr->m_game_window.getSize());
	m_view.setSize(game_view_size);
	m_gui_view.setSize(game_view_size);
	// Reset main menu view and GUI view's size and center
	m_view.setCenter(game_view_size * 0.5f);
	m_gui_view.setCenter(game_view_size * 0.5f);

	// Store a 64x64 game map from file city_map.dat.
	m_game_map = Map("../resources/binary/city_map.dat", 64, 64, m_game_ptr->m_str_tile_map);
	// Initialize the zoom level.
	m_zoom_level = 1.f;
	// Center the camera on the isometric map.
	sf::Vector2f camera_center(m_game_map.m_width, m_game_map.m_height * 0.5);
	camera_center *= static_cast<float>(m_game_map.m_tile_half_width);
	m_view.setCenter(camera_center);

	// Initialize the action state to NONE.
	m_action_state = GameActionEnum::NONE;
}

void MainGameState::render(const float dt)
{
	// Clear previous content and draw the background in the gui view.
	m_game_ptr->m_game_window.clear(sf::Color::Black);
	m_game_ptr->m_game_window.setView(m_gui_view);
	m_game_ptr->m_game_window.draw(m_game_ptr->m_background_sprite);

	// Then draw the game map in the main game view.
	m_game_ptr->m_game_window.setView(m_view);
	m_game_map.render(m_game_ptr->m_game_window, dt);
}

void MainGameState::update(const float dt)
{

}

void MainGameState::inputProcess()
{
	sf::Event event;
	// Check if there is a pending sf::Event object.
	while (m_game_ptr->m_game_window.pollEvent(event))
	{
		// Check the event type.
		switch (event.type)
		{
		// If user click the top right close button, then close the game window.
		case sf::Event::Closed:
		{
			m_game_ptr->m_game_window.close();
			break;;
		}
		// If user resize the window size.
		case sf::Event::Resized:
		{
			// Reset main menu view and GUI view's size.
			m_view.setSize(event.size.width, event.size.height);
			m_gui_view.setSize(event.size.width, event.size.height);
			// Zoom main game view with previously stored zoom level since resize event will reset zoom level to 1.
			m_view.zoom(m_zoom_level);
			// Set background sprite's position to window position (0, 0) related world position inside GUI view.
			m_game_ptr->m_background_sprite.setPosition(m_game_ptr->m_game_window.mapPixelToCoords(sf::Vector2i(0,
					0),
				m_gui_view));
			// Set background sprite to fill the entire window.
			m_game_ptr->m_background_sprite.setScale(
				float(event.size.width) / float(m_game_ptr->m_background_sprite.getTexture()->getSize().x),
				float(event.size.height) / float(m_game_ptr->m_background_sprite.getTexture()->getSize().y));
			break;
		}
		// If user pressed a specific key then handle it.
		case sf::Event::KeyPressed:
		{
			// If user pressed escape key, then close the game window.
			if (event.key.code == sf::Keyboard::Escape)
				m_game_ptr->m_game_window.close();
			break;
		}
		// When mouse moved, pan the camera.
		case sf::Event::MouseMoved:
		{
			// Only process when player keep pressing the mouse middle button.
			if (m_action_state == GameActionEnum::CAMERA_PANNING)
			{
				sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(m_game_ptr->m_game_window) -
					m_prev_mouse_pos);
				/* Main game view should move towards the opposite direction according to the zoom level since moving
				 * a camera to the right is the same as moving everything else to the left, */
				m_view.move(-1.f * pos * m_zoom_level);
				// Update mouse position.
				m_prev_mouse_pos = sf::Mouse::getPosition(m_game_ptr->m_game_window);
			}
			break;
		}
		// Check mouse button pressed cases.
		case sf::Event::MouseButtonPressed:
		{
			// Set current game state to camera panning when mouse middle button is pressed for the first time.
			if (event.mouseButton.button == sf::Mouse::Middle)
				if (m_action_state != GameActionEnum::CAMERA_PANNING)
				{
					m_action_state = GameActionEnum::CAMERA_PANNING;
				}
			break;
		}
		// Check mouse button released cases.
		case sf::Event::MouseButtonReleased:
		{
			// If the mouse middle button is released then reset the current game state.
			if (event.mouseButton.button == sf::Mouse::Middle)
				m_action_state = GameActionEnum::NONE;
			break;
		}
		// Check mouse wheel scroll cases.
		case sf::Event::MouseWheelScrolled:
		{
			// If mouse wheel scroll downward, double the zoom level of game view(view bigger, object smaller).
			if (event.mouseWheelScroll.delta < 0)
			{
				m_view.zoom(2.f);
				m_zoom_level *= 2.f;
			}
			// If mouse wheel move upward, halve the zoom level of game view(view smaller, object bigger).
			else
			{
				m_view.zoom(0.5f);
				m_zoom_level *= 0.5f;
			}
			break;
		}
		default:
			break;
		}
	}
}

