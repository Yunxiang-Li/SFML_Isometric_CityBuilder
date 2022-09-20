#include <fstream>
#include <iostream>
#include "MainGameState.hpp"
#include "Utility.hpp"
#include "City.hpp"

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

	// Create and store a 64x64 game city.
	m_game_city_ptr = std::make_shared<City>(City(CITY_NAME_STR,
		game_ptr->m_tile_half_width_pixel,game_ptr->m_str_tile_map));
	// Shuffle the city to allow update in random order later.
	m_game_city_ptr->shuffleTiles();

	/* Create and store right click menu Gui object. Display when the player presses the right mouse button.
	 * All possible tiles that can be placed will be shown. After player choose one, that will also be the current
	 * selected tile.*/
	m_str_gui_map.emplace(MENU_GUI_STR, Gui(sf::Vector2f(MENU_GUI_ENTRY_SHAPE_WIDTH,
		MENU_GUI_ENTRY_SHAPE_HEIGHT),MENU_GUI_TEXT_PADDING, false,
			(*this->get_game_ptr()->getGuiStylePtr(BUTTON_NAME)),
			{
				std::make_pair(GRASS_GUI_ENTRY_NAME, GRASS_GUI_ENTRY_MSG),
				std::make_pair(WATER_GUI_ENTRY_NAME, WATER_GUI_ENTRY_MSG),
				std::make_pair(FOREST_GUI_ENTRY_NAME, FOREST_GUI_ENTRY_MSG),
				std::make_pair(RESIDENTIAL_GUI_ENTRY_NAME, RESIDENTIAL_GUI_ENTRY_MSG),
				std::make_pair(COMMERCIAL_GUI_ENTRY_NAME, COMMERCIAL_GUI_ENTRY_MSG),
				std::make_pair(INDUSTRIAL_GUI_ENTRY_NAME, INDUSTRIAL_GUI_ENTRY_MSG),
				std::make_pair(ROAD_GUI_ENTRY_NAME, ROAD_GUI_ENTRY_MSG),
			}));

	/* Create and store select cost text Gui object. Display the player-selected tiles' total cost.
	 * The color will be red if player does not have enough fund.*/
	m_str_gui_map.emplace(SELECT_TILE_COST_TEXT_GUI_STR, Gui(sf::Vector2f(
		SELECT_TILE_COST_TEXT_GUI_ENTRY_SHAPE_WIDTH, SELECT_TILE_COST_TEXT_GUI_ENTRY_SHAPE_HEIGHT), 0,
			false, (*this->get_game_ptr()->getGuiStylePtr(TEXT_NAME)),
			{std::make_pair("", "")}));

	/* Create and store the information bar Gui object. Located at the bottom of the screen, display the in-game days
	 * and other useful information for the player. */
	m_str_gui_map.emplace(INFO_BAR_GUI_STR, Gui(sf::Vector2f(INFO_BAR_GUI_ENTRY_SHAPE_WIDTH,
		INFO_BAR_GUI_ENTRY_SHAPE_HEIGHT), INFO_BAR_GUI_TEXT_PADDING, true,
			(*this->get_game_ptr()->getGuiStylePtr(BUTTON_NAME)),
			{
				std::make_pair(TIME_GUI_ENTRY_STR, TIME_GUI_ENTRY_MSG),
				std::make_pair(FUND_GUI_ENTRY_STR, FUND_GUI_ENTRY_MSG),
				std::make_pair(POPULATION_GUI_ENTRY_STR, POPULATION_GUI_ENTRY_MSG),
				std::make_pair(EMPLOYMENT_GUI_ENTRY_STR, EMPLOYMENT_GUI_ENTRY_MSG),
				std::make_pair(CURR_TILE_GUI_ENTRY_STR, CURR_TILE_GUI_ENTRY_MSG)
			}));

	// Center the camera on the isometric map.
	sf::Vector2f camera_center(m_game_city_ptr->get_map_ptr()->get_width(), m_game_city_ptr->get_map_ptr()->
	get_height() * 0.5);
	camera_center *= static_cast<float>(m_game_city_ptr->get_map_ptr()->get_tile_half_width());
	m_view.setCenter(camera_center);

	// Initialize current selected tile as a Grass tile.
	m_curr_selected_tile_ptr = std::make_shared<Tile>(this->get_game_ptr()->m_str_tile_map.at
		(GRASS_TILE_TEXTURE_NAME));
}

