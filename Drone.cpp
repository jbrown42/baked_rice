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
    takingOff = true;
    path = World::generatePath(droneID);
    takeoff();
}

void Drone::takeoff() {
    curX = path.front().second;
    curY = path.front().first;
    returnPath.push((path.front()));
    path.pop();
    World::placeDrone(curY,curX,droneID);
    move();
}

void Drone::land() {
    printf("landing %d\n", droneID);
    World::removeDrone(curY,curX);

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
        while (curY != nextY) {
            World::removeDrone(curY,curX);
            if (curY < nextY) {
                ++curY;
            } else {
                --curY;
            }
            World::placeDrone(curY,curX,droneID);
            pthread_mutex_lock(&Mthread::mNumDronesMoved);
            pthread_mutex_lock(&Mthread::mNumDronesTakenOff);
            Mthread::numOfDronesMoved += 1;
            if (Mthread::numOfDronesMoved >= Mthread::numDronesTakenOff) {
                pthread_cond_signal(&Mthread::allDronesMoved);
            }
            pthread_mutex_unlock(&Mthread::mNumDronesMoved);
            pthread_mutex_unlock(&Mthread::mNumDronesTakenOff);
            pthread_cond_wait(&Mthread::dronesCanMove,&Mthread::mDronesCanMove);
        }

        while (curX != nextX) {
            World::removeDrone(curY,curX);
            if (curX < nextX) {
                ++curX;
            } else {
                --curX;
            }
            World::placeDrone(curY,curX,droneID);
            pthread_mutex_lock(&Mthread::mNumDronesMoved);
            pthread_mutex_lock(&Mthread::mNumDronesTakenOff);
            Mthread::numOfDronesMoved += 1;
            if (Mthread::numOfDronesMoved >= Mthread::numDronesTakenOff) {
                if (takingOff) {
                    takingOff = false;
                    Mthread::numDronesTakenOff += 1;
                    pthread_mutex_unlock(&Mthread::mTakeoff);
                    pthread_cond_broadcast(&Mthread::dronesCanMove);
                }
                pthread_cond_signal(&Mthread::allDronesMoved);
            }
            pthread_mutex_unlock(&Mthread::mNumDronesMoved);
            pthread_mutex_unlock(&Mthread::mNumDronesTakenOff);
            pthread_cond_wait(&Mthread::dronesCanMove,&Mthread::mDronesCanMove);
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