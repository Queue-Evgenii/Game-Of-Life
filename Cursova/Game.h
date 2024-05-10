#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "Grid.h"
#ifdef SFML_DYNAMIC
#include <SFML/Graphics.hpp>
#endif 

class GameOfLife {
protected:
  int rows;
  int cols;
  Grid curGrid, prevGrid;
  int diedCount = 0, bornCount = 0, aliveCount = 0;
  int countNeighbors(int row, int col) {
    int count = 0;
    for (int i = row - 1; i <= row + 1; i++) {
      for (int j = col - 1; j <= col + 1; j++) {
        if (i >= 0 && i < rows && j >= 0 && j < cols && !(i == row && j == col)) {
          count += curGrid[i][j];
        }
      }
    }

    return count;
  }
  void updateGrid() {
    bool** temp = new bool* [rows];
    for (int i = 0; i < rows; ++i) {
      temp[i] = new bool[cols];
    }
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        int neighbors = countNeighbors(i, j);

        if (curGrid[i][j]) {
          if (neighbors == 2 || neighbors == 3) {
            temp[i][j] = true;
          }
          else {
            temp[i][j] = false;
            ++diedCount;
            --aliveCount;
          }
        }
        else {
          if (neighbors == 3) {
            temp[i][j] = true;
            ++bornCount;
            ++aliveCount;
          }
          else {
            temp[i][j] = false;
          }
        }
      }
    }

    // Копируем новую сетку в текущую
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        curGrid[i][j] = temp[i][j];
      }
    }

    for (int i = 0; i < rows; i++) {
      delete[]temp[i];
    }
    delete[]temp;
  }
  void fillGrid() {
    diedCount = 0;
    bornCount = 0;
    curGrid.fillGrid();
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        prevGrid[i][j] = curGrid[i][j];
        if (curGrid[i][j]) {
          ++bornCount;
        }
      }
    }
    aliveCount = bornCount;
  }
  void initGrid() {
    curGrid.resize(rows, cols);
    prevGrid.resize(rows, cols);
  }
  bool checkGrids(int& index) {
    if (index == 0) {
      for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
          prevGrid[i][j] = curGrid[i][j];
        }
      }
    }
    else if (index == 2) {
      for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
          if (curGrid[i][j] != prevGrid[i][j]) {
            index = 0;
            return true;
          }
        }
      }
      return false;
    }
    ++index;
    return true;
  }
  virtual void drawGrid() {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        std::cout << (curGrid[i][j] ? "*" : " ");
      }
      std::cout << std::endl;
    }
    std::cout << "Died: " << diedCount << std::endl;
    std::cout << "Born: " << bornCount << std::endl;
    std::cout << "Alive: " << aliveCount << std::endl;
  }
public:
  GameOfLife() {};
  GameOfLife(int rows, int cols) : rows(rows), cols(cols) {
    initGrid();
    fillGrid();
    int index = 0;
    do {
      system("cls");
      drawGrid();
      updateGrid();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } while (checkGrids(index));
  };
};

