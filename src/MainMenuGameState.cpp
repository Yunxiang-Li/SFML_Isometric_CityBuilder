#include "MainMenuGameState.hpp"

void MainMenuGameState::render(const float dt)
{
	// Set game window's view to be game view, clear previous content and render stuff inside game view.
	m_game_ptr->m_game_window.setView(m_game_view);
	m_game_ptr->m_game_window.clear(sf::Color::Black);
	//m_game_ptr->m_game_window.draw(m_game_ptr->m_background);
}

void MainMenuGameState::inputProcess()
{
	sf::Event event;
	// Check if there is a pending sf::Event object.
	while (m_game_ptr->m_game_window.pollEvent(event))
	{
		// Check the event type.
		switch(event.type)
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
				// Reset game view's size.
				m_game_view.setSize(event.size.width, event.size.height);
				// Convert and set background position from window coordinates (0, 0) to world coordinates.
				m_game_ptr->m_background.setPosition(m_game_ptr->m_game_window.mapPixelToCoords(sf::Vector2i(0, 0)));
				// Set background to fill the entire window.
				m_game_ptr->m_background.setScale(
					float(event.size.width) / float(m_game_ptr->m_background.getTexture()->getSize().x),
					float(event.size.height) / float(m_game_ptr->m_background.getTexture()->getSize().y));
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
			default:
				break;
		}
	}
}

MainMenuGameState::MainMenuGameState(std::unique_ptr<Game> game_ptr)
{
	// Store the game pointer.
	m_game_ptr = std::move(game_ptr);
	// Set game view's size to be same as current window's size.
	sf::Vector2f main_menu_view_size = sf::Vector2f(m_game_ptr->m_game_window.getSize());
	m_game_view.setSize(main_menu_view_size);

	// Set game view's center to be the center of the window's size.
	main_menu_view_size *= 0.5f;
	m_game_view.setCenter(main_menu_view_size);
}

void MainMenuGameState::update(const float dt)
{

}