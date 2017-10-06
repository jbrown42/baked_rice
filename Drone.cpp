//
// Created by jared on 9/24/17.
//

#include <pthread.h>
#include <iostream>
#include "World.h"
#include "Drone.h"
#include "Mthread.h"

using namespace std;


Drone::Drone(long ID) {
    droneID = ID;
    path = World::generatePath(droneID);
    takingOff = true;
    takeoff();
}

void Drone::takeoff() {
    curX = path.front().second;
    curY = path.front().first;
    returnPath.push((path.front()));
    path.pop();
    World::placeDrone(curY,curX,droneID);
    Mthread::numDronesInAir += 1;
    move();
}

void Drone::land() {
    Mthread::numDronesInAir -= 1;
    World::removeDrone(curY,curX);
    pthread_mutex_lock(&Mthread::mDroneMoving);
    if (Mthread::numDronesMoved == Mthread::numDronesInAir) {
        World::printMap();
        Mthread::numDronesMoved = 0;
        pthread_cond_broadcast(&Mthread::cDronesCanMove);
    }
    pthread_mutex_unlock(&Mthread::mDroneMoving);
}

void Drone::avoidCollision(bool vertical, bool positive){
    if (vertical) {
        if (World::placeDrone(curY,curX+1,droneID) == 0) {
            ++curX;
        } else if (World::placeDrone(curY,curX-1,droneID) == 0) {
            --curX;
        } else if (positive) {
            if ((World::placeDrone(curY-1,curX,droneID) == 0)) {
                --curY;
            } else {
                printf("%d drone surrounded\n",droneID);
            }
        } else {
            if ((World::placeDrone(curY-1,curX,droneID) == 0)) {
                ++curY;
            } else {
                printf("%d drone surrounded\n",droneID);
            }
        }
        printf("%d avoided vertical collison\n", droneID);
    } else {
        if (World::placeDrone(curY+1,curX,droneID) == 0) {
            ++curY;
        } else if (World::placeDrone(curY-1,curX,droneID) == 0) {
            --curY;
        } else if (positive) {
            if ((World::placeDrone(curY,curX-1,droneID) == 0)) {
                --curX;
            } else {
                printf("%d drone surrounded\n",droneID);
            }
        } else {
            if ((World::placeDrone(curY,curX+1,droneID) == 0)) {
                ++curX;
            } else {
                printf("%d drone surrounded\n",droneID);
            }
        }
        printf("%d avoided horizontal collison\n", droneID);
    }
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
        }
        while (curY != nextY) {
            pthread_mutex_lock(&Mthread::mDroneMoving);
            World::removeDrone(curY,curX);
            if (curY < nextY) {
                if (World::placeDrone(curY + 1,curX,droneID) == -1) {
                    avoidCollision(true,true);
                } else {
                    ++curY;
                }
            } else {
                if (World::placeDrone(curY - 1,curX,droneID) == -1) {
                    avoidCollision(true,false);
                } else {
                    --curY;
                }
            }
            Mthread::numDronesMoved += 1;
            if (Mthread::numDronesMoved == Mthread::numDronesInAir) {
                World::printMap();
                Mthread::numDronesMoved = 0;
                pthread_cond_broadcast(&Mthread::cDronesCanMove);
            }else{
                pthread_cond_wait(&Mthread::cDronesCanMove,&Mthread::mDroneMoving);
            }
            pthread_mutex_unlock(&Mthread::mDroneMoving);
        }

        if (takingOff) {
            takingOff = false;
            pthread_mutex_unlock(&Mthread::mTakeoff);
        }

        while (curX != nextX) {
            pthread_mutex_lock(&Mthread::mDroneMoving);
            World::removeDrone(curY,curX);
            if (curX < nextX) {
                if (World::placeDrone(curY,curX + 1,droneID) == -1) {
                    avoidCollision(false,true);
                } else {
                    ++curX;
                }
            } else {
                if (World::placeDrone(curY,curX - 1,droneID) == -1) {
                    avoidCollision(false,false);
                } else {
                    --curX;
                }
            }
            Mthread::numDronesMoved += 1;
            if (Mthread::numDronesMoved == Mthread::numDronesInAir) {
                World::printMap();
                Mthread::numDronesMoved = 0;
                pthread_cond_broadcast(&Mthread::cDronesCanMove);
            }else{
                pthread_cond_wait(&Mthread::cDronesCanMove,&Mthread::mDroneMoving);
            }
            pthread_mutex_unlock(&Mthread::mDroneMoving);
        }
        if (path.empty()) {
            returnPath.pop();
        } else {
            returnPath.push((path.front()));
            path.pop();
        }
    }
    land();
}