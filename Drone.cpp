//
// Created by jared on 9/24/17.
//

#include <iostream>
#include "World.h"
#include "Drone.h"

using namespace std;

Drone::Drone() {
    path = World::generatePath();
    takeoff();
}

void Drone::takeoff() {
    printf("attempting to takeoff\n");
    curX = path.front().second;
    curY = path.front().first;
    cout<<"("<<curY<<","<<curX<<")\n";
    returnPath.push((path.front()));
    path.pop();
    World::placeDrone(curY,curX);
    printf("takeoff successful\n");
    move();
}

void Drone::move(){
    while (!path.empty() || !returnPath.empty()) {
        if (path.empty()) {
            nextX = returnPath.top().second;
            nextY = returnPath.top().first;
        } else {
            nextX = path.front().second;
            nextY = path.front().first;
        }
        if (path.empty() && returnPath.size() == 1) {
            printf("landing at airport\n");
        }
        cout<<"("<<nextY<<","<<nextX<<")\n";
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
        if (path.empty()) {
            returnPath.pop();
        } else {
            returnPath.push((path.front()));
            path.pop();
        }
    }
}