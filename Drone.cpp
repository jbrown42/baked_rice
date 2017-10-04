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
    pthread_mutex_lock(&Mthread::mNumDronesTakenOff);
    Mthread::numDronesTakenOff -= 1;
    pthread_mutex_unlock(&Mthread::mNumDronesTakenOff);
//    pthread_mutex_unlock(&Mthread::mTakeoff);
    World::removeDrone(curY,curX);
    if (Mthread::numDronesTakenOff == 0) {
        pthread_cond_signal(&Mthread::allDronesMoved);
    }
    pthread_exit(NULL);
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
//            pthread_mutex_lock(&Mthread::mTakeoff);
        }
        while (curY != nextY) {
            World::removeDrone(curY,curX);
            if (curY < nextY) {
                ++curY;
            } else {
                --curY;
            }
            World::placeDrone(curY,curX,droneID);
            pthread_mutex_lock(&Mthread::mAllDronesMoved);
            pthread_mutex_lock(&Mthread::mNumDronesMoved);
            Mthread::numOfDronesMoved += 1;
            pthread_mutex_lock(&Mthread::mNumDronesTakenOff);
            pthread_mutex_unlock(&Mthread::mAllDronesMoved);
            if (Mthread::numOfDronesMoved >= Mthread::numDronesTakenOff) {
                nanosleep(&Mthread::waitTime,NULL);
                pthread_cond_signal(&Mthread::allDronesMoved);
            }
            pthread_mutex_unlock(&Mthread::mNumDronesMoved);
            pthread_mutex_unlock(&Mthread::mNumDronesTakenOff);
            pthread_mutex_lock(&Mthread::mDronesCanMove);
            pthread_cond_wait(&Mthread::dronesCanMove,&Mthread::mDronesCanMove);
            pthread_mutex_unlock(&Mthread::mDronesCanMove);
        }

        while (curX != nextX) {
            World::removeDrone(curY,curX);
            if (curX < nextX) {
                ++curX;
            } else {
                --curX;
            }
            World::placeDrone(curY,curX,droneID);
            pthread_mutex_lock(&Mthread::mAllDronesMoved);
            pthread_mutex_lock(&Mthread::mNumDronesMoved);
            Mthread::numOfDronesMoved += 1;
            pthread_mutex_lock(&Mthread::mNumDronesTakenOff);
            pthread_mutex_unlock(&Mthread::mAllDronesMoved);
            if (Mthread::numOfDronesMoved >= Mthread::numDronesTakenOff) {
                if (takingOff) {
                    Mthread::numDronesTakenOff += 1;
                    pthread_mutex_unlock(&Mthread::mTakeoff);
                    takingOff = false;
                }
                nanosleep(&Mthread::waitTime,NULL);
                pthread_cond_signal(&Mthread::allDronesMoved);
            }
            pthread_mutex_unlock(&Mthread::mNumDronesMoved);
            pthread_mutex_unlock(&Mthread::mNumDronesTakenOff);
            pthread_mutex_lock(&Mthread::mDronesCanMove);
            pthread_cond_wait(&Mthread::dronesCanMove,&Mthread::mDronesCanMove);
            pthread_mutex_unlock(&Mthread::mDronesCanMove);
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