std::string tileTypeToStr(TileTypeEnum tile_type)
{
	// A string store the result.
	std::string res_str{};
	// Check each case, for unlisted input, use Void to represent.
	switch (tile_type)
	{
	case TileTypeEnum::INDUSTRIAL:
		res_str = INDUSTRIAL_TILE_TEXTURE_NAME;
		break;
	case TileTypeEnum::COMMERCIAL:
		res_str = COMMERCIAL_TILE_TEXTURE_NAME;
		break;
	case TileTypeEnum::RESIDENTIAL:
		res_str = RESIDENTIAL_TILE_TEXTURE_NAME;
		break;
	case TileTypeEnum::GRASS:
		res_str = GRASS_TILE_TEXTURE_NAME;
		break;
	case TileTypeEnum::FOREST:
		res_str = FOREST_TILE_TEXTURE_NAME;
		break;
	case TileTypeEnum::WATER:
		res_str = WATER_TILE_TEXTURE_NAME;
		break;
	case TileTypeEnum::ROAD:
		res_str = ROAD_TILE_TEXTURE_NAME;
		break;
	default:
		res_str = VOID_TILE_TEXTURE_NAME;
		break;
	}
	return res_str;
}

void MainGameState::render(const float dt)
{
	// Clear previous content and draw the background in the gui view.
	this->get_game_ptr()->m_game_window.clear(sf::Color::Black);
	this->get_game_ptr()->m_game_window.setView(m_gui_view);
	this->get_game_ptr()->m_game_window.draw(this->get_game_ptr()->m_background_sprite);

	// Draw the game map within the main game view.
	this->get_game_ptr()->m_game_window.setView(m_view);
	m_game_city_ptr->get_map_ptr()->render(this->get_game_ptr()->m_game_window, dt);

	// Then draw each Gui object within the gui game view.
	this->get_game_ptr()->m_game_window.setView(m_gui_view);
	for (const auto& pair : m_str_gui_map)
		this->get_game_ptr()->m_game_window.draw(pair.second);
}

void MainGameState::update(const float dt)
{
	// Update the whole game city.
	m_game_city_ptr->update(dt);

	// Set up information bar Gui's all 5 GuiEntry objects' texts(displayed at the bottom of the screen).
	m_str_gui_map.at(INFO_BAR_GUI_STR).set_gui_entry_text(0, "Day: " +
	std::to_string(m_game_city_ptr->get_day()));
	m_str_gui_map.at(INFO_BAR_GUI_STR).set_gui_entry_text(1, "Fund: $" +
	std::to_string(m_game_city_ptr->get_fund()));
	m_str_gui_map.at(INFO_BAR_GUI_STR).set_gui_entry_text(2, "Population:" +
		std::to_string(m_game_city_ptr->get_population()) + "(Homeless:" +
		std::to_string(m_game_city_ptr->getHomelessNum()) + ")");
	m_str_gui_map.at(INFO_BAR_GUI_STR).set_gui_entry_text(3, "(Employee:"+
	std::to_string(m_game_city_ptr->get_employable()) + "(Unemployed:" +
		std::to_string(m_game_city_ptr->getUnemployedNum()) + ")");
	m_str_gui_map.at(INFO_BAR_GUI_STR).set_gui_entry_text(4, "Selected Tile:" +
		tileTypeToStr(m_curr_selected_tile_ptr->m_tileType));

	// Highlight the mouse hovered GuiEntry object within right click menu Gui.
	m_str_gui_map.at(MENU_GUI_STR).highlight_entry(m_str_gui_map.at(MENU_GUI_STR).
	get_gui_entry_idx(this->get_game_ptr()->m_game_window.mapPixelToCoords
	(sf::Mouse::getPosition(this->get_game_ptr()->m_game_window), m_gui_view)));
}

