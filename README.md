# SFML_CityBuilder

This is a simple 2D isometric city-building game demo created by C++ and SFML with CLion.

You can check the video, screenshots/GIFs and more information about this demo from my [personal portfolio website](https://yunxiang-li.github.io./#/game-projects).

[![standard-readme compliant](https://img.shields.io/badge/readme%20style-standard-brightgreen.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme)

## Table of Contents

- [Overview](#Overview)
- [Install](#install)
- [Usage](#Usage)
- [Structure](#Structure)
- [Maintainers](#Maintainers)
- [License](#license)

## Overview

The game will be launched with a 1280 x 720 window and a game menu scene with a blue sky background. When player click the **Load Game** button, then the game will be switched into **main game scene** and a 64*64 game map will be generated. If this is the first time that player play the game, a random map with only grass, water and forest tiles will be generated. If not, previously saved game map will be loaded and re-generated.

Inside the **main game scene**, player will notice the information bar on the bottom of the screen. The first one shows how many days elapse inside the game, second one displays the city's current total fund. The third one indicates the city's current population and homeless amount(in parenthesis). The fourth one means current employee's amount and unemployment number(in parenthesis). The fifth one indicates the player's current selected tile's type(default value will be of grass type).

Player can press mouse left button to select a tile or a bunch of tiles and de-select them by clicking right mouse button. If city's fund is enough to pay selected tiles' overall cost, all selected tiles will be replaced by player's previously stored tile type. Besides, player can click mouse right button and switch between different tile types inside the displayed tile menu Gui. There are altogether **7** types of tiles in this game: commercial, residential, industrial, road, grass, water and forest. Commercial, residential and industrial tiles are responsible for the city's development(more money, more goods, more people and thus more employee) and can only be placed on grass tiles. Additionally, these 3 tiles will upgrade based on small chance and city's development. After level up, textures will also be upgraded. Road and water tiles are also placeable only on grass tiles. Road tile is used to connect different regions and its texture will be updated automatically according to its neighbor road tiles' directions. Water tiles are just decoration and have simple animation.

When player quit the game, current game map will be saved automatically and load next time when player run the game again.

## Install

I Use C++14, [SFML(Simple and Fast Multimedia Library)](https://www.sfml-dev.org/) and CLion(2022.1) under Windows 10 environment for this project.

[CLion Download](https://www.jetbrains.com/clion/download/#section=windows)<br>

## Usage

1. Download this repo, open(or unzip and open) the **SFML_Isometric_CityBuilder** folder with **CLion**.

2. Set up **SFML** according to [official guide](https://www.sfml-dev.org/tutorials/2.5/start-vc.php).

3. Build and run the project to play the game.

## Structure

Detailed documentation can be viewed via this [link](https://yunxiang-li.github.io/SFML_Isometric_CityBuilder_Document/html)

The whole project contains three main folders, **include** folder, **src** folder and **resources** folder.

1. **include** folder contains altogether **16** header files:

2. **src** folder contains altogether **11** source files:

3. **Resources** folder contains **1** font(.ttf) file, **2** binary data files(.dat) and **8** game objects' image files(.png).

**1.** `Animation.hpp` : represents one certain animation of tiles.

**2.** `AnimationHandler` : help manage each animation within the game.

**3.** `City.hpp` : represents the game city and manages the actual gameplay.

**4.** `Game.hpp` : represent the instance of the city builder game itself.

**5.** `GameActionEnum.hpp` : a enum class contains all possible game actions inside the main game state.

**6.** `GameState.hpp` : a virtual base class represents a pure game state.

**7.** `Gui.hpp` : main part of Gui system which inherits from both `sf::Transformable` and `sf::Drawable` classes that allows our GUI to be moved and rendered.

**8.** `GuiEntry.hpp` : represents the entry part of the GUI.

**9.** `GuiStyle.hpp` : represents the style(size, color, font) of GUI objects.

**10.** `MainGameState.hpp` : inherits from the virtual base `GameState` class and represents the main game state.

**11.** `MainMenuGameState.hpp` : inherits from the virtual base `GameState` class and represents the main menu game state.

**12.** `Map.hpp` : contains a vector of `Tile` objects which is responsible for all game map behaviors.

**13.** `TextureManager.hpp` : manages all textures' load, store and retrieve behaviors with singleton pattern.

**14.** `Tile.hpp` : process all behaviors of one Tile object.

**15.** `TileTypeEnum.hpp` : a enum class contains all **7** types of tiles within the game.

**16.** `Utility.hpp` : stores all in-game data(include number, string, file path and so on).

## Maintainers

[@Yunxiang-Li](https://github.com/Yunxiang-Li).

## License

[MIT license](https://github.com/Yunxiang-Li/SFML_Isometric_CityBuilder/blob/main/LICENSE)
