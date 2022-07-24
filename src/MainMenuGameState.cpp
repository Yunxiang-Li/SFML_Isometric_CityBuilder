#include "MainMenuGameState.hpp"
#include "MainGameState.hpp"

MainMenuGameState::MainMenuGameState(std::shared_ptr<Game> game_ptr)
{
	// Store the game pointer.
	m_game_ptr = game_ptr;
	// Set game view's size to be same as current window's size.
	sf::Vector2f main_menu_view_size = sf::Vector2f(m_game_ptr->m_game_window.getSize());
	m_game_view.setSize(main_menu_view_size);

	// Set game view's center(origin of all transformations) to be the center position of the window(640x360 here).
	main_menu_view_size *= 0.5f;
	m_game_view.setCenter(main_menu_view_size);
}

void MainMenuGameState::render(const float dt)
{
	// Set game window's view to be game view, clear previous content and render game background sprite inside game view.
	m_game_ptr->m_game_window.setView(m_game_view);
	m_game_ptr->m_game_window.clear(sf::Color::Black);
	m_game_ptr->m_game_window.draw(m_game_ptr->m_background_sprite);
}

void MainMenuGameState::update(const float dt)
{

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
				// Reset game view's size and center.
				m_game_view.setSize(event.size.width, event.size.height);
			  	m_game_view.setCenter(event.size.width * 0.5f, event.size.height * 0.5);
				// Set background sprite's position to window position (0, 0) related world position inside default(main menu) view.
				m_game_ptr->m_background_sprite.setPosition(m_game_ptr->m_game_window.mapPixelToCoords(sf::Vector2i(0, 0)));
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

void MainMenuGameState::loadGame()
{
  // Create a MainGameState object and push it into the state stack.
  MainGameState main_menu_game_state(m_game_ptr);
  m_game_ptr->pushState(std::make_unique<MainGameState>(main_menu_game_state));
}