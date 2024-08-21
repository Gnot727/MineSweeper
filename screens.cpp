//
// Created by Goran on 7/25/2024.
//
#include <iostream>
#include <vector>
#include<string>
#include<cmath>
#include<fstream>
#include <sstream>
#include <random>
#include <SFML/Graphics.hpp>
#include "screens.h"
#include <chrono>
#include "board.h"
#include "tile.h"

using namespace std;


void Window::setTextPosition(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}
void write(string filename, string name){
    fstream outputFile(filename, ios::out);
    if (!outputFile) {
        cout << "File not opened" << endl;
        return;
    }
    outputFile<<name;
    outputFile.close();
}

Window::Window(int height, int width, string title, sf::Color color) :
window(sf::VideoMode(width,height),title,sf::Style::Close), mineCount(0){

    ifstream configFile("files/config.cfg");
    if(!configFile.is_open()){
        cout<<"config file not opened"<<endl;
    }
    else{
        configFile >> width>> height >> mineCount;
    }
    cout<<"Width:" <<width<<"height:" <<height<<endl;
    if(!font.loadFromFile("files/font.ttf")){
        cout<<"Font File is not open"<<endl;
    }
    titleText.setFont(font);
    inputText.setFont(font);
    userText.setFont(font);
    leaderBoardText.setFont(font);

}
void Window::setText(sf::Text &text, string letters, int charSize, sf::Color strColor, float x, float y){
    text.setString(letters);
    text.setCharacterSize(charSize);
    text.setFillColor(strColor);
    setTextPosition(text,x,y);
}

WelcomeScreen::WelcomeScreen(int height, int width, string letters, sf::Color color) :
Window(height*32 + 100, width *32, letters, color) {

setText(titleText, "WELCOME TO MINESWEEPER!", 24, sf::Color::White, window.getSize().x / 2, window.getSize().y / 2 - 150);
titleText.setStyle(sf::Text::Bold);
titleText.setStyle(sf::Text::Underlined);

setText(inputText,"Enter your name:",20, sf::Color::White,window.getSize().x/2,window.getSize().y/2 - 75);
inputText.setStyle(sf::Text::Bold);

setText(userText,"",18,sf::Color::Yellow,window.getSize().x/2,window.getSize().y/2 -45);
userText.setStyle(sf::Text::Bold);

}

string input;

bool WelcomeScreen::runScreen()  {
    int count = 0;
    bool gameStart = false;
    string cursor = "|";
    string displayInput;
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::TextEntered) {

                cout <<  event.text.unicode<<endl;

                char entered = (char) event.text.unicode;
                if(isalpha(entered) && input.size()<10 && input.size() >= 0){
                    if(count == 0){
                        entered = toupper(entered);
                        input+= entered;
                        count++;
                    }
                    else{
                        entered = tolower(entered);
                        input+=entered;
                    }
                    displayInput =input +cursor;
                    userText.setString(displayInput);
                    setTextPosition(userText,window.getSize().x/2 ,window.getSize().y/2 -45);
                }
                //store name to put in leaderboard


                if(event.text.unicode == sf::Keyboard::Enter || event.text.unicode == 13 && input.size() >= 1 && input.size()<=10){
                    window.close();
                    gameStart = true;
                }


                if ((event.text.unicode == sf::Keyboard::BackSpace || event.text.unicode == 8) && input.length()>=1) {
                    input.erase(input.length()-1,1);
                    displayInput = input +cursor;
                    userText.setString(displayInput);
                    if(input.length() == 0){
                        count = 0;
                    }
                }

                if(event.text.unicode == sf::Keyboard::Space || event.text.unicode == 32 && input.length() <10){

                    input+= " ";
                    displayInput = input +cursor;
                    userText.setString(displayInput);
                }
            }
            string name = input;
        }

        window.clear(sf::Color::Blue);
        window.draw(titleText);
        window.draw(inputText);
        window.draw(userText);

        window.display();
    }
    if(gameStart){
        return true;
    }
    return false;

}
LeaderBoard::LeaderBoard(int height, int width, std::string title, sf::Color color):
        Window(height *16 + 50, width*16, (title), color){
    setText(leaderBoardText,"LEADERBOARD",20,sf::Color::White,width/2,height/2-120);
    titleText.setStyle(sf::Text::Bold);
    titleText.setStyle(sf::Text::Underlined);

}

