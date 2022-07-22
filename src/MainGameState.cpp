#include "MainGameState.hpp"

void MainGameState::render(const float dt)
{
	// Clear previous content and draw the background.
	m_game_ptr->m_game_window.clear(sf::Color::Black);
	//m_game_ptr->m_game_window.draw(m_game_ptr->m_background);
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
				// Set main menu view to new window's size.
				m_main_game_view.setSize(event.size.width, event.size.height);
				m_gui_view.setSize(event.size.width, event.size.height);
				// Convert and set background position from window coordinates (0, 0) to world coordinates using GUI view.
				m_game_ptr->m_background.setPosition(m_game_ptr->m_game_window.mapPixelToCoords(sf::Vector2i(0, 0), m_gui_view));
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
				// If user pressed space key, then load the main game scene.
				else if (event.key.code == sf::Keyboard::Space)
					this->loadGame();
				break;
			}
			default:
				break;
		}
	}
}

MainGameState::MainGameState(std::unique_ptr<Game> game_ptr)
{
	// Store the game pointer.
	m_game_ptr = std::move(game_ptr);
	// Set main game view and GUI view's size.
	sf::Vector2f game_view_size = sf::Vector2f(m_game_ptr->m_game_window.getSize());
	m_main_game_view.setSize(game_view_size);
	m_gui_view.setSize(game_view_size);

	// Set main game view and GUI view's center to be the center of the window's size.
	game_view_size *= 0.5f;
	m_main_game_view.setCenter(game_view_size);
	m_gui_view.setCenter(game_view_size);
}

void MainGameState::loadGame()
{
	m_game_ptr->pushState(std::make_unique<MainGameState>(std::move(m_game_ptr)));
}

