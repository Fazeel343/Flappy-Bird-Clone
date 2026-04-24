Flappy Bird Clone (C++ & SFML)
A high-fidelity, arcade-style Flappy Bird recreation built using C++ and the SFML (Simple and Fast Multimedia Library). This project showcases smooth 2D physics, procedural obstacle generation, and a fully functional UI system including a dedicated instructions menu.

🚀 Overview
This repository contains a complete, playable clone of Flappy Bird. The game is designed for performance and simplicity, with the core engine logic contained within a single source file to demonstrate efficient game-loop management.

📂 Project Structure
Plaintext
.
├── resources/           # Game assets (Textures, Fonts, Audio)
├── src/                 # SFML include headers and library binaries
├── FLAPPY_BIRD.cpp      # Main game engine and logic
├── Makefile             # Automated build system
├── launch               # Compiled executable (Linux/macOS/Windows Bash)
└── [SFML DLLs]          # Necessary runtime libraries for Windows
🎮 How to Play
The game features a dedicated Instructions Tab accessible from the main menu.

Press SPACE to flap your wings and gain altitude.

Avoid hitting the pipes or the ground.

Pass through pipes to increase your score.

Press R to restart immediately after a Game Over.

Have fun and try to beat the high score!

🛠️ Technical Features
Custom Physics: Implemented gravity, velocity, and jump impulses.

Automated Build: Includes a Makefile to handle complex SFML linking.

UI Management: Multiple game states including Menu, Instructions, Gameplay, and Game Over.

Collision System: Precise AABB (Axis-Aligned Bounding Box) detection.

🔧 Installation & Building
Prerequisites
Ensure you have a C++ compiler (like g++) and the SFML development files installed. The src/ directory in this repo is pre-configured for linking.

1. Compile the Project
Run the following command in your terminal:

Bash
make
This uses the Makefile to compile FLAPPY_BIRD.cpp and link the graphics, window, system, and audio modules.

2. Run the Game
Execute the compiled binary:

Bash
./launch
3. Clean Build
To remove the object files and executable:

Bash
make clean
