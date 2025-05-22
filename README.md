# INT2215 Project: Bomberman

This project is a Bomberman game developed for the **INT2215** course. The game is implemented in C++ using the SDL2 library and its extensions for graphics, text rendering, and audio. 

## Table of Contents
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Learning Resources](#learning-resources)
- [Gameplay Instructions](#gameplay-instructions)
- [Project Structure](#project-structure)

## Features
- **2D Graphics**: Sprite-based visuals for characters, bombs, and environments using SDL2_Image.
- **Text Rendering**: Displays scores, menus, and game-over screens using SDL2_ttf.
- **Audio**: Sound effects for explosions and background music using SDL2_mixer.
- **Game Mechanics**: Player movement, bomb placement, collision detection, and enemy AI.
- **Levels**: Player can define their own using txt files.
- **Menu System**: Start, pause, and game-over screens.

## Prerequisites
To build and run this project, ensure you have the following installed:
- **C++ Compiler**: e.g., `g++` (part of GCC) or any compatible compiler.
- **SDL2 Libraries**:
  - [SDL2](https://www.libsdl.org/): Core library for window management, rendering, and input handling.
  - [SDL2_Image](https://github.com/libsdl-org/SDL_image): For loading image formats (e.g., PNG, JPG).
  - [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf): For rendering TrueType fonts.
  - [SDL2_mixer](https://github.com/libsdl-org/SDL_mixer): For audio playback (e.g., WAV, MP3).
- **Build Tools**: This project was buit using Visual Studio.


## Installation
Follow these steps to set up and run the project:

1. **Install SDL2 and its extensions**:
   - Refer to the official documentation for your operating system:
     - [SDL2](https://www.libsdl.org/)
     - [SDL2_Image](https://github.com/libsdl-org/SDL_image)
     - [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf)
     - [SDL2_mixer](https://github.com/libsdl-org/SDL_mixer)
   - On Windows, download the development libraries and configure them with your IDE (e.g., Visual Studio).

2. **Clone the project repository**:
   ```bash
   git clone https://github.com/Kaasw/INT2215.git
   ```


3. **Build the project**:
   - Open the project using Visual Studio, redirect to main.cpp and run the file.
   - Ensure your build system links the SDL2 libraries correctly.



## Learning Resources
The project was developed with guidance from the following tutorials:
- **[Noah11012's SDL2 Tutorial Series](https://dev.to/noah11012/series/8749)**: Covers SDL2 basics, sprite rendering, and game loops.
- **[Lazy Foo' Productions SDL Tutorials](https://lazyfoo.net/)**: Detailed guides on SDL2, including texture management, input handling, and audio integration.


## Gameplay Instructions
- **Controls**:
  - **WASD**: Move the player (up, down, left, right).
  - **Spacebar**: Place a bomb.
  - **Enter**: Select options in menus.
- **Objective**:
  - Clear obstacles (e.g., breakable walls) using bombs.
  - Defeat all enemies to progress to the next level.
  - Avoid bomb explosions and enemy collisions.
- **Tips**:
  - Bombs have a fixed timer and blast radius.
  - Plan your movements to avoid trapping yourself.

## Project Structure
```
Project1/
├── assets/                             # Sprites, fonts, and audio files.
│   ├── texture/                        # PNG/JPG files for sprites.
│   ├── fonts/                          # TTF font files.
│   └── music/                          # WAV/MP3 audio files.
├── src/                                # Source code.
│   ├── main.cpp                        # Game main.
│   ├── runner/                         # Define the game windows, run game, draw and update.
│   ├── levels/                         # Game levels define with txt files.
│   ├── graphics/
        ├──map/                         # Load map logic.
        ├──menu/                        # Game menu.
        ├──sprite/                      # Load objects sprite.
    ├── entities/
        ├── object/                     # Parents for all the game's object.
        ├── AI/                         # Define the logic for enemy with A* algorithm.
        ├── baloon/                     # Define the logic for baloon
        ├── bomb/                       # Define the logic for bomb
        ├── brick/                      # Define the logic for brick
        ├── explosion/                  # Define the logic for explosion
        ├── oneal/                      # Define the logic for oneal
        ├── wall/                       # Define the logic for wall
        ├── player/                     # Define the logic for wall
│   
└── README.md          # This file
```

