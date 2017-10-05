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
//    cout<<droneID<<"("<<curY<<","<<curX<<")\n";
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
                ++curY;
            } else {
                --curY;
            }
            World::placeDrone(curY,curX,droneID);
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

        pthread_mutex_unlock(&Mthread::mTakeoff);

        while (curX != nextX) {
            pthread_mutex_lock(&Mthread::mDroneMoving);
            World::removeDrone(curY,curX);
            if (curX < nextX) {
                ++curX;
            } else {
                --curX;
            }
            World::placeDrone(curY,curX,droneID);
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
//        cout<<droneID<<"("<<nextY<<","<<nextX<<")\n";
    }
    land();
}