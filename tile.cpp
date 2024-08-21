//
// Created by Goran on 7/28/2024.
//
#include <random>
#include "tile.h"
#include "board.h"

Tile::Tile(sf::RenderWindow &window,sf::Sprite _hidden, sf::Sprite _revealed) :
    window(window),hiddenSprite(_hidden),revealedSprite(_revealed){
    value = 0;
}


void Tile::reveal(){

    hiddenT = false;
}

bool Tile::returnHidden() {
    return hiddenT;
}

bool Tile::isClicked() {
    return clicked;
}

void Tile::setClicked() {
    clicked = true;
}


void Tile::setFlagged() {
    Flagged = true;
}
void Tile::setFlaggedFalse() {
    Flagged = false;
}
bool Tile::isFlagged() {
    return Flagged;
}

void Tile::setMine() {
    Mine = true;
}

bool Tile::isMine() {
    return Mine;
}














