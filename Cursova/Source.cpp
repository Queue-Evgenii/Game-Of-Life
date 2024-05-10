#include "Game.h"
#include <iostream>
#include <ctime>

int main() {
  srand(time(NULL));
  char temp[99];
  int rows, cols;
  do {
    std::cout << "Enter quantity of rows (4 < value < 80): ";
    std::cin.getline(temp, 99);
    rows = atoi(temp);
  }while (rows < 4 || rows > 80);

  do {
    std::cout << "Enter quantity of columns (4 < value < 140): ";
    std::cin.getline(temp, 99);
    cols = atoi(temp);
  } while (cols < 4 || cols > 140);
#ifdef SFML_GRAPHICS_HPP
  GameOfLifeWithGraphics newGame(rows, cols);
#else
  GameOfLife newGame(rows, cols);
#endif
  return 0;
}
