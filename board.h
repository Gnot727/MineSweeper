//
// Created by Goran on 7/28/2024.
//

#ifndef PROJECT3_BOARD_H
#define PROJECT3_BOARD_H

#endif //PROJECT3_BOARD_H
#include <iostream>
#include <vector>
#include<string>
#include<cmath>
#include<fstream>
#include <sstream>
#include <random>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "screens.h"

#include"tile.h"
#pragma once

class Board{

    sf::RenderWindow &window;
    sf::Sprite sprite;
    int columns;
    int rows;
    int mineCount;
    bool paused = false;
    bool runClock= false;
    chrono::high_resolution_clock ::time_point initTime;
    int totalTime;
    bool leaderboardOpen =false;
public:

    vector<vector<Tile*>> tiles;



    Board(int rows, int col,sf::RenderWindow &window,sf::Sprite &hidden, sf::Sprite &revealed);
    ~Board();
    void setMines();
    void setNeighbors();
    void chainReaction(sf::RenderWindow &window, Tile* tile);
    void changeHappyFace(sf::Sprite &lose);
    void drawLose(sf::RenderWindow &wind, sf::Sprite &lose);
    void displayMines(sf::RenderWindow &window,sf::Sprite &mine);

    void resetBoard(sf::RenderWindow &window, sf::Sprite &hidden, sf::Sprite &revealed);
    void boolClicked(int x, int y);
    void drawFlag(sf::RenderWindow &window, sf::Sprite &flag);
    void drawBoard(sf::RenderWindow &window,sf::Sprite &revealed, sf::Sprite &hidden, sf::Sprite &mines);
    void setClock(sf::RenderWindow &window, sf::Sprite &digits);
    void clicked(int x, int y);
    void drawCounter(sf::RenderWindow &window, sf::Sprite &digits,int &flagCount);
    bool checkIfWin();

    void startClock();
    void setPaused();

    int getElapsedTime();
    void resetClock();
    void changePauseButton(sf::Sprite &play);

    void openLeader();
    void setFlaggedWin(sf::RenderWindow &window, sf::Sprite &flag);
    void setLoseMines(sf::RenderWindow &window, sf::Sprite &mine);
    void setPausedScreen(sf::RenderWindow &window, sf::Sprite &revealed,sf::Sprite &playButton);
    void setLeaderboardScreen(sf::RenderWindow&window,sf::Sprite &revealed);

};