void LeaderBoard::updateLeader(string filename, string playername, int time) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "Could not open file" << endl;
        return;
    }

    timeName.clear();

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string timeStr, name;
        if (getline(ss, timeStr, ',') && getline(ss, name)) {
            int existingTime = stoi(timeStr);
            timeName[name] = existingTime;
        }
    }
    inFile.close();

    timeName[playername] = time;

    vector<pair<int, string>> leaderboard;
    for (const auto& entry : timeName) {
        leaderboard.push_back({entry.second, entry.first});
    }

    sort(leaderboard.begin(), leaderboard.end());

    timeName.clear();
    for (int i = 0; i < leaderboard.size() && i < 5; ++i) {
        timeName[leaderboard[i].second] = leaderboard[i].first;
    }

    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "Could not open file for writing" << endl;
        return;
    }

    for (const auto& entry : timeName) {
        outFile << entry.second << "," << entry.first << endl;
    }
    outFile.close();
}


void LeaderBoard::drawLeader(string filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "Could not open file" << endl;
        return;
    }

    timeName.clear();
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string timeStr, name;
        if (getline(ss, timeStr, ',') && getline(ss, name)) {
            int existingTime = stoi(timeStr);
            timeName[name] = existingTime;
        }
    }
    inFile.close();

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        cout << "Font File is not open" << endl;
        return;
    }

    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("LEADERBOARD");
    titleText.setStyle(sf::Text::Bold);
    titleText.setStyle(sf::Text::Underlined);
    titleText.setFillColor(sf::Color::White);
    titleText.setCharacterSize(20);
    setTextPosition(titleText, window.getSize().x / 2.0f, (window.getSize().y / 2) - 120);
    window.draw(titleText);

    // Display leaderboard sorted by time
    string leaderboard;
    int count = 1;
    for (const auto& entry : timeName) {
        leaderboard += to_string(count) + "." + "\t" + to_string(entry.second) + "\t" + entry.first + "\n\n";
        count++;
    }

    sf::Text leaderText;
    leaderText.setFont(font);
    leaderText.setString(leaderboard);
    leaderText.setStyle(sf::Text::Bold);
    leaderText.setCharacterSize(18);
    leaderText.setFillColor(sf::Color::White);
    setTextPosition(leaderText, window.getSize().x / 2.0f, (window.getSize().y / 2) + 20);
    window.draw(leaderText);
}

bool LeaderBoard::runScreen() {
    while (window.isOpen()) {
        string name;
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return false;
            }
        }
        window.clear(sf::Color::Blue);

        drawLeader("files/leaderboard.txt");
        window.display();
    }
    return false;
}




