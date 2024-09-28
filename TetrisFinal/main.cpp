//
//  main.cpp
//  TetrisFinal
//
//  Created by Assier Anteneh Alemu on 12/09/2024.
//

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <cstdlib>
#include <ctime>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define BLOCK_SIZE 30
#define GRID_WIDTH 10
#define GRID_HEIGHT 20


int grid[GRID_HEIGHT][GRID_WIDTH] = {0};
int currentShape;
int nextShape;
int currentX, currentY;
int score = 0;
bool gameOver = false;
float fallSpeed = 0.05;
float fallTime = 0;

// Shape definitions
int SHAPES[7][4][4] = {
    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // I
    {{1, 1}, {1, 1}},                                           // O
    {{0, 1, 0}, {1, 1, 1}, {0, 0, 0}},                         // T
    {{1, 0, 0}, {1, 1, 1}, {0, 0, 0}},                         // L
    {{0, 0, 1}, {1, 1, 1}, {0, 0, 0}},                         // J
    {{0, 1, 1}, {1, 1, 0}, {0, 0, 0}},                         // S
    {{1, 1, 0}, {0, 1, 1}, {0, 0, 0}}                          // Z
};

int currentTetromino[4][4]; // Store the current tetromino's shape
int nextTetromino[4][4];    // Store the next tetromino's shape

void drawSquare(int x, int y) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + BLOCK_SIZE, y);
    glVertex2f(x + BLOCK_SIZE, y + BLOCK_SIZE);
    glVertex2f(x, y + BLOCK_SIZE);
    glEnd();
}

void drawGrid() {
    glColor3f(0.3f, 0.3f, 0.3f); // Grey color for grid lines
    for (int x = 0; x <= GRID_WIDTH * BLOCK_SIZE; x += BLOCK_SIZE) {
        glBegin(GL_LINES);
        glVertex2f(x, 0);
        glVertex2f(x, GRID_HEIGHT * BLOCK_SIZE);
        glEnd();
    }
    for (int y = 0; y <= GRID_HEIGHT * BLOCK_SIZE; y += BLOCK_SIZE) {
        glBegin(GL_LINES);
        glVertex2f(0, y);
        glVertex2f(GRID_WIDTH * BLOCK_SIZE, y);
        glEnd();
    }
}

void drawTetromino(int tetromino[4][4], int x, int y) {
    glColor3f(0.0f, 1.0f, 0.0f); // Green for tetromino
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (tetromino[i][j]) {
                drawSquare((x + j) * BLOCK_SIZE, (y + i) * BLOCK_SIZE);
            }
        }
    }
}

bool checkCollision(int dx, int dy, int shape[4][4]) {
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (shape[y][x]) {
                int newX = currentX + x + dx;
                int newY = currentY + y + dy;

                if (newX < 0 || newX >= GRID_WIDTH || newY >= GRID_HEIGHT || (newY >= 0 && grid[newY][newX])) {
                    return false;
                }
            }
        }
    }
    return true;
}

void rotateTetromino() {
    if (currentShape == 1) return; // Skip rotation for O-tetromino (doesn't benefit from rotation)

    int tempShape[4][4];
    int rotatedShape[4][4];

    // Copy the current shape to a temporary array
    memcpy(tempShape, currentTetromino, sizeof(tempShape));

    // Perform rotation on the temporary array
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            rotatedShape[x][3 - y] = tempShape[y][x];
        }
    }

    // Check if the rotated shape is valid
    bool isValid = true;
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (rotatedShape[y][x]) {
                int newX = currentX + x;
                int newY = currentY + y;

                if (newX < 0 || newX >= GRID_WIDTH || newY >= GRID_HEIGHT || (newY >= 0 && grid[newY][newX])) {
                    isValid = false; // Rotation causes collision or out of bounds
                    break;
                }
            }
        }
        if (!isValid) break;
    }

    // Apply the rotation if valid
    if (isValid) {
        // Copy rotated shape back to currentTetromino
        memcpy(currentTetromino, rotatedShape, sizeof(rotatedShape));
    }
}

void placeTetromino() {
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            if (currentTetromino[y][x]) {
                grid[currentY + y][currentX + x] = 1;
            }
        }
    }

    currentShape = nextShape;
    nextShape = rand() % 7;
    memcpy(currentTetromino, SHAPES[currentShape], sizeof(currentTetromino));
    currentX = GRID_WIDTH / 2 - 2;
    currentY = 0;

    if (!checkCollision(0, 0, currentTetromino)) {
        gameOver = true;
    }
}

void clearRows() {
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        bool fullRow = true;
        for (int x = 0; x < GRID_WIDTH; ++x) {
            if (grid[y][x] == 0) {
                fullRow = false;
                break;
            }
        }

        if (fullRow) {
            score += 10;  // Adjusted to increase by 10
            for (int clearY = y; clearY > 0; --clearY) {
                for (int x = 0; x < GRID_WIDTH; ++x) {
                    grid[clearY][x] = grid[clearY - 1][x];
                }
            }
            for (int x = 0; x < GRID_WIDTH; ++x) {
                grid[0][x] = 0;
            }
        }
    }
}

