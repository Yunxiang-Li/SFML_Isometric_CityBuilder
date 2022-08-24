#include <fstream>
#include <iostream>
#include "MainGameState.hpp"
#include "Utility.hpp"

MainGameState::MainGameState(std::shared_ptr<Game> game_ptr) : m_action_state(GameActionEnum::NONE)
{
	// Store the game pointer.
	this->set_game_ptr(game_ptr);
	// Set main game view and GUI view's size.
	sf::Vector2f game_view_size(sf::Vector2f(this->get_game_ptr()->m_game_window.getSize()));
	m_view.setSize(game_view_size);
	m_gui_view.setSize(game_view_size);
	// Reset main menu view and GUI view's size and center
	m_view.setCenter(game_view_size * 0.5f);
	m_gui_view.setCenter(game_view_size * 0.5f);

	// Store a 64x64 game map from file city_map.dat.
	m_game_map_ptr = std::move(std::make_shared<Map>(Map(MAP_BINARY_FILE_PATH,
		GAME_MAP_WIDTH,GAME_MAP_HEIGHT, this->get_game_ptr()->m_str_tile_map)));
	// Center the camera on the isometric map.
	sf::Vector2f camera_center(m_game_map_ptr->get_width(), m_game_map_ptr->get_height() * 0.5);
	camera_center *= static_cast<float>(m_game_map_ptr->get_tile_half_width());
	m_view.setCenter(camera_center);

	// Initialize current selected tile as a Grass tile.
	m_curr_selected_tile_ptr = std::make_shared<Tile>(this->get_game_ptr()->m_str_tile_map.at
		(GRASS_TILE_TEXTURE_NAME));
}

void MainGameState::render(const float dt)
{
	// Clear previous content and draw the background in the gui view.
	this->get_game_ptr()->m_game_window.clear(sf::Color::Black);
	this->get_game_ptr()->m_game_window.setView(m_gui_view);
	this->get_game_ptr()->m_game_window.draw(this->get_game_ptr()->m_background_sprite);

	// Then draw the game map in the main game view.
	this->get_game_ptr()->m_game_window.setView(m_view);
	m_game_map_ptr->render(this->get_game_ptr()->m_game_window, dt);
}

void MainGameState::update(const float dt)
{

}