gameScreen::gameScreen(int height, int width, string title, sf::Color color) :
Window(height *32 + 100, width*32, (title), color) {

}
bool gameScreen::runScreen() {
    bool openLeaderboard = false;
    bool isFlag = false;
    bool isMine = false;
    bool gameEnd= false;
    bool debug = false;
    bool win = false;
    bool paused = false;
    int row;
    int col;
    ifstream configFile("files/config.cfg");
    if(!configFile.is_open()){
        cout<<"Config file not opened" <<endl;
    }
    configFile >>col>>row >> mineCount;

    sf::Texture hiddenTile;
    if(!hiddenTile.loadFromFile("files/images/tile_hidden.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite hiddenSprite;
    hiddenSprite.setTexture(hiddenTile);

    sf::Texture revealedTile;
    if(!revealedTile.loadFromFile("files/images/tile_revealed.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite revealedSprite;
    revealedSprite.setTexture(revealedTile);

    sf::Texture playButton;
    if(!playButton.loadFromFile("files/images/play.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite playButtonSprite;
    playButtonSprite.setTexture(playButton);

    sf::Texture pauseButton;
    if(!pauseButton.loadFromFile("files/images/pause.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite pauseButtonSprite;
    pauseButtonSprite.setTexture(pauseButton);

    sf::Texture mine;
    if(!mine.loadFromFile("files/images/mine.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite mineSprite;
    mineSprite.setTexture(mine);

    sf::Texture leaderboardButton;
    if(!leaderboardButton.loadFromFile("files/images/leaderboard.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite leaderboardSprite;
    leaderboardSprite.setTexture(leaderboardButton);

    sf::Texture flag;
    if(!flag.loadFromFile("files/images/flag.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite flagSprite;
    flagSprite.setTexture(flag);

    sf::Texture winFace;
    if(!winFace.loadFromFile("files/images/face_win.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite winFaceSprite;
    winFaceSprite.setTexture(winFace);

    sf::Texture loseFace;
    if(!loseFace.loadFromFile("files/images/face_lose.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite loseFaceSprite;
    loseFaceSprite.setTexture(loseFace);

    sf::Texture happyFace;
    if(!happyFace.loadFromFile("files/images/face_happy.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite happyFaceSprite;
    happyFaceSprite.setTexture(happyFace);

    sf::Texture digits;
    if(!digits.loadFromFile("files/images/digits.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite digitsSprite;
    digitsSprite.setTexture(digits);

    sf::Texture debugButton;
    if(!debugButton.loadFromFile("files/images/debug.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite debugButtonSprite;
    debugButtonSprite.setTexture(debugButton);

    happyFaceSprite.setPosition((25/2.0f*32)-32, 32*(16 +.5f));
    debugButtonSprite.setPosition((25*32)-304,32*(16+0.5f));
    pauseButtonSprite.setPosition((25*32)-240,32*(16+0.5f));

    Board board(height,width,window,hiddenSprite,revealedSprite);
    Tile tile(window,hiddenSprite,revealedSprite);
    board.setMines();
    board.setNeighbors();
   // board.debug();
    int activeMines = mineCount;
    bool pausedLeader = false;
    while (window.isOpen()) {
        sf::Event event;
        //cout<<"active mines:"<<activeMines;

        while (window.pollEvent(event)) {

            if (board.checkIfWin()) {
                gameEnd = true;
                win = true;
                openLeaderboard = true;
                board.openLeader();
                board.setPaused();
                board.changeHappyFace(winFaceSprite);

                auto elapsedTime = board.getElapsedTime();
                string playerName = input;


            }

            if (event.type == sf::Event::MouseButtonPressed) {


                if (event.mouseButton.button == sf::Mouse::Left) {

                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    if (mousePosition.x <= window.getSize().x && mousePosition.y <= window.getSize().y) {
                        int x = mousePosition.x / 32;
                        int y = mousePosition.y / 32;
                        if (debugButtonSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                            //cout<<"Debug button"<<endl;
                            debug = !debug;
                        }
                        if (happyFaceSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                            activeMines = mineCount;
                            gameEnd = false;
                            debug = false;
                            board.resetBoard(window, hiddenSprite, revealedSprite);
                            board.resetClock();


                        }
                        /*if (leaderboardSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                            pausedLeader = !pausedLeader;
                            openLeaderboard = true;
                            board.openLeader();
                            board.setPaused();
                        }*/
                        if (pauseButtonSprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                            //board.setPausedScreen();
                            paused = !paused;
                            board.changePauseButton(playButtonSprite);
                            board.setPaused();
                            if (!paused) {
                                board.startClock();
                            }
                        }


                        if (x >= 0 && x <= col && y >= 0 && y <= row && !gameEnd) {
                            if (!gameEnd) {
                                board.startClock();
                            }
                            board.clicked(x, y);
                            board.boolClicked(x, y);

                            if (board.tiles[y][x]->value == -1 && board.tiles[y][x]->isClicked()) {
                                gameEnd = true;
                                board.setPaused();

                                board.changeHappyFace(loseFaceSprite);
                            }

                            Tile *temp = board.tiles[y][x];
                            board.chainReaction(window, temp);
                        }
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    if (mousePosition.x <= window.getSize().x && mousePosition.y <= window.getSize().y){
                    int x = mousePosition.x / 32;
                    int y = mousePosition.y / 32;

                    if (x >= 0 && x <= col && y >= 0 && y <= row) {
                        // Set clicked here
                        if (!board.tiles[y][x]->isFlagged() && !board.tiles[y][x]->isClicked()) {
                            board.tiles[y][x]->setFlagged();
                            activeMines = activeMines - 1;
                        } else if (board.tiles[y][x]->isFlagged() && activeMines < mineCount) {
                            board.tiles[y][x]->setFlaggedFalse();
                            activeMines = activeMines + 1;
                        }
                    }
                    cout << activeMines << endl;
                }
}
            }

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(sf::Color::White);
        board.setClock(window, digitsSprite);
        board.drawBoard(window, revealedSprite, hiddenSprite, mineSprite);
        board.drawFlag(window, flagSprite);
        if (gameEnd) {
            if (win) {
                window.draw(winFaceSprite);
                board.setFlaggedWin(window, flagSprite);
            } else {
                board.drawLose(window, loseFaceSprite);
                board.setLoseMines(window, mineSprite);
            }

        } else {
            window.draw(happyFaceSprite);
        }
        window.draw(debugButtonSprite);
        if (debug) {
            board.displayMines(window, mineSprite);
        }
        if (paused) {
            window.draw(playButtonSprite);
            board.setPausedScreen(window, revealedSprite, playButtonSprite);
        } else {
            window.draw(pauseButtonSprite);
        }
        if (openLeaderboard && pausedLeader) {
            board.setLeaderboardScreen(window, revealedSprite);



                LeaderBoard leaderboard(16, 25, "Leaderboard", sf::Color::Blue);
                openLeaderboard = leaderboard.runScreen();

                if (!openLeaderboard) {
                    pausedLeader = false;
                    board.startClock();
                }

        }
            board.drawCounter(window,digitsSprite,activeMines);

            window.display();

    }
    return false;
}



