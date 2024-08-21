#include <iostream>
#include <vector>
#include <fstream>
#include<string>
#include<cmath>
#include <random>
#include <SFML/Graphics.hpp>
#include "screens.h"

#include"tile.h"
#include"board.h"

using namespace std;




int main() {

    int height;
    int width;
    ifstream configFile("files/config.cfg");
    if(!configFile.is_open()){
        cout<<"Config file not opened" <<endl;
    }
    configFile >>width>>height;

    WelcomeScreen welcomescreen(height,width,"Welcome Screen",sf::Color::Blue);
    if(welcomescreen.runScreen()) {
        gameScreen gamescreen(height, width, "Game Screen", sf::Color::White);
        gamescreen.runScreen();


    }
    return 0;
}

