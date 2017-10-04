//
// Created by jared on 9/24/17.
//

#ifndef BAKED_RICE_DRONE_H
#define BAKED_RICE_DRONE_H

#include <utility>
#include <queue>
#include <stack>

class Drone {
private:
    std::queue<std::pair<int,int>> path;
    std::stack<std::pair<int,int>> returnPath;
    int curX;
    int curY;
    int nextX;
    int nextY;
    void move();
    void takeoff();
    void land();
    long droneID;
    bool takingOff;
public:
    Drone(long ID);
    std::pair<int,int> getPos();
};


#endif //BAKED_RICE_DRONE_H