#ifdef SFML_GRAPHICS_HPP
class GameOfLifeWithGraphics : GameOfLife {
private:
  bool isPause = false, gridStatus = true, isStart = false, isRestart = false;
  sf::RenderWindow window;
  sf::RectangleShape square, pauseButton, stopButton, startButton;
  sf::RectangleShape restartButton, fillButton, clearButton;
  sf::Text pauseButtonText, stopButtonText, startButtonText, fillButtonText, clearButtonText;
  sf::Text diedCountText, bornCountText, restartButtonText, aliveCountText;
  sf::Font font;
  void clearGrid() {
    diedCount = 0;
    bornCount = 0;
    aliveCount = 0;
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        prevGrid[i][j] = curGrid[i][j] = false;
      }
    }
  }
  void drawGrid() override {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        sf::Color color = curGrid[i][j] ? sf::Color(100, 5, 200) : sf::Color(235, 235, 235);
        square.setFillColor(color);
        square.setPosition(j * 10 + 10, i * 10 + 10);
        window.draw(square);
      }
    }
  }
  void drawInterface() {
    diedCountText.setString("Died: " + std::to_string(diedCount));
    bornCountText.setString("Born: " + std::to_string(bornCount));
    aliveCountText.setString("Alive: " + std::to_string(aliveCount));
    window.draw(startButton);
    window.draw(pauseButton);
    window.draw(stopButton);
    window.draw(fillButton);
    window.draw(clearButton);
    window.draw(fillButtonText);
    window.draw(clearButtonText);
    window.draw(diedCountText);
    window.draw(bornCountText);
    window.draw(aliveCountText);
    window.draw(startButtonText);
    window.draw(pauseButtonText);
    window.draw(stopButtonText);
  }
  void drawRestart() {
    aliveCountText.setString("Alive: " + std::to_string(aliveCount));
    aliveCountText.setPosition(10, rows * 10 + 15);
    window.draw(restartButton);
    window.draw(stopButton);
    window.draw(restartButtonText);
    window.draw(aliveCountText);
    window.draw(stopButtonText);
  }
  void setInterface() {
    square.setSize(sf::Vector2f(10, 10));
    square.setOutlineThickness(1);
    square.setOutlineColor(sf::Color::Black);

    fillButton.setSize(sf::Vector2f(105, 30));
    fillButton.setPosition(cols * 10 + 25, 130);
    fillButton.setFillColor(sf::Color(200, 200, 200));

    clearButton.setSize(sf::Vector2f(105, 30));
    clearButton.setPosition(cols * 10 + 25, 170);
    clearButton.setFillColor(sf::Color(200, 200, 200));

    pauseButton.setSize(sf::Vector2f(105, 30));
    pauseButton.setPosition(cols * 10 + 25, 90);
    pauseButton.setFillColor(sf::Color(200, 200, 200));

    stopButton.setSize(sf::Vector2f(105, 30));
    stopButton.setPosition(cols * 10 + 25, 50);
    stopButton.setFillColor(sf::Color(200, 200, 200));

    startButton.setSize(sf::Vector2f(105, 30));
    startButton.setPosition(cols * 10 + 25, 10);
    startButton.setFillColor(sf::Color(200, 200, 200));

    restartButton.setSize(sf::Vector2f(105, 30));
    restartButton.setPosition(cols * 10 + 25, 10);
    restartButton.setFillColor(sf::Color(200, 200, 200));

    if (!font.loadFromFile("arial.ttf")) {
      std::cout << "Failed to load font" << std::endl;
    }

    bool delta = (cols > 35);

    diedCountText.setFont(font);
    diedCountText.setCharacterSize(18);
    diedCountText.setFillColor(sf::Color(0, 0, 0));
    diedCountText.setPosition(10, rows * 10 + 15);

    bornCountText.setFont(font);
    bornCountText.setCharacterSize(18);
    bornCountText.setFillColor(sf::Color(0, 0, 0));
    delta ? bornCountText.setPosition(150, rows * 10 + 15) : bornCountText.setPosition(10, rows * 10 + 40);
    

    aliveCountText.setFont(font);
    aliveCountText.setCharacterSize(18);
    aliveCountText.setFillColor(sf::Color(0, 0, 0));
    delta ? aliveCountText.setPosition(290, rows * 10 + 15) : aliveCountText.setPosition(10, rows * 10 + 65);

    

    fillButtonText.setFont(font);
    fillButtonText.setCharacterSize(12);
    fillButtonText.setString("Fill");
    fillButtonText.setFillColor(sf::Color(0, 0, 0));
    fillButtonText.setPosition(cols * 10 + 67, 135);

    clearButtonText.setFont(font);
    clearButtonText.setCharacterSize(12);
    clearButtonText.setString("Clear");
    clearButtonText.setFillColor(sf::Color(0, 0, 0));
    clearButtonText.setPosition(cols * 10 + 60, 175);

    pauseButtonText.setFont(font);
    pauseButtonText.setCharacterSize(12);
    pauseButtonText.setString("Pause");
    pauseButtonText.setFillColor(sf::Color(0, 0, 0));
    pauseButtonText.setPosition(cols * 10 + 60, 95);

    stopButtonText.setFont(font);
    stopButtonText.setCharacterSize(12);
    stopButtonText.setString("Stop");
    stopButtonText.setFillColor(sf::Color(0, 0, 0));
    stopButtonText.setPosition(cols * 10 + 65, 55);

    startButtonText.setFont(font);
    startButtonText.setCharacterSize(12);
    startButtonText.setString("Start");
    startButtonText.setFillColor(sf::Color(0, 0, 0));
    startButtonText.setPosition(cols * 10 + 65, 15);

    restartButtonText.setFont(font);
    restartButtonText.setCharacterSize(12);
    restartButtonText.setString("Restart");
    restartButtonText.setFillColor(sf::Color(0, 0, 0));
    restartButtonText.setPosition(cols * 10 + 60, 15);
  }
