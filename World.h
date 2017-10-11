//
// Created by jared on 9/24/17.
//

#ifndef BAKED_RICE_WORLD_H
#define BAKED_RICE_WORLD_H

#include <vector>
#include <map>
#include <queue>
#include <string>
#include <pthread.h>
#include "Drone.h"

class World {
private:
    static int size;
    static std::map<int,std::vector<std::string>> world;
    static std::map<int, std::vector<pthread_mutex_t>> mWorld;
    World();
public:
    static void land();
    static void createWorld(int size);
    static void printMap();
    static std::queue<std::pair<int,int>> generatePath(long longAlt);
    static int placeDrone(int y, int x, Drone* drone);
    static int numDronesLanded;
};

#endif //BAKED_RICE_WORLD_H
