//
// Created by jared on 9/24/17.
//

#ifndef BAKED_RICE_WORLD_H
#define BAKED_RICE_WORLD_H

#include <vector>
#include <map>
#include <queue>
#include <string>
#include "Drone.h"

class World {
private:
    static int mapHeight;
    static int mapWidth;
    static std::map<int,std::vector<std::string>> world;
    World(){};
public:
    static bool debug;
    static void createWorld(int size);
    static void printMap();
    static std::queue<std::pair<int,int>> generatePath(long longAlt);
    static void placeDrone(int y, int x);
    static void removeDrone(int y, int x);
};

#endif //BAKED_RICE_WORLD_H
