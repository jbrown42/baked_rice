//
// Created by jared on 9/24/17.
//

#include <iostream>
#include "World.h"
#include "Drone.h"

using namespace std;

Drone::Drone() {
    path = World::generatePath();
    move();
}

void Drone::move(){
    curX = path.front().second;
    curY = path.front().first;
    cout<<"("<<curY<<","<<curX<<")\n";
    path.pop();
    World::placeDrone(curY,curX);
    while (!path.empty()) {
        nextX = path.front().second;
        nextY = path.front().first;
        while (curY != nextY) {
            World::removeDrone(curY,curX);
            if (curY < nextY) {
                ++curY;
            } else {
                --curY;
            }
            World::placeDrone(curY,curX);
            World::printMap();
        }
        while (curX != nextX) {
            World::removeDrone(curY,curX);
            if (curX < nextX) {
                ++curX;
            } else {
                --curX;
            }
            World::placeDrone(curY,curX);
            World::printMap();
        }
        cout<<"("<<nextY<<","<<nextX<<")\n";
        path.pop();
    }
}