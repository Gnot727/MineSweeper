//
// Created by Goran on 7/28/2024.
//

#ifndef PROJECT3_TILE_H
#define PROJECT3_TILE_H
#include <iostream>
#include <vector>
#include<string>
#include<cmath>
#include<fstream>
#include <sstream>
#include <random>
#include <SFML/Graphics.hpp>
#include "screens.h"

#pragma once

class Tile{

    sf::RenderWindow& window;
    bool hiddenT = true;
    bool clicked = false;
    bool Flagged =false;
    bool Mine=  false;
public:

    sf::Sprite hiddenSprite;
    sf::Sprite revealedSprite;
    sf::Sprite sprite;
    int value;
    vector<Tile*> neighbors;

    Tile( sf::RenderWindow &window,sf::Sprite hidden,sf::Sprite revealed);


    void setFlagged();
    void setFlaggedFalse();
    bool isFlagged();
    bool isClicked();
    void setClicked();
    bool returnHidden();
    void reveal();
    void setMine();
    bool isMine();

};
#endif //PROJECT3_TILE_H
