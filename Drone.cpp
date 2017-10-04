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
    takeoff();
}

void Drone::takeoff() {
    curX = path.front().second;
    curY = path.front().first;
    cout<<droneID<<"("<<curY<<","<<curX<<")\n";
    returnPath.push((path.front()));
    path.pop();
    World::placeDrone(curY,curX,droneID);
    pthread_mutex_lock(&Mthread::mNumDronesInAir);
    Mthread::numDronesInAir += 1;
    pthread_mutex_unlock(&Mthread::mNumDronesInAir);
    Mthread::droneTakingOff = false;
    pthread_cond_broadcast(&Mthread::cDroneTakeOff);
    pthread_mutex_unlock(&Mthread::mTakeoff);
    move();
}

void Drone::land() {

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
        cout<<droneID<<"("<<nextY<<","<<nextX<<")\n";
        while (curY != nextY) {
            pthread_mutex_lock(&Mthread::mTakeoff);
            if (Mthread::droneTakingOff) {
                pthread_cond_wait(&Mthread::cDroneTakeOff,&Mthread::mTakeoff);
            }
            pthread_mutex_unlock(&Mthread::mTakeoff);
            World::removeDrone(curY,curX);
            if (curY < nextY) {
                ++curY;
            } else {
                --curY;
            }
            World::placeDrone(curY,curX,droneID);
        }

        while (curX != nextX) {
            pthread_mutex_lock(&Mthread::mTakeoff);
            if (Mthread::droneTakingOff) {
                pthread_cond_wait(&Mthread::cDroneTakeOff,&Mthread::mTakeoff);
            }
            pthread_mutex_unlock(&Mthread::mTakeoff);
            World::removeDrone(curY,curX);
            if (curX < nextX) {
                ++curX;
            } else {
                --curX;
            }
            World::placeDrone(curY,curX,droneID);
        }
        if (path.empty()) {
            returnPath.pop();
        } else {
            returnPath.push((path.front()));
            path.pop();
        }
    }
    //landed at airport
    land();
}