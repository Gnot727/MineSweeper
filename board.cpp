//
// Created by Goran on 7/28/2024.
//

#include "board.h"
#include <set>
#include <cstdlib>
#include<ctime>
#include <random>
#include <chrono>
#include "screens.h"


Board::Board(int col, int row,sf::RenderWindow &gameScr,sf::Sprite &hidden, sf::Sprite &revealed):
        window(gameScr){
    columns = col;
    rows = row;
    mineCount = 0;

    ifstream configFile("files/config.cfg");
    if(!configFile.is_open()){
        cout<<"Config file not opened" <<endl;
    }
    configFile >>col>>row >> mineCount;
    cout<< "row:" <<row<< "|" <<"col:" <<col<<endl;
    tiles.resize(row,vector<Tile*>(col,nullptr));

    /*resize(4,vector<Tile*>(4,nullptr))
     * {{nullptr},{nullptr},{nullptr},{nullptr}}
     * {{nullptr},{nullptr},{nullptr},{nullptr}}
     * {{nullptr},{nullptr},{nullptr},{nullptr}}
     * {{nullptr},{nullptr},{nullptr},{nullptr}}
     */
    for(int i = 0; i < row; i++){
        for(int j = 0; j<col; j++){
            tiles[i][j] = new Tile(gameScr,hidden,revealed);
            //{0,0}{1,0}{2,0}{3,0}...
        }
    }
}


Board::~Board() {
    for(auto row : tiles) {
        for (auto col: row) {
            delete col;
        }
    }
}



void Board::clicked(int x, int y) {
    if(!tiles[y][x] -> isFlagged()) {
        tiles[y][x]->reveal();
    }
}