void MainGameState::inputProcess()
{
	// Store mouse positions within main game view and gui view separately.
	sf::Vector2f mouse_pos_in_game_view(this->get_game_ptr()->m_game_window.mapPixelToCoords(
		sf::Mouse::getPosition(this->get_game_ptr()->m_game_window), m_view));
	sf::Vector2f mouse_pos_in_gui_view(this->get_game_ptr()->m_game_window.mapPixelToCoords(
		sf::Mouse::getPosition(this->get_game_ptr()->m_game_window), m_gui_view));

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

			// Set information bar Gui object's dimension and position.
			m_str_gui_map.at(INFO_BAR_GUI_STR).set_each_gui_entry_dimension(sf::Vector2f(
				event.size.width / m_str_gui_map.at(INFO_BAR_GUI_STR).get_entry_size(),
				INFO_BAR_GUI_ENTRY_SHAPE_HEIGHT));
			m_str_gui_map.at(INFO_BAR_GUI_STR).setPosition(this->get_game_ptr()->m_game_window.mapPixelToCoords
			(sf::Vector2i(0, event.size.height - INFO_BAR_GUI_ENTRY_SHAPE_HEIGHT), m_gui_view));
			// Display the updated information bar.
			m_str_gui_map.at(INFO_BAR_GUI_STR).show();

			// Set background sprite's position to window position (0, 0) related world position inside GUI view.
			this->get_game_ptr()->m_background_sprite.setPosition(this->get_game_ptr()->
			m_game_window.mapPixelToCoords(sf::Vector2i(0,0),m_gui_view));
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
				m_select_end_pos.x = (mouse_pos.y / m_game_city_ptr->get_map_ptr()->get_tile_half_width()) +
					(mouse_pos.x / (2 * m_game_city_ptr->get_map_ptr()->get_tile_half_width())) -
					(0.5 * m_game_city_ptr->get_map_ptr()->get_width()) - INT_TRUNCATION_OFFSET;
				m_select_end_pos.y = (mouse_pos.y / m_game_city_ptr->get_map_ptr()->get_tile_half_width()) -
					(mouse_pos.x / (2 * m_game_city_ptr->get_map_ptr()->get_tile_half_width())) +
					(0.5 * m_game_city_ptr->get_map_ptr()->get_width()) + INT_TRUNCATION_OFFSET;

				// Deselected all tiles first.
				m_game_city_ptr->get_map_ptr()->deselect_tiles();

				/* If m_curr_selected_tile_ptr is pointed to a grass tile, then current player action is considered to
				 * destroy all exist tiles rather than grass tiles. Therefore, grass tiles are considered as black list
				 * and all other tiles should be selected. */
				if (m_curr_selected_tile_ptr->m_tileType == TileTypeEnum::GRASS)
					m_game_city_ptr->get_map_ptr()->select(m_select_start_pos, m_select_end_pos,
						{ TileTypeEnum::GRASS});
				/* Else current player action is considered to build related new tile. And new tile can only be built on
				 * the grass tiles. Therefore, all other 7 type of tiles are considered as black list. */
				else
					m_game_city_ptr->get_map_ptr()->select(m_select_start_pos, m_select_end_pos,
						{ TileTypeEnum::VOID, TileTypeEnum::WATER, TileTypeEnum::ROAD,
						  TileTypeEnum::FOREST, TileTypeEnum::RESIDENTIAL, TileTypeEnum::COMMERCIAL,
						  TileTypeEnum::INDUSTRIAL});

				// Set up select text cost Gui object's contents.
				unsigned int selected_tiles_total_cost(m_curr_selected_tile_ptr->getCost() * m_game_city_ptr->
				get_map_ptr()->get_selected_tiles_num());
				m_str_gui_map.at(SELECT_TILE_COST_TEXT_GUI_STR).set_gui_entry_text(0, "$" +
				std::to_string(selected_tiles_total_cost));

				//  If the city does not have enough funds, then highlights the text content with red color.
				if (m_game_city_ptr->get_fund() < selected_tiles_total_cost)
					m_str_gui_map.at(SELECT_TILE_COST_TEXT_GUI_STR).highlight_entry(0);
				else
					m_str_gui_map.at(SELECT_TILE_COST_TEXT_GUI_STR).highlight_entry(-1);

				// Place and display the text to the bottom right of the mouse cursor.
				m_str_gui_map.at(SELECT_TILE_COST_TEXT_GUI_STR).setPosition(mouse_pos_in_gui_view +
				sf::Vector2f(SELECT_TILE_COST_TEXT_GUI_X_OFFSET, SELECT_TILE_COST_TEXT_GUI_Y_OFFSET));
				m_str_gui_map.at(SELECT_TILE_COST_TEXT_GUI_STR).show();
			}

			// If player's mouse hover on any of GuiEntries of right click menu, then highlight it.
			m_str_gui_map.at(MENU_GUI_STR).highlight_entry(m_str_gui_map.at(MENU_GUI_STR).
			get_gui_entry_idx(mouse_pos_in_gui_view));
			break;
		}
		// Check mouse button pressed cases.
		case sf::Event::MouseButtonPressed:
		{
			// when mouse middle button is pressed.
			if (event.mouseButton.button == sf::Mouse::Middle)
			{
				// Hide right click menu and selection cost text.
				m_str_gui_map.at(MENU_GUI_STR).hide();
				m_str_gui_map.at(SELECT_TILE_COST_TEXT_GUI_STR).hide();

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
				// If right click menu is visible, then try to select the tile which player's mouse hovers on.
				if (m_str_gui_map.at(MENU_GUI_STR).get_visible() == true)
				{
					// Try to get activated message from player mouse clicked Gui Entry object if valid.
					std::string activated_msg = m_str_gui_map.at(MENU_GUI_STR).
						get_mouse_pos_entry_msg(mouse_pos_in_gui_view);
					/* If there is one valid Gui Entry object been clicked, then store related tile object as current
					 * selected tile.*/
					if (activated_msg != "NULL")
						m_curr_selected_tile_ptr = std::make_shared<Tile>(this->get_game_ptr()->m_str_tile_map.
							at(activated_msg));

					// Hide the right click menu.
					m_str_gui_map.at(MENU_GUI_STR).hide();
				}
				else
				{
					// Switch to the tile select state.
					if (m_action_state != GameActionEnum::TILE_SELECTING)
					{
						m_action_state = GameActionEnum::TILE_SELECTING;
						/* Inverse of algebra formula inside Map::render function(change world coordinate to tile
						 * coordinate). Additional 0.5 is a compensation offset for integer truncation. */
						m_select_start_pos.x = (mouse_pos_in_game_view.y / m_game_city_ptr->get_map_ptr()->
							get_tile_half_width()) + (mouse_pos_in_game_view.x / (2 * m_game_city_ptr->get_map_ptr()
								->get_tile_half_width())) - (0.5 * m_game_city_ptr->get_map_ptr()->get_width()) -
									INT_TRUNCATION_OFFSET;
						m_select_start_pos.y = (mouse_pos_in_game_view.y  / m_game_city_ptr->get_map_ptr()->
							get_tile_half_width()) - (mouse_pos_in_game_view.x / (2 * m_game_city_ptr->get_map_ptr()
								->get_tile_half_width())) + (0.5 * m_game_city_ptr->get_map_ptr()->get_width()) +
									INT_TRUNCATION_OFFSET;
					}
				}
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				// Process when current state is tile selecting.
				if (m_action_state == GameActionEnum::TILE_SELECTING)
				{
					// Switch to the none state.
					m_action_state = GameActionEnum::NONE;
					// Hide the Gui which shows selected tiles overall cost.
					m_str_gui_map.at(SELECT_TILE_COST_TEXT_GUI_STR).hide();
					// De-select all exist tiles.
					m_game_city_ptr->get_map_ptr()->deselect_tiles();
				}
				// If not, then instead open the right click menu.
				else
				{
					// Get mouse position within gui view.
					sf::Vector2f pos = mouse_pos_in_gui_view;

					/* Ensure that right click menu Gui is displayed within the screen properly. If not enough space to
					 * put right click menu on mouse position's right side, then put it  on the left side of mouse
					 * position.*/
					if (pos.x > (this->get_game_ptr()->m_game_window.getSize().x - m_str_gui_map.at(MENU_GUI_STR)
					.get_gui_size().x))
						pos -= sf::Vector2f(m_str_gui_map.at(MENU_GUI_STR).get_gui_size().x, 0);
					if (pos.y > (this->get_game_ptr()->m_game_window.getSize().y - m_str_gui_map.at(MENU_GUI_STR)
						.get_gui_size().y))
						pos -= sf::Vector2f(0, m_str_gui_map.at(MENU_GUI_STR).get_gui_size().y);

					// Then update right click menu's position and display it.
					m_str_gui_map.at(MENU_GUI_STR).setPosition(pos);
					m_str_gui_map.at(MENU_GUI_STR).show();
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
					// If we have enough funds and have a current selected tile, then replace all selected tiles.
					if (m_curr_selected_tile_ptr != nullptr)
					{
						// Calculate overall cost.
						unsigned int selected_tiles_cost(m_curr_selected_tile_ptr->getCost() * m_game_city_ptr->
						get_map_ptr()->get_selected_tiles_num());
						// Check if city's fund is enough or not.
						if (m_game_city_ptr->get_fund() >= selected_tiles_cost)
						{
							// Replace all selected tiles.
							m_game_city_ptr->bulldoze(*m_curr_selected_tile_ptr);
							// Update city's fund.
							m_game_city_ptr->set_fund(m_game_city_ptr->get_fund() - selected_tiles_cost);
							// Update road tiles and re-calculate connected regions.
							m_game_city_ptr->updateTiles();
						}

					}
					// Hide select tile cost Gui.
					m_str_gui_map.at(SELECT_TILE_COST_TEXT_GUI_STR).hide();
					// Set game state to none and deselect all tiles.
					m_action_state = GameActionEnum::NONE;
					m_game_city_ptr->get_map_ptr()->deselect_tiles();
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
