# Tetlone

Tetlone is a small **Tetris-style game** written in **C++ using
OpenGL**.\
The project focuses on basic graphics programming concepts such as
rendering, input handling, and implementing a simple game loop.

## Overview

Tetlone recreates the classic Tetris gameplay. Differently shaped blocks
spawn at the top of the screen and slowly fall downward.\
The player can move the blocks horizontally or accelerate their descent
in order to complete horizontal rows.

When a row is completed: - The row disappears - The player earns
points - The visual theme of the game changes

## Features

- Classic **Tetris-style gameplay**
- **Local high score system** stored in a text file
- Keyboard controls for moving blocks
- Dynamic **theme changes** when rows are cleared
- Sound effects

| Key             | Action           |
| --------------- | ---------------- |
| A / Left Arrow  | Move block left  |
| D / Right Arrow | Move block right |
| S / Down Arrow  | Move block down  |

## Technical Details

This project demonstrates several programming concepts:

- **C++ game loop implementation**
- Batch-rendering with the **OpenGL API**
- Themes with fragment shader
- **Keyboard input handling**
- Simple **score persistence** using a local text file
- Basic **game state management**

## Requirements

-   **Microsoft Visual Studio 2022** or later
-   Windows environment

## Installation

1.  Clone or download this repository.
2.  Run the `GenWinProject.bat` script to generate the `.sln` file.
3.  Open the generated solution in **Visual Studio**.
4.  Build and run the project.

## Future Improvements

Possible future improvements for the project:

- Support **cross-platform builds**
- Improve control UI
- Implement multiplayer game mode

## License

This project is open source. Feel free to modify and distribute it.
