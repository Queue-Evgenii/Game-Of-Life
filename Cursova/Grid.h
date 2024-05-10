#pragma once

class Grid {
private:
  int rows;
  int cols;
  bool** grid;
public:
  Grid() : rows(0), cols(0), grid(nullptr) {}
  ~Grid() {
    for (int i = 0; i < rows; i++) {
      delete[] grid[i];
    }
    delete[] grid;
  }
  bool* operator[](int row) {
    return grid[row];
  }
  void fillGrid() {
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        grid[i][j] = (rand() % 2 == 0);
      }
    }
  }
  void resize(int rows, int cols) {
    if (grid) {
      for (int i = 0; i < rows; i++) {
        delete[] grid[i];
      }
      delete[] grid;
    }
    this->rows = rows;
    this->cols = cols;
    grid = new bool* [rows];
    for (int i = 0; i < rows; i++) {
      grid[i] = new bool[cols] { 0 };
    }
  }
};