public:
  GameOfLifeWithGraphics(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    std::cout << "Game created!" << std::endl;
    int _cols = cols * 10 + 150;
    int _rows = (cols > 35) ? rows * 10 + 60 : rows * 10 + 100;
    window.create(sf::VideoMode(_cols, _rows), "Game Life");
    sf::View view = window.getDefaultView();
    initGrid();
    while (window.isOpen()) {
      setInterface();
      int index = 0;
      do {
        sf::Event event;
        while (window.pollEvent(event)) {
          if (event.type == sf::Event::Closed) {
            return;
          }
          if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));
          }
          if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            if (isStart && pauseButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
              isPause = !isPause;
            }
            if (startButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
              isStart = true;
            }
            if (stopButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
              return;
            }
            if (fillButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
              fillGrid();
            }
            if (clearButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
              clearGrid();
            }
            if (mousePosition.y < (rows * 10 + 10) && mousePosition.x < (cols * 10 + 10) && mousePosition.y > 10 && mousePosition.x > 10) {
              int y = std::floor(mousePosition.y / 10);
              int x = std::floor(mousePosition.x / 10);
              curGrid[y - 1][x - 1] ? ++diedCount : ++bornCount;
              curGrid[y - 1][x - 1] ? --aliveCount : ++aliveCount;
              curGrid[y - 1][x - 1] = !curGrid[y - 1][x - 1];
              drawGrid();
              window.display();
            }
          }
          if (event.type == sf::Event::MouseMoved) {
            pauseButton.setFillColor(sf::Color(200, 200, 200));
            startButton.setFillColor(sf::Color(200, 200, 200));
            stopButton.setFillColor(sf::Color(200, 200, 200));
            fillButton.setFillColor(sf::Color(200, 200, 200));
            clearButton.setFillColor(sf::Color(200, 200, 200));
            if (isStart && pauseButton.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
              pauseButton.setFillColor(sf::Color(226, 226, 226));
            }
            if (stopButton.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
              stopButton.setFillColor(sf::Color(226, 226, 226));
            }
            if (startButton.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
              startButton.setFillColor(sf::Color(226, 226, 226));
            }
            if (fillButton.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
              fillButton.setFillColor(sf::Color(226, 226, 226));
            }
            if (clearButton.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
              clearButton.setFillColor(sf::Color(226, 226, 226));
            }
          }
        }
        window.clear(sf::Color(255, 255, 255));
        drawGrid();
        drawInterface();
        if (!isPause && isStart) {
          updateGrid();
          gridStatus = checkGrids(index);
        }
        window.display();
        sf::sleep(sf::milliseconds(100));
      } while (gridStatus && window.isOpen());

      //after game over
      while (!gridStatus && window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
          if (event.type == sf::Event::Closed) {
            return;
          }
          if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));
          }
          if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            if (restartButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
              gridStatus = true;
              isRestart = true;
            }
            if (stopButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
              return;
            }
          }
          if (event.type == sf::Event::MouseMoved) {
            restartButton.setFillColor(sf::Color(200, 200, 200));
            stopButton.setFillColor(sf::Color(200, 200, 200));
            if (restartButton.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
              restartButton.setFillColor(sf::Color(226, 226, 226));
            }
            if (stopButton.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
              stopButton.setFillColor(sf::Color(226, 226, 226));
            }
          }
        }
        window.clear(sf::Color(255, 255, 255));
        drawGrid();
        drawRestart();
        window.display();
        if (isRestart) {
          isStart = false;
          isPause = false;
          isRestart = false;
          gridStatus = true;
          break;
        }
        sf::sleep(sf::milliseconds(100));
      }
    }
  }
};
#endif