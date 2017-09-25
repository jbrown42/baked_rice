//
// Created by jared on 9/24/17.
//
#include <iostream>
#include <ctime>
#include "World.h"
using namespace std;

int World::mapHeight;
int World::mapWidth;

/*coordinates stored (y,x)
  top left is (0,0)
  bottom left is (mapHeight-1,0)*/
map<int,vector<string>> World::world;

void World::createWorld(int size) {
    mapHeight = size;
    mapWidth = size * 2;
    for(int i = 0; i < mapHeight; ++i){
        for(int j = 0; j < mapWidth*2; ++j) {
            world[i].push_back("~");
        }
    }
    world[mapHeight-1][0] = "A"; //drone airport
}

void World::printMap() {
    for(int i = 0; i < mapHeight; ++i) {
        for (int j = 0; j < mapWidth*2; ++j) {
            cout<<" "<<world[i][j]<<" ";
        }
        cout<<endl;
    }
}

queue<pair<int,int>> World::generatePath(){
    queue<pair<int,int>> ret;
    pair<int,int> curPair; //coordinates stored (y,x)
    bool firstPos = true;

    srand((int)time(0)); //done so rand() is actually random everytime
    int numSteps = rand()%4; //number of steps to path

    //want even number of steps so that we can add final desination
    if (numSteps%2 == 1) { //if odd
        numSteps += 3; //3 incase we get 1, need at least 4 steps
    } else {
        if (numSteps = 0){
            numSteps += 4;
        } else {
            numSteps += 2;//2 incase we get 0, need at least 4 steps
        }
    }

    curPair.first = mapHeight -1;
    curPair.second = 0;
    ret.push(curPair);

    //first pos is airport(mapHeight -1,0) (needed for move function)
    //second is (rand,0)
    //third is (last,rand)
    //fourth is (rand,last)
    //....
    //last is (mapHeight -1, last)
    for(int i = 0; i < numSteps; ++i){
        if (firstPos) {
            curPair.first = rand() % mapHeight;
            firstPos = false;
        } else {
            curPair.second = rand() % mapWidth;
            firstPos = true;
        }
        ret.push(curPair);
    }

    //want to add final destination at bottom of map (ground)
    curPair.first = mapHeight - 1;
    ret.push(curPair);

    return ret;
}