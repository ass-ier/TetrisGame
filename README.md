Tetris Game 🎮

A simple Tetris clone built using C++ and OpenGL (GLUT).

📂 Project Structure
```
TetrisFinal/
├── TetrisFinal/            # Source folder
│   └── main.cpp            # Main game source code
├── TetrisFinal.xcodeproj/  # Xcode project files
└── README.md               # Project documentation
```

🚀 Features
	•	Real-time block falling with adjustable speed.
	•	Classic Tetromino shapes (I, O, T, L, J, S, Z).
	•	Smooth rotation and movement.
	•	Instant drop with spacebar.
	•	Score tracking.
	•	Next Tetromino preview.
	•	Game over detection and restart option.

🎮 Controls
```
| Key                | Action              |
|--------------------|---------------------|
| Left Arrow         | Move Left           |
| Right Arrow        | Move Right          |
| Up Arrow           | Rotate Tetromino    |
| Down Arrow         | Accelerate Drop     |
| Spacebar           | Instant Drop        |
| R                  | Restart (Game Over) |
```

🛠️ How to Build and Run

🖥️ Requirements
	•	C++ Compiler
	•	OpenGL Utility Toolkit (GLUT)
	•	Xcode (for macOS users)

⚙️ macOS Build Instructions (Xcode)
	1.	Open TetrisFinal.xcodeproj in Xcode.
	2.	Select the correct scheme.
	3.	Click the Run button or use Cmd + R to build and run the project.

✅ General Compilation (Command Line)
If you want to compile from terminal (macOS example):
```
g++ TetrisFinal/main.cpp -o TetrisFinal -framework OpenGL -framework GLUT
./TetrisFinal
```
✨ Author

Assier Anteneh Alemu
Developed on: 12/09/2024

