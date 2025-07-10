#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

const int WIDTH = 800;
const int HEIGHT = 800;
const int ROWS = 100;
const int COLS = 100;
const float CELL_WIDTH = WIDTH / (float)COLS;
const float CELL_HEIGHT = HEIGHT / (float)ROWS;

std::vector<std::vector<int>> grid(ROWS, std::vector<int>(COLS));
std::vector<std::vector<int>> nextGrid(ROWS, std::vector<int>(COLS));

// Initialize grid with random states
void randomizeGrid() {
  for (int i = 0; i < ROWS; ++i)
    for (int j = 0; j < COLS; ++j)
      grid[i][j] = rand() % 2;
}

// Count live neighbors
int countNeighbors(int x, int y) {
  int count = 0;
  for (int dx = -1; dx <= 1; ++dx)
    for (int dy = -1; dy <= 1; ++dy) {
      if (dx == 0 && dy == 0)
        continue;
      int nx = x + dx;
      int ny = y + dy;
      if (nx >= 0 && nx < ROWS && ny >= 0 && ny < COLS)
        count += grid[nx][ny];
    }
  return count;
}

// Apply Game of Life rules
void updateGrid() {
  for (int i = 0; i < ROWS; ++i) {
    for (int j = 0; j < COLS; ++j) {
      int neighbors = countNeighbors(i, j);
      if (grid[i][j] == 1) {
        nextGrid[i][j] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
      } else {
        nextGrid[i][j] = (neighbors == 3) ? 1 : 0;
      }
    }
  }
  grid.swap(nextGrid);
}

// Draw cells
void drawGrid() {
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_QUADS);
  for (int i = 0; i < ROWS; ++i) {
    for (int j = 0; j < COLS; ++j) {
      if (grid[i][j] == 1) {
        float x = j * CELL_WIDTH;
        float y = i * CELL_HEIGHT;
        float x1 = x / WIDTH * 2 - 1;
        float y1 = y / HEIGHT * 2 - 1;
        float x2 = (x + CELL_WIDTH) / WIDTH * 2 - 1;
        float y2 = (y + CELL_HEIGHT) / HEIGHT * 2 - 1;

        glColor3f(1.0f, 1.0f, 1.0f); // white cell
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
      }
    }
  }
  glEnd();
}

int main() {
  srand(time(NULL));

  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    return -1;
  }
  GLFWwindow *window =
      glfwCreateWindow(WIDTH, HEIGHT, "Game of Life", NULL, NULL);
  if (!window) {
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW\n";
    return -1;
  }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1, 1, -1, 1, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  randomizeGrid();

  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    drawGrid();
    updateGrid();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
