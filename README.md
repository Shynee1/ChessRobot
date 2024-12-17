# Chess Robot

## Overview
The Chess Robot is a fully functional robotic system capable of physically playing chess on a real-world chessboard. It autonomously detects player moves and responds with its own moves using robotics, chess engine logic, and a custom-designed graphical user interface (GUI). The system operates through a 2-axis gantry for X and Y movement, a linear actuator with an **electromagnet** for Z-axis control, and magnetic piece detection using reed sensors under the chessboard.

This project was created by the **Haverford Electronics Group** with support from **The Haverford School**.

[ChessRobot_GUI](https://github.com/user-attachments/assets/98f5556a-331b-49cb-8486-76731d9ac243)

## How It Works

1. **Move Detection**: The player moves chess pieces on the board, which activates magnetic reed sensors beneath each square.
2. **Move Processing**: The robot's software determines the player's move by comparing the new board state with the previous state.
3. **Move Calculation**: The chess engine calculates the robot's best possible response move.
4. **Piece Movement**: The robot's gantry system moves along the X, Y, and Z axes to pick up and place pieces using an electromagnet.

## Key Features

- **Autonomous Chess Play**: Plays chess autonomously against a human player.
- **Player Move Detection**: Uses magnetic reed sensors wired to a custom PCB beneath the chessboard to detect player moves.
- **Robot Movement**: Precisely moves chess pieces using a gantry system with X, Y, and Z-axis control, combined with an electromagnet for gripping pieces.
- **Chess Engine**: Uses chess engine logic to calculate the robot's next move.
- **Custom GUI**: Provides real-time status, game settings, and control over the robot.

## Demonstration
To see the Chess Robot in action, check out this video:
[https://drive.google.com/file/d/1hXQ2FK0-B0WJ1CRfMA2Q9d0eLAbO2lZv/view?usp=sharing]

## Project Structure
```
ChessRobot/
├── build/           # Build files (generated by CMake, not included in source control)
├── external/        # External libraries (e.g., chess logic, serial communication)
├── headers/         # C++ header files for core logic, hardware control, etc.
├── src/             # C++ source files containing the main implementation
├── assets/          # Asset files (GUI images, resources)
├── CMakeLists.txt   # CMake build configuration
├── conanfile.txt    # Conan package configuration
└── .gitignore       # Git ignore file
```

## In-depth Explanation

### 1. **Move Detection**
- An Arduino MEGA connected to the custom PCB recieves signals from magnetic reed sensors beneath the chessboard.
- The `BoardInput` class communicates with the Arduino MEGA to recieve the updated board state.
- The system tracks changes in the board state to identify the origin and destination squares of the player's move.

### 2. **Move Calculation**
- The new board state is processed by a chess engine to calculate the best response.
- This engine can be set to easy, medium, or hard.
- By default the robot runs Stockfish 17 but a ![custom chess engine](https://github.com/Shynee1/ChessEngine) can be used instead.

### 3. **Piece Movement**
- The robot's gantry system uses GRBL commands to move to the starting position of the piece.
- The Z-axis actuator lowers the electromagnet to magnetically pick-up the piece.
- The robot moves it to the target square and turns off the electromagnet to release the piece.
- The robot is always sent to home after every move to ensure saftey.

## Getting Started

### Prerequisites
  - CMake 3.16 or higher.
  - Conan package manager.
  - C++17 compiler.
  - Raspberry Pi 3 Model B

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/ChessRobot.git
   cd ChessRobot
   ```
2. Install dependencies using Conan:
   ```bash
   conan install .
   ```
3. Build the project using CMake:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```
4. Upload the Arduino sketch to the Arduino board.

## Usage
1. Turn on the Chess Robot.
2. Launch the ChessRobot executable.
3. Connect the machine and board via the GUI.
4. Use the GUI to:
   - Start a new game.
   - Adjust engine difficulty.
   - View game status and machine coordinates.
5. Play a move on the physical chessboard. The robot will detect the move, calculate its response, and physically move the pieces using the gantry system and electromagnet.

## Contributing
Contributions are welcome! If you'd like to report issues or suggest improvements, please open a GitHub issue or submit a pull request.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

With this Chess Robot, you'll experience a unique blend of robotics, chess strategy, and software engineering. Watch as the robot plays chess with precision and intelligence, blending human input with robotic execution.

---

### Created by the Haverford Electronics Group with support from The Haverford School.