void instantDrop() {
    while (checkCollision(0, 1, currentTetromino)) {
        currentY++;
    }
    placeTetromino();
    clearRows();
}

void updateTetromino() {
    if (checkCollision(0, 1, currentTetromino)) {
        currentY++;
    } else {
        placeTetromino();
        clearRows();
    }
}

void drawNextTetromino() {
    glColor3f(0.0f, 0.0f, 1.0f); // Blue for the next tetromino
    int nextX = GRID_WIDTH + 1;
    int nextY = 1;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (SHAPES[nextShape][i][j]) {
                drawSquare((nextX + j) * BLOCK_SIZE, (nextY + i) * BLOCK_SIZE);
            }
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();
    
    // Draw current tetromino
    drawTetromino(currentTetromino, currentX, currentY);
    
    // Draw placed blocks
    glColor3f(1.0f, 0.0f, 0.0f); // Red for placed blocks
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            if (grid[y][x]) {
                drawSquare(x * BLOCK_SIZE, y * BLOCK_SIZE);
            }
        }
    }
    
    // Define sidebar dimensions and positions
    int sidebarWidth = SCREEN_WIDTH - GRID_WIDTH * BLOCK_SIZE;
    int sidebarX = GRID_WIDTH * BLOCK_SIZE;
    int sidebarHeight = SCREEN_HEIGHT;
    int centerX = sidebarWidth / 2;
    
    // Draw next tetromino centered in the sidebar, slightly lower
    glColor3f(0.0f, 0.0f, 1.0f); // Blue for the next tetromino
    int nextX = sidebarX + centerX - 2; // Center the next tetromino
    int nextY = 3; // Lowered position in the sidebar
    
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (SHAPES[nextShape][i][j]) {
                drawSquare(nextX + j * BLOCK_SIZE, nextY + i * BLOCK_SIZE);
            }
        }
    }
    
    // Display game-over message and score if game is over
    glColor3f(1.0f, 1.0f, 1.0f);
    if (gameOver) {
        glRasterPos2f(sidebarX + centerX - 50, SCREEN_HEIGHT / 2 - 10);
        const char *msg = "GAME OVER!";
        while (*msg) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *msg++);
        }
        
        // Display the score
        glRasterPos2f(sidebarX + centerX - 50, SCREEN_HEIGHT / 2 + 20);
        char scoreText[20];
        sprintf(scoreText, "Score: %d", score);
        for (char *c = scoreText; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
        
        // Display "Restart game with 'R'" below the score
        glRasterPos2f(sidebarX + centerX - 50, SCREEN_HEIGHT / 2 + 50);
        const char *restartMsg = "Restart game with 'R'";
        while (*restartMsg) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *restartMsg++);
        }
        
    } else {
        glRasterPos2f(sidebarX + centerX - 50, SCREEN_HEIGHT - 30); // Adjusted to center score in sidebar
        char scoreText[20];
        sprintf(scoreText, "Score: %d", score);
        for (char *c = scoreText; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    }
    
    glutSwapBuffers();
    
}
void timer(int value) {
    if (!gameOver) {
        fallTime += fallSpeed;
        if (fallTime >= 1.0) { // Adjust this value if needed
            updateTetromino();
            fallTime = 0;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0); // Calls timer function every 16ms (60fps)
}

void specialKeys(int key, int x, int y) {
    if (gameOver) return;

    if (key == GLUT_KEY_LEFT && checkCollision(-1, 0, currentTetromino)) {
        currentX--;
    } else if (key == GLUT_KEY_RIGHT && checkCollision(1, 0, currentTetromino)) {
        currentX++;
    } else if (key == GLUT_KEY_UP) {
        rotateTetromino();
    } else if (key == GLUT_KEY_DOWN) {
        updateTetromino();
    }
}


void restartGame() {
    // Reset game state variables
    memset(grid, 0, sizeof(grid)); // Clear the grid
    score = 0;                      // Reset score
    gameOver = false;               // Reset game over state
    fallTime = 0;                   // Reset fall time

    // Initialize the first and next Tetromino shapes
    currentShape = rand() % 7;
    nextShape = rand() % 7;
    memcpy(currentTetromino, SHAPES[currentShape], sizeof(currentTetromino));

    // Reset Tetromino position
    currentX = GRID_WIDTH / 2 - 2;
    currentY = 0;

    // Check if the game can start
    if (!checkCollision(0, 0, currentTetromino)) {
        gameOver = true; // If it can't, the game is over
    }
}

void keyboard(unsigned char key, int x, int y) {
   if (gameOver) {
            if (key == 'r' || key == 'R') {
                restartGame(); // Restart the game
            }
        } else {
            if (key == ' ') {
                instantDrop();
            }
        }

}
void init() {
    srand(static_cast<unsigned int>(time(0)));
    currentShape = rand() % 7;
    nextShape = rand() % 7;
    memcpy(currentTetromino, SHAPES[currentShape], sizeof(currentTetromino));
    currentX = GRID_WIDTH / 2 - 2;
    currentY = 0;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("Tetris");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}
