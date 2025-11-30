# Prime-match-3
Made a match-3 type of game using C language and Raylib library. This game is different from usual match-3 type games of matching colours, candies, etc to matching numbers.

Prime Match-3

Prime Match-3 is a unique puzzle game developed in C using the Raylib library. It puts a mathematical twist on the classic "match-three" genre. Instead of matching colors or shapes, players must calculate sums to match a target prime number within a time limit.

🎮 How to Play

1)The Objective: Swap adjacent tiles on the 9x9 grid.

2)The Match: A match is formed when 3 adjacent tiles (horizontal or vertical) sum up to the TARGET number displayed at the bottom left.

3)The Target: The target is always a prime number (e.g., 3, 5, 7, 11, 13, 17, 19, 23).

4)Progression: Every successful match increases your score and generates a new target prime number.

5)Time Limit: You have 3 minutes to score as high as possible.

6)Stuck? Use the SHUFFLE button to rearrange the board if no moves are available.

✨ Features

Dynamic Gameplay: The matching criteria changes constantly as the Target Prime updates.

Shuffle Mechanic: Randomized board resetting ensures the game never reaches a dead end.

Game State System: Complete flow with Play state, Timer countdown, and Game Over screen.

Restart Function: Instantly restart the game from the Game Over screen.

Efficient Code: Written in pure C with minimal overhead, utilizing Raylib for hardware-accelerated graphics.

🛠️ Technology Stack

Language: C (Standard C99)

Library: Raylib (v4.0 or higher recommended)

📂 Project Structure

/
├── assets/
│   └── background.jpg    # Game background texture
├── main.c                # The complete source code
├── README.md             # This file
└── game_window.png       # Screenshot


🚀 How to Build & Run

Prerequisites

A C Compiler (GCC, Clang, or MSVC).

The Raylib library installed on your system.

Compilation

Linux (GCC):

gcc main.c -o primematch -lraylib -lGL -lm -lpthread -ldl -lrt -lX11


Windows (MinGW/GCC):

gcc main.c -o primematch.exe -lraylib -lopengl32 -lgdi32 -lwinmm


macOS (Clang):

clang main.c -o primematch -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo


Running the Game

Ensure the assets folder is in the same directory as your executable, then run:

./primematch


📝 Future Improvements

Add audio feedback (match sounds, background music).

Add visual animations for tile swapping and falling.

Implement a High Score system saved to a local file.

📄 License

This project is open-source. Feel free to use and modify the code for educational purposes.
