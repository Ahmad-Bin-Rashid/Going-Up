# GoingUp - A Roguelike Deckbuilder Game

## Overview
**GoingUp** is a console-based adventure game where players navigate through various rooms, encounter enemies, and collect treasures. The game features dynamic map generation, strategic combat mechanics, and diverse enemy types, providing a unique experience with each playthrough.

## Features
- **Dynamic Map Generation**: Every dungeon is procedurally generated, ensuring that no two runs are the same.
- **Strategic Combat**: Engage in turn-based combat using attack cards such as Single Slash and Bow.
- **Intelligent Enemies**: Enemies use probability-based decision-making to predict and counter player moves.
- **Deck Building**: Collect and manage attack cards to build a powerful deck and conquer the dungeon.
- **Diverse Room Types**: Explore various room types like Spawn, Treasure, Trap, Large, and Boss rooms.
- **Replayability**: Randomized enemies, traps, treasures, and room layouts ensure infinite replayability.

## Project Structure
### Key Components
- **Classes**:
  - **Character**: Base class for all characters in the game (players and enemies).
  - **Player**: Derived from `Character`, represents the player with specific attributes and abilities.
  - **Enemy**: Derived from `Character`, represents various enemy types like Goblin, Skeleton Warrior, Cave Spider, Fire Imp, and Infernal Warlord.
  - **Floor**: Manages the map generation, room connections, and validation.
  - **Combat**: Handles combat mechanics between the player and enemies.
  - **ScreenBuffer**: Manages console screen output and animations.

- **Data Structures**:
  - **DoublyLinkedList**: Custom implementation for managing player and enemy movements.
  - **Stack**: Custom stack implementation for managing attack cards.
  - **Queue**: Custom queue implementation used for room traversal.

- **Utility Functions**:
  - **roomTypeToString**: Converts room type enums to strings.
  - **stringToRoomType**: Converts strings to room type enums.
  - **gotoxy**: Moves the console cursor to a specific position.
  - **fullsc**: Sets the console to full screen.
  - **hideCursor**: Hides the console cursor for a clean interface.
  - **readFramesFromFile**: Reads sprite frames from text files.

## How to Build
1. Ensure you have a C++ compiler installed (e.g., `g++`, `clang++`).
2. Navigate to the project directory.
3. Compile the project with the following command:
   ```bash
   g++ -std=c++17 GoingUp.cpp -o GoingUp.exe
4. Run executable: 
   ```bash
   ./GoingUp.exe

## How to Play
1. **Start the Game**: 
   Run the game by executing the compiled `GoingUp.exe` or `./GoingUp` on your system.

2. **Navigate Through Rooms**: 
   Use the provided options to move through different room types, such as Spawn, Treasure, Trap, Large, and Boss rooms. The console interface will display the available options for you to choose from.

3. **Encounter Enemies and Engage in Combat**: 
   As you move through the dungeon, you'll encounter various enemies. Use your attack cards (e.g., Single Slash, Bow) in turn-based combat to defeat enemies. The enemies also adapt their strategies, making each combat encounter unique.

4. **Collect Treasures and Avoid Traps**: 
   Explore Treasure rooms for valuable items, including weapons and health potions. Be wary of Trap rooms that deal damage, which require a dice roll for evasion.

5. **Defeat the Boss to Win**: 
   After progressing through multiple rooms, you'll face the final boss. Defeat the boss to win the game and complete your dungeon run.

## Dependencies
- **C++ Standard Library**: 
   - Used for core data structures and algorithms.
   - Includes containers like `vector`, `stack`, `queue`, and other necessary utilities for implementing gameplay mechanics.
  
- **Windows API**: 
   - Used for console manipulation, like moving the cursor to specific positions and resizing the console window for better UI.


## Course Details

**Subject**: Data Structures and Algorithms  
**Semester**: 3rd  

## License
This project is licensed under the MIT License. See the LICENSE file for details.
