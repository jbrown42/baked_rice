//
// Created by jared on 9/24/17.
//

#include <iostream>
#include "World.h"
#include "Drone.h"

using namespace std;

Drone::Drone() {
    path = World::generatePath();
    while (!path.empty()) {
        cout<<"("<<path.front().first<<","<<path.front().second<<")\n";
        path.pop();
    }
}