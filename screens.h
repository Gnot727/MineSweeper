//
// Created by Goran on 7/25/2024.
//

#include <iostream>
#include <vector>
#include<string>
#include<cmath>
#include<fstream>
#include <random>
#include <map>
#include <SFML/Graphics.hpp>



#pragma once
using namespace std;


class Window{
protected:
    sf::Font font;
    sf::Text titleText;
    sf::Text inputText;
    sf::Text userText;

    sf::Text leaderBoardText;
    int width;
    int mineCount;
    int height;
public:
    Window(int height, int width, string title, sf::Color color);
    void setText(sf::Text &text, string letters, int charSize, sf::Color strColor, float x, float y);
    void setTextPosition(sf::Text &text, float x, float y);

    virtual bool runScreen() = 0;


    sf::RenderWindow window;
};

class WelcomeScreen: public Window{
public:
    WelcomeScreen(int height, int width, string letters, sf::Color color);
    bool runScreen() override;
};

class gameScreen: public Window{

public:

    gameScreen(int height, int width, string title, sf::Color color);
    bool runScreen()override;
};
class LeaderBoard: public Window{

    map<string,int> timeName;
public:
    LeaderBoard(int height, int width, string letters, sf::Color color);
    bool runScreen() override;
    void drawLeader(string filename);
    void updateLeader(string filename, string playername, int time);

};




