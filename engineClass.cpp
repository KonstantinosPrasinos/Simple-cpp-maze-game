#include "engineClass.h"

Engine::Engine(){
    //Map Reading and assignment to vector of strings
    string mapLine;
    ifstream mapFile ("map.txt");
    if (mapFile.is_open()){
        while (getline(mapFile, mapLine)){
            if (mapLine.size() > 0){
                Map.push_back(mapLine);
            }
        }
    }

    Round = 0;
    Score = 0;

    placePawnsInRandomPositions();
}

Engine::~Engine(){

}

vector<string> Engine::getMap(){
    return Map;
}

void Engine::printCoordinates(){
}

int Engine::checkCollision(int x, int y){
    if(Map.at(y).at(x) != '*'){
        return 1;
    } else {
        return 0;
    }
}

string Engine::checkGameState(int x, int y){
    if (poter.getPositionX() == traal.getPositionX() && poter.getPositionY() == traal.getPositionY()){
        return "lose";
    } else if (poter.getPositionX() == gnome.getPositionX() && gnome.getPositionY() == gnome.getPositionY()){
        return "lose";
    }

    for (int i = 0; i < 10; i++){
        if (poter.getPositionX() == jewels[i].getPositionX() && poter.getPositionY() == jewels[i].getPositionY()){
            Score = Score + 10;
            return "collected";
        }
    }

    if (poter.getPositionX() == scroll.getPositionX() && poter.getPositionY() == scroll.getPositionY()){
        Score = Score + 100;
        return "win";
    }

    return "continue";
}

void Engine::printMap(){
    //Printing the map
    for (int i = 0; i < Map.size(); i++){
        printw(Map[i].data());
        printw("\n");
    }
    refresh();
}

void Engine::placePawnsInRandomPositions(){
    //Random Number Generator
    random_device rd;
    mt19937 eng(rd());
    uniform_int_distribution<int> distrX(1, 9); //Remember to change the limits for the size of the new map and make it dynamic not a single int
    uniform_int_distribution<int> distrY(1, 5); //Remember to change the limits for the size of the new map and make it dynamic not a single int
    if (Round == 0){
        //Place Player on Map
        int valid = 0;
        int x, y;
        do
        {
            x = distrX(eng);
            y = distrY(eng);
            valid = checkCollision(x, y);
        } while (valid == 0);
        Poter tempPoter(x, y);
        Map.at(y).at(x) = 'P';
        poter = tempPoter;

        //Place Jewels on Map
        for(int i = 0; i < 10; i++){
            valid = 0;
            x = y = 0;
            do
            {
                x = distrX(eng);
                y = distrY(eng);
                valid = checkCollision(x, y);
                if (x == poter.getPositionX() && y == poter.getPositionY())
                {
                    valid = 0;
                } else if (i != 0){
                    for (int j = 0; j < jewels.size() - 1; j++){
                        if (x == jewels[j].getPositionX() && y == jewels[j].getPositionY()){
                            valid = 0;
                        }
                    }
                }
            } while (valid == 0);
            Jewel tempJewel(x, y);
            jewels.push_back(tempJewel);
            Map.at(y).at(x) = 'J';
        }
    } else {
        int valid = 0;
        int x = 0;
        int y = 0;
        do{
            x = distrX(eng);
            y = distrY(eng);
            valid = checkCollision(x, y);
        } while (valid == 0);
        Scroll tempScroll(x, y);
        scroll = tempScroll;
        Map.at(y).at(x) = 'S';
    }
}

void Engine::coordinateMovements(){
    int valid, validKeyPress = 0;
    
    do{
        int input = poter.determineMovement();
        switch (input){
        case KEY_LEFT:
            valid = checkCollision(poter.getPositionX() - 1, poter.getPositionY());
            if (valid == 1){
                Map.at(poter.getPositionY()).erase(Map.at(poter.getPositionY()).begin() + poter.getPositionX());
                Map.at(poter.getPositionY()).erase(Map.at(poter.getPositionY()).begin() + poter.getPositionX() - 1);
                Map.at(poter.getPositionY()).insert(Map.at(poter.getPositionY()).begin() + poter.getPositionX() - 1, 'P');
                Map.at(poter.getPositionY()).insert(Map.at(poter.getPositionY()).begin() + poter.getPositionX(), ' ');
                poter.MoveLeft();
                validKeyPress = 1;
            }
            break;
        case KEY_RIGHT:
            valid = checkCollision(poter.getPositionX() + 1, poter.getPositionY());
            if (valid == 1){
                Map.at(poter.getPositionY()).erase(Map.at(poter.getPositionY()).begin() + poter.getPositionX() + 1);
                Map.at(poter.getPositionY()).erase(Map.at(poter.getPositionY()).begin() + poter.getPositionX());
                Map.at(poter.getPositionY()).insert(Map.at(poter.getPositionY()).begin() + poter.getPositionX(), ' ');
                Map.at(poter.getPositionY()).insert(Map.at(poter.getPositionY()).begin() + poter.getPositionX() + 1, 'P');
                poter.MoveRight();
                validKeyPress = 1;
            }
            break;
        case KEY_UP:
            valid = checkCollision(poter.getPositionX(), poter.getPositionY() - 1);
            if (valid == 1){
                Map.at(poter.getPositionY()).erase(Map.at(poter.getPositionY()).begin() + poter.getPositionX());
                Map.at(poter.getPositionY()).insert(Map.at(poter.getPositionY()).begin() + poter.getPositionX(), ' ');
                Map.at(poter.getPositionY() - 1).erase(Map.at(poter.getPositionY()).begin() + poter.getPositionX());
                Map.at(poter.getPositionY() - 1).insert(Map.at(poter.getPositionY()).begin() + poter.getPositionX(), 'P');
                poter.MoveUp();
                validKeyPress = 1;
            }
            break;
        case KEY_DOWN:
            valid = checkCollision(poter.getPositionX(), poter.getPositionY() + 1);
            if (valid == 1){
                Map.at(poter.getPositionY()).erase(Map.at(poter.getPositionY()).begin() + poter.getPositionX());
                Map.at(poter.getPositionY()).insert(Map.at(poter.getPositionY()).begin() + poter.getPositionX(), ' ');
                Map.at(poter.getPositionY() + 1).erase(Map.at(poter.getPositionY()).begin() + poter.getPositionX());
                Map.at(poter.getPositionY() + 1).insert(Map.at(poter.getPositionY()).begin() + poter.getPositionX(), 'P');
                poter.MoveDown();
                validKeyPress = 1;
            }
            break;
        case ' ':
            validKeyPress = 1;
            break;
        case 27:
            validKeyPress = 1;
            break;
        default:
            validKeyPress = 0;
            break;
        }
    } while (validKeyPress == 0);
}

void Engine::nextRound(){
    coordinateMovements();
    clear();
    printMap();
    refresh();
    cout<<Round<<" ";;
}