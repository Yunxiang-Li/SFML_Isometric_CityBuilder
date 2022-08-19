#include "MainMenuGameState.hpp"
#include "MainGameState.hpp"
#include "Gui.hpp"

MainMenuGameState::MainMenuGameState(std::shared_ptr<Game> game_ptr)
{
	// Store the game pointer.
	m_game_ptr = game_ptr;
	// Set game view's size to be same as current window's size.
	sf::Vector2f main_menu_view_size(sf::Vector2f(m_game_ptr->m_game_window.getSize()));
	m_view.setSize(main_menu_view_size);

	// Set game view's center(origin of all transformations) to be the center position of the window(640x360 here).
	sf::Vector2f half_main_menu_view_size(main_menu_view_size/2.f);
	m_view.setCenter(half_main_menu_view_size);

	// Create and store the new Gui object.
	m_str_Gui_map.emplace("menu", std::make_shared<Gui>(Gui(sf::Vector2f(192, 32), 4,
		false, *(m_game_ptr->getGuiStylePtr("button")),
		{std::make_pair("Load Game", "message:load_game")})));

	// Set Gui at the center position of the view.
	m_str_Gui_map.at("menu")->setPosition(half_main_menu_view_size);
	// Set Gui's origin to its center.
	m_str_Gui_map.at("menu")->setOrigin(96, 32*1/2);
	// Mark the Gui visible and place all the entries in the correct location.
	m_str_Gui_map.at("menu")->show();
}

void MainMenuGameState::render(const float dt)
{
	// Set game window's view to be game view, clear previous content and render game background sprite inside game view.
	m_game_ptr->m_game_window.setView(m_view);
	m_game_ptr->m_game_window.clear(sf::Color::Black);
	m_game_ptr->m_game_window.draw(m_game_ptr->m_background_sprite);

	// Render each object.
	for (const auto& gui : m_str_Gui_map)
		m_game_ptr->m_game_window.draw(*gui.second);
}

void MainMenuGameState::update(const float dt)
{

}

void MainMenuGameState::inputProcess()
{
	sf::Event event;
	// Transfer current mouse position from screen coordinate to world coordinate.
	sf::Vector2f mouse_pos = m_game_ptr->m_game_window.mapPixelToCoords(sf::Mouse::getPosition
		(m_game_ptr->m_game_window), m_view);

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
			// Store the new view dimension.
			sf::Vector2f new_view_size = sf::Vector2f(event.size.width, event.size.height);
			// Reset game view's size and center.
			m_view.setSize(new_view_size);

			// Set background sprite's position to window position (0, 0) related world position inside default(main menu) view.
			m_game_ptr->m_background_sprite.setPosition(m_game_ptr->m_game_window.mapPixelToCoords(sf::Vector2i(0,
				0), m_view));

			// Get half of new view dimension.
			sf::Vector2f new_half_view_pos(new_view_size/2.f);
			// Transfer from screen position to world position.
			new_half_view_pos = m_game_ptr->m_game_window.mapPixelToCoords(sf::Vector2i
				(new_half_view_pos), m_view);
			// Set specified Gui at the center of the screen again.
			m_str_Gui_map.at("menu")->setPosition(new_half_view_pos);

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
		// Highlight mouse hover GuiEntry object.
		case sf::Event::MouseMoved:
		{
			m_str_Gui_map.at("menu")->highlight_entry(m_str_Gui_map.at("menu")->get_gui_entry_idx(mouse_pos));
		}
		// Handle left mouse button pressed case.
		case sf::Event::MouseButtonPressed:
		{
			// Check if user press left mouse button.
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				// Load game if user press the "load game" GuiEntry object.
				std::string msg = m_str_Gui_map.at("menu")->get_mouse_pos_entry_msg(mouse_pos);
				if (msg == "message:load_game")
					this->loadGame();
			}
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
	m_game_ptr->push_state(std::make_unique<MainGameState>(main_menu_game_state));
}