void Board::drawBoard(sf::RenderWindow &gameScr,sf::Sprite &revealed, sf::Sprite &hidden,sf::Sprite &mine) {
    int col;
    int row;
    sf::Texture number8;
    if(!number8.loadFromFile("files/images/number_8.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite number8Sprite;
    number8Sprite.setTexture(number8);


    sf::Texture number7;
    if(!number7.loadFromFile("files/images/number_7.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite number7Sprite;
    number7Sprite.setTexture(number7);

    sf::Texture number6;
    if(!number6.loadFromFile("files/images/number_6.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite number6Sprite;
    number6Sprite.setTexture(number6);

    sf::Texture number5;
    if(!number5.loadFromFile("files/images/number_5.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite number5Sprite;
    number5Sprite.setTexture(number5);

    sf::Texture number4;
    if(!number4.loadFromFile("files/images/number_4.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite number4Sprite;
    number4Sprite.setTexture(number4);

    sf::Texture number3;
    if(!number3.loadFromFile("files/images/number_3.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite number3Sprite;
    number3Sprite.setTexture(number3);

    sf::Texture number2;
    if(!number2.loadFromFile("files/images/number_2.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite number2Sprite;
    number2Sprite.setTexture(number2);

    sf::Texture number1;
    if(!number1.loadFromFile("files/images/number_1.png")){
        cout<<"Texture File not Loaded"<<endl;
    }
    sf::Sprite number1Sprite;
    number1Sprite.setTexture(number1);

    ifstream configFile("files/config.cfg");
    if(!configFile.is_open()){
        cout<<"Config file not opened" <<endl;
    }
    configFile >>col>>row;

    for(int i = 0 ; i< row; i++) {
        for (int j = 0; j < col; j++) {

            if (!tiles[i][j] -> returnHidden()) {
                revealed.setPosition(j * 32, i * 32);
                gameScr.draw(revealed);

                if (tiles[i][j]->value == -1) {
                    mine.setPosition(j * 32,i * 32);
                    gameScr.draw(mine);
                }
                if(tiles[i][j] -> value == 1){
                    number1Sprite.setPosition(j*32,i*32);
                    gameScr.draw(number1Sprite);
                }
                if(tiles[i][j] -> value == 2){
                    number2Sprite.setPosition(j*32,i*32);
                    gameScr.draw(number2Sprite);
                }
                if(tiles[i][j] -> value == 3){
                    number3Sprite.setPosition(j*32,i*32);
                    gameScr.draw(number3Sprite);
                }
                if(tiles[i][j] -> value == 4){
                    number4Sprite.setPosition(j*32,i*32);
                    gameScr.draw(number4Sprite);
                }
                if(tiles[i][j] -> value == 5){
                    number5Sprite.setPosition(j*32,i*32);
                    gameScr.draw(number5Sprite);
                }
                if(tiles[i][j] -> value == 6){
                    number6Sprite.setPosition(j*32,i*32);
                    gameScr.draw(number6Sprite);
                }
                if(tiles[i][j] -> value == 7){
                    number7Sprite.setPosition(j*32,i*32);
                    gameScr.draw(number7Sprite);
                }
                if(tiles[i][j] -> value == 8){
                    number8Sprite.setPosition(j*32,i*32);
                    gameScr.draw(number8Sprite);
                }
            } else {
                hidden.setPosition(j * 32, i * 32);
                gameScr.draw(hidden);
            }

        }
    }

}

void Board::setMines() {
    int mines = 0;
    int newRow;
    int cols;
    ifstream configFile("files/config.cfg");
    if(!configFile.is_open()){
        cout<<"Config file not opened" <<endl;
    }
    configFile >>cols>>newRow >> mineCount;
    while(mines < mineCount){
        int row = rand() % newRow;
        int col = rand() % cols;
        if (tiles[row][col]->value != -1) {
            tiles[row][col]->value = -1;
            mines++;
            tiles[row][col] -> setMine();
        }
    }
}

void Board::setNeighbors() {
    int col;
    int row;
    ifstream configFile("files/config.cfg");
    if(!configFile.is_open()){
        cout<<"Config file not opened" <<endl;
    }
    configFile >>col>>row >> mineCount;
    for (int x = 0; x < col; x++) {
        for (int y = 0; y < row; y ++) {

            //top left
            if (y == 0 && x == 0) {
                tiles[y][x]->neighbors.push_back(tiles[y+1][x]);
                tiles[y][x]->neighbors.push_back(tiles[y][x+1]);
                tiles[y][x]->neighbors.push_back(tiles[y+1][x+1]);
            }
                //top right
            else if (x == 24 && y == 0){
                tiles[y][x]->neighbors.push_back(tiles[y][x-1]);
                tiles[y][x] ->neighbors.push_back(tiles[y+1][x]);
                tiles[y][x]->neighbors.push_back(tiles[y+1][x-1]);
            }

                //bottom left
            else if(x==0 && y== 15){
                tiles[y][x]->neighbors.push_back(tiles[y][x+1]);
                tiles[y][x]->neighbors.push_back(tiles[y-1][x]);
                tiles[y][x]->neighbors.push_back(tiles[y-1][x+1]);
            }

                //bottom right
            else if(x == 24 && y == 15){
                tiles[y][x]->neighbors.push_back(tiles[y-1][x]);
                tiles[y][x]->neighbors.push_back(tiles[y-1][x-1]);
                tiles[y][x]->neighbors.push_back(tiles[y][x-1]);
            }

                //top row
            else if(y==0){
                tiles[y][x]->neighbors.push_back(tiles[y][x+1]);
                tiles[y][x]->neighbors.push_back(tiles[y+1][x]);
                tiles[y][x]->neighbors.push_back(tiles[y+1][x+1]);
                tiles[y][x]->neighbors.push_back(tiles[y+1][x-1]);
                tiles[y][x]->neighbors.push_back(tiles[y][x-1]);
            }

                //right row
            else if(x==24){
                tiles[y][x]->neighbors.push_back(tiles[y][x-1]);
                tiles[y][x]->neighbors.push_back(tiles[y-1][x-1]);
                tiles[y][x]->neighbors.push_back(tiles[y-1][x]);
                tiles[y][x]->neighbors.push_back(tiles[y+1][x]);
                tiles[y][x]->neighbors.push_back(tiles[y+1][x-1]);
            }


                //bottom row
            else if (y==15){
                tiles[y][x]->neighbors.push_back(tiles[y][x-1]);
                tiles[y][x]->neighbors.push_back(tiles[y-1][x+1]);
                tiles[y][x]->neighbors.push_back(tiles[y][x+1]);
                tiles[y][x]->neighbors.push_back(tiles[y-1][x-1]);
                tiles[y][x]->neighbors.push_back(tiles[y-1][x]);
            }
                //left row
            else if(x == 0){
                tiles[y][x]->neighbors.push_back(tiles[y-1][x]);
                tiles[y][x]->neighbors.push_back(tiles[y-1][x+1]);
                tiles[y][x]->neighbors.push_back(tiles[y][x+1]);
                tiles[y][x]->neighbors.push_back(tiles[y+1][x+1]);
                tiles[y][x]->neighbors.push_back(tiles[y+1][x]);
            }
            else{
                tiles[y][x]->neighbors.push_back(tiles[y+1][x+1]);
                tiles[y][x]->neighbors.push_back(tiles[y][x+1]);
                tiles[y][x]->neighbors.push_back(tiles[y-1][x+1]);
                tiles[y][x]->neighbors.push_back(tiles[y][x-1]);
                tiles[y][x]->neighbors.push_back(tiles[y-1][x-1]);
                tiles[y][x]->neighbors.push_back(tiles[y+1][x-1]);
                tiles[y][x]->neighbors.push_back(tiles[y-1][x]);
                tiles[y][x]->neighbors.push_back(tiles[y+1][x]);
            }
            if (tiles[y][x] -> value == -1){
                for(auto tile: tiles[y][x] -> neighbors){
                    if (tile->value != -1)
                        tile ->value +=1;
                }
            }
        }
    }
}

void Board::chainReaction(sf::RenderWindow &gameScr, Tile* tile) {
    if(tile -> value > 0){
        return;
    }
    if(tile ->value < 0){
        return;
    }
    if(tile ->isFlagged()){
        return;
    }
    else {
        for (auto& t : tile->neighbors) {
            if(!t->returnHidden() || t->isFlagged()){
                continue;
            }
            if (t->value == 0 && t->returnHidden()) {
                t->reveal();
                t->setClicked();
                chainReaction(gameScr, t);
            }
            else {
                t->setClicked();
                t->reveal();
            }
        }
    }
}

void Board::changeHappyFace(sf::Sprite &faceSprite) {
    faceSprite.setPosition((25/2.0f*32)-32, 32*(16 +.5f));
}
void Board::changePauseButton(sf::Sprite &playSprite) {
    playSprite.setPosition((25*32)-240,32*(16+0.5f));
}

void Board::drawLose(sf::RenderWindow &gameScr,sf::Sprite &loseFace){
    gameScr.draw(loseFace);
}
void Board::boolClicked(int x, int y) {
    tiles[y][x] -> setClicked();

}

void Board::drawFlag(sf::RenderWindow &gameScr, sf::Sprite &flag) {
    int row;
    int col;
    ifstream configFile("files/config.cfg");
    if(!configFile.is_open()){
        cout<<"Config file not opened" <<endl;
    }
    configFile >>col>>row >> mineCount;

    for (int x = 0; x < col; x++) {
        for (int y = 0; y < row; y ++) {
            if(tiles[y][x] -> isFlagged()){
                flag.setPosition(x * 32,y * 32);
                gameScr.draw(flag);
            }
        }
    }
}

void Board::displayMines(sf::RenderWindow &gameScr,sf::Sprite &mine) {
    int row;
    int col;
    ifstream configFile("files/config.cfg");
    if(!configFile.is_open()){
        cout<<"Config file not opened" <<endl;
    }
    configFile >>col>>row >> mineCount;

    for (int x = 0; x < col; x++) {
        for (int y = 0; y < row; y ++) {
            if(tiles[y][x] -> value == -1){
                mine.setPosition(x*32,y*32);
                gameScr.draw(mine);
            }
        }
    }
}


void Board::resetBoard(sf::RenderWindow &gameScr, sf::Sprite &hidden, sf::Sprite &revealed) {
    int row;
    int col;
    ifstream configFile("files/config.cfg");
    if(!configFile.is_open()){
        cout<<"Config file not opened" <<endl;
    }
    configFile >>col>>row >> mineCount;
    for(auto row : tiles) {
        for (auto col: row) {
            delete col;
        }
    }

    tiles = vector<vector<Tile *>>(row, vector<Tile *>(col, nullptr));

    for(int i = 0; i < row; i++){
        for(int j = 0; j<col; j++){
            tiles[i][j] = new Tile(gameScr,hidden,revealed);
            //{0,0}{1,0}{2,0}{3,0}...
        }
    }

    setMines();
    setNeighbors();
}

void Board::drawCounter(sf::RenderWindow &gameScr, sf::Sprite &digits, int &placedFlags) {
    int row;
    int col;
    ifstream configFile("files/config.cfg");
    if(!configFile.is_open()){
        cout<<"Config file not opened" <<endl;
    }
    configFile >>col>>row;

    string counterValue = to_string(placedFlags);

    bool negative = false;
    if(placedFlags < 0){
        negative = true;
    }
    float digitSize = 21.0f;
    if(negative){

        digits.setTextureRect(sf::IntRect(10*digitSize,0,digitSize,32));
        digits.setPosition(12, 32 *(row + 0.5) + 16);
        gameScr.draw(digits);

    }
    for(int i = 0; i <counterValue.size();i++){
        unsigned char charDigit = counterValue[i];
        int currDigit = charDigit - '0';

        digits.setTextureRect(sf::IntRect(currDigit *digitSize,0,digitSize,32));
        digits.setPosition(33 + i *digitSize,32*(row+0.5)+16);
        gameScr.draw(digits);
    }
}

bool Board::checkIfWin() {
    int row;
    int col;
    int currentMines;
    ifstream configFile("files/config.cfg");
    if (!configFile.is_open()) {
        cout << "Config file not opened" << endl;
        return false;
    }
    configFile >> col >> row >> currentMines;


    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (!tiles[i][j]->isMine() && !tiles[i][j]->isClicked()) {
                return false;
            }
        }
    }
        return true;
    }

void Board::startClock() {
    if(!runClock){
        initTime = chrono::high_resolution_clock ::now();
        runClock = true;
        paused = false;
    }
}

int Board::getElapsedTime() {
    if(runClock){
        auto currentTime = chrono::high_resolution_clock ::now();
        auto elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime-initTime);
        int elapsedSeconds = elapsedTime.count();
        if(paused){
            return elapsedSeconds;
        }
        else{
            return totalTime +elapsedSeconds;
        }
    }
    return totalTime;
}

void Board::setClock(sf::RenderWindow &gameScr, sf::Sprite &digits) {
    int col;
    int row;
    ifstream configFile("files/config.cfg");
    if (!configFile.is_open()) {
        cout << "Config file not opened" << endl;

    }
    configFile >> col >> row;

            int totalseconds = getElapsedTime();


            int minutes = totalseconds / 60;
            int seconds = totalseconds % 60;

            float digitSize = 21;

            string time = to_string(minutes * 100 + seconds);

            while (time.size() < 4) {
                time = '0' + time;
            }
            cout << "time" << " " << time << endl;
            for (int i = 0; i < 2; i++) {
                unsigned char charDigit = time[i];
                int currDigit = charDigit - '0';

                digits.setTextureRect(sf::IntRect(currDigit * digitSize, 0, digitSize, 32));
                digits.setPosition((col * 32) - 97 + i * digitSize, 32 * (row + 0.5) + 16);
                gameScr.draw(digits);
            }
            for(int i = 2; i<time.size();i++){
                unsigned char charDigit = time[i];
                int currDigit = charDigit - '0';

                digits.setTextureRect(sf::IntRect(currDigit*digitSize,0,digitSize,32));
                digits.setPosition((col * 32) - 95 + (i * digitSize), 32 * (row + 0.5) + 16);
                gameScr.draw(digits);
            }
        }


void Board::setPaused() {

    if(runClock && !paused){
        auto pauseTime = chrono::high_resolution_clock ::now();
        auto elapsedTime = chrono::duration_cast<chrono::seconds>(pauseTime-initTime);
        totalTime +=elapsedTime.count();
        paused = true;
        runClock = false;
    }
}

void Board::resetClock() {
    paused = false;
    runClock = false;
    totalTime = 0;
    initTime = chrono::high_resolution_clock ::now();
    runClock = true;
    }

void Board::setPausedScreen(sf::RenderWindow &gameScr, sf::Sprite &revealed,sf::Sprite &playButton) {
if(paused){
    int row;
    int col;
    int currentMines;
    ifstream configFile("files/config.cfg");
    if (!configFile.is_open()) {
        cout << "Config file not opened" << endl;
    }
    configFile >> col >> row >> currentMines;
    for(int i = 0;i<row;i++){
        for(int j =0; j<col;j++){
            revealed.setPosition(j*32,i*32);
            gameScr.draw(revealed);
        }
    }
    playButton.setPosition((25*32)-240,32*(16+0.5f));
    gameScr.draw(playButton);
}
}

void Board::setFlaggedWin(sf::RenderWindow &gameScr, sf::Sprite &flag) {
    int row;
    int col;
    int currentMines;
    ifstream configFile("files/config.cfg");
    if (!configFile.is_open()) {
        cout << "Config file not opened" << endl;

    }
    configFile >> col >> row >> currentMines;


    for (int x = 0; x < col; x++) {
        for (int y = 0; y < row; y ++) {
            if(tiles[y][x] -> value == -1){
                flag.setPosition(x * 32,y * 32);
                gameScr.draw(flag);
            }
        }
    }

}

void Board::setLoseMines(sf::RenderWindow &gameScr, sf::Sprite &mine) {
    int row;
    int col;
    int currentMines;
    ifstream configFile("files/config.cfg");
    if (!configFile.is_open()) {
        cout << "Config file not opened" << endl;

    }
    configFile >> col >> row >> currentMines;


    for (int x = 0; x < col; x++) {
        for (int y = 0; y < row; y++) {
            if (tiles[y][x]->value == -1) {
                mine.setPosition(x * 32, y * 32);
                gameScr.draw(mine);
            }
        }
    }


}

void Board::openLeader() {
leaderboardOpen = true;
}

void Board::setLeaderboardScreen(sf::RenderWindow &gameScr, sf::Sprite &revealed) {

        int row;
        int col;
        int currentMines;
        ifstream configFile("files/config.cfg");
        if (!configFile.is_open()) {
            cout << "Config file not opened" << endl;
        }
        configFile >> col >> row >> currentMines;
        configFile.close();
        for(int i = 0;i<col;i++){
            for(int j =0; j<row;j++){
                revealed.setPosition(i*32,j*32);
                gameScr.draw(revealed);
            }
        }
    }











