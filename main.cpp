//
// Created by jared on 9/20/17.
//

#include <iostream>
#include <vector>
#include <map>
#include <mutex>
#include "Drone.h"
#include "World.h"

int main (){
    World::createWorld(15);
    World::printMap();
    Drone d = Drone();
    return 0;
}