void MainGameState::inputProcess()
{
	sf::Event event;
	// Check if there is a pending sf::Event object.
	while (this->get_game_ptr()->m_game_window.pollEvent(event))
	{
		// Check the event type.
		switch (event.type)
		{
		// If user click the top right close button, then close the game window.
		case sf::Event::Closed:
		{
			this->get_game_ptr()->m_game_window.close();
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
			this->get_game_ptr()->m_background_sprite.setPosition(this->get_game_ptr()->
			m_game_window.mapPixelToCoords(sf::Vector2i(0,
					0),
				m_gui_view));
			// Set background sprite to fill the entire window.
			this->get_game_ptr()->m_background_sprite.setScale(
				float(event.size.width) / float(this->get_game_ptr()->m_background_sprite.getTexture()
				->getSize().x),
				float(event.size.height) / float(this->get_game_ptr()->m_background_sprite.getTexture()
				->getSize().y));
			break;
		}
		// If user pressed a specific key then handle it.
		case sf::Event::KeyPressed:
		{
			// If user pressed escape key, then close the game window.
			if (event.key.code == sf::Keyboard::Escape)
				this->get_game_ptr()->m_game_window.close();
			break;
		}
		// Mouse moved condition.
		case sf::Event::MouseMoved:
		{
			// Pan the camera when player keep pressing the mouse middle button.
			if (m_action_state == GameActionEnum::CAMERA_PANNING)
			{
				sf::Vector2f pos(sf::Vector2f(sf::Mouse::getPosition(this->get_game_ptr()
				->m_game_window) - m_prev_mouse_pos));
				/* Main game view should move towards the opposite direction according to the zoom level since moving
				 * a camera to the right is the same as moving everything else to the left. */
				m_view.move(-1.f * pos * m_zoom_level);
				// Update mouse position.
				m_prev_mouse_pos = sf::Mouse::getPosition(this->get_game_ptr()->m_game_window);
			}
			// Select a rectangle of tile objects when player keep pressing the mouse left button.
			else if (m_action_state == GameActionEnum::TILE_SELECTING)
			{
				// Convert and store mouse's screen position to world position.
				sf::Vector2f mouse_pos(this->get_game_ptr()->m_game_window.mapPixelToCoords(sf::Mouse::getPosition
					(this->get_game_ptr()->m_game_window), m_view));
				/* Inverse of algebra formula inside Map::render function(change world coordinate to tile coordinate).
				 * Additional 0.5 is a compensation offset for integer truncation. */
				m_select_end_pos.x = (mouse_pos.y / m_game_map_ptr->get_tile_half_width()) +
					(mouse_pos.x / (2 * m_game_map_ptr->get_tile_half_width())) - (0.5 * m_game_map_ptr->get_width()) -
					INT_TRUNCATION_OFFSET;
				m_select_end_pos.y = (mouse_pos.y / m_game_map_ptr->get_tile_half_width()) -
					(mouse_pos.x / (2 * m_game_map_ptr->get_tile_half_width())) + (0.5 * m_game_map_ptr->get_width()) +
					INT_TRUNCATION_OFFSET;

				// Deselected all tiles first.
				m_game_map_ptr->deselect_tiles();

				/* If current selected tile is of grass type, then current player action is considered to destroy all
				 * exist tiles rather than grass and water tiles. Therefore, grass and water tiles are considered as
				 * black list and all other tiles should be selected. */
				if (m_curr_selected_tile_ptr->m_tileType == TileTypeEnum::GRASS)
					m_game_map_ptr->select(m_select_start_pos, m_select_end_pos, { TileTypeEnum::GRASS,
																				  TileTypeEnum::WATER});
				/* Else current player action is considered to build new tiles. And new tiles can only be built on the
				 * grass tiles. Therefore, all other 6 type of tiles are considered as black list. */
				else
					m_game_map_ptr->select(m_select_start_pos, m_select_end_pos,
						{TileTypeEnum::WATER, TileTypeEnum::ROAD, TileTypeEnum::FOREST,
						 TileTypeEnum::RESIDENTIAL, TileTypeEnum::COMMERCIAL, TileTypeEnum::INDUSTRIAL});
			}
			break;
		}
		// Check mouse button pressed cases.
		case sf::Event::MouseButtonPressed:
		{
			// when mouse middle button is pressed.
			if (event.mouseButton.button == sf::Mouse::Middle)
			{
				// Switch to the camera panning state.
				if (m_action_state != GameActionEnum::CAMERA_PANNING)
				{
					m_action_state = GameActionEnum::CAMERA_PANNING;
					// Store mouse's current position.
					m_prev_mouse_pos = sf::Mouse::getPosition(this->get_game_ptr()->m_game_window);
				}
			}
			// When mouse left button is pressed.
			else if (event.mouseButton.button == sf::Mouse::Left)
			{
				// Switch to the tile select state.
				if (m_action_state != GameActionEnum::TILE_SELECTING)
				{
					m_action_state = GameActionEnum::TILE_SELECTING;
					// Convert and store mouse's screen position to world position.
					sf::Vector2f mouse_pos(this->get_game_ptr()->m_game_window.mapPixelToCoords
					(sf::Mouse::getPosition(this->get_game_ptr()->m_game_window), m_view));
					/* Inverse of algebra formula inside Map::render function(change world coordinate to tile coordinate).
					 * Additional 0.5 is a compensation offset for integer truncation. */
					m_select_start_pos.x = (mouse_pos.y / m_game_map_ptr->get_tile_half_width()) +
						(mouse_pos.x / (2 * m_game_map_ptr->get_tile_half_width())) -
						(0.5 * m_game_map_ptr->get_width()) - 0.5;
					m_select_start_pos.y = (mouse_pos.y / m_game_map_ptr->get_tile_half_width()) -
						(mouse_pos.x / (2 * m_game_map_ptr->get_tile_half_width())) +
						(0.5 * m_game_map_ptr->get_width()) + 0.5;
				}
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				// Switch to the none state and de-select all exist tiles.
				if (m_action_state == GameActionEnum::TILE_SELECTING)
				{
					m_action_state = GameActionEnum::NONE;
					m_game_map_ptr->deselect_tiles();
				}
			}
			break;
		}
		// Check mouse button released cases.
		case sf::Event::MouseButtonReleased:
		{
			// If the mouse middle button is released then reset the current game state.
			if (event.mouseButton.button == sf::Mouse::Middle)
				m_action_state = GameActionEnum::NONE;
			else if (event.mouseButton.button == sf::Mouse::Left)
			{
				// Switch to the none state and de-select all exist tiles.
				if (m_action_state == GameActionEnum::TILE_SELECTING)
				{
					m_action_state = GameActionEnum::NONE;
					m_game_map_ptr->deselect_tiles();
				}
			}
			break;
		}
		// Check mouse wheel scroll cases.
		case sf::Event::MouseWheelScrolled:
		{
			// If mouse wheel scroll downward, double the zoom level of game view(view bigger, object smaller).
			if (event.mouseWheelScroll.delta < 0)
			{
				m_view.zoom(DOWNWARD_SCROLL_FACTOR);
				m_zoom_level *= DOWNWARD_SCROLL_FACTOR;
			}
			// If mouse wheel move upward, halve the zoom level of game view(view smaller, object bigger).
			else
			{
				m_view.zoom(UPWARD_SCROLL_FACTOR);
				m_zoom_level *= UPWARD_SCROLL_FACTOR;
			}
			break;
		}
		default:
			break;
		}
	}
}

