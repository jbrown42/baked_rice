//
// Created by jared on 9/24/17.
//
#include <iostream>
#include <ctime>
#include "World.h"
#include "Mthread.h"

using namespace std;

int World::mapHeight;
int World::mapWidth;
bool World::debug;

/*coordinates stored (y,x)
  top left is (0,0)
  bottom left is (mapHeight-1,0)*/
map<int,vector<string>> World::world;

void World::createWorld(int size) {
    mapHeight = size;
    mapWidth = size * 2;
    for(int i = 0; i < mapHeight; ++i){
        for(int j = 0; j < mapWidth; ++j) {
            world[i].push_back("~");
        }
    }
    world[mapHeight-1][0] = "A"; //drone airport
}

void World::printMap() {
    if (debug) return;
    for(int i = 0; i < mapHeight; ++i) {
        for (int j = 0; j < mapWidth; ++j) {
            cout<<" "<<world[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void World::placeDrone(int y, int x, int ID) {
    world[y][x] = to_string(ID);
}

void World::removeDrone(int y, int x) {
    if ((y == mapHeight -1) && (x == 0)) {
        world[y][x] = "A";
    } else {
        world[y][x] = "~";
    }
}

queue<pair<int,int>> World::generatePath(long startAlt){
    queue<pair<int,int>> ret;
    pair<int,int> curPair; //coordinates stored (y,x)
    bool firstPos = false;

    srand((int)time(0)); //done so rand() is actually random everytime
    int numSteps = rand()%4; //number of steps to path

    //want odd number of steps so that we can add final desination
    if (numSteps%2 == 0) { //if even
        numSteps += 1; //need at least 1 step and an odd amount
    }

    curPair.first = mapHeight -1;
    curPair.second = 0;
    ret.push(curPair);
    curPair.first = startAlt;
    ret.push(curPair);

    //first pos is airport(mapHeight -1,0) (needed for move function)
    //second is (startAlt,0)
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
            if (curPair.second == 0) {
                curPair.second += 1;
            }
            firstPos = true;
        }
        ret.push(curPair);
    }

    //want to add final destination at bottom of map (ground)
    curPair.first = mapHeight - 1;
    ret.push(curPair);

    return ret;
}