//
// Created by jared on 9/24/17.
//

#ifndef BAKED_RICE_DRONE_H
#define BAKED_RICE_DRONE_H

#include <utility>
#include <queue>

class Drone {
private:
    std::queue<std::pair<int,int>> path;
    int curX;
    int curY;
    int nextX;
    int nextY;
    void move();
public:
    Drone();
    std::pair<int,int> getPos();
};


#endif //BAKED_RICE_DRONE_H
