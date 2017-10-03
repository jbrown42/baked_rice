//
// Created by jared on 9/24/17.
//

#include <pthread.h>
#include <iostream>
#include <sys/wait.h>
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
    //lock curNumDrones mutex;
    //remove from cur#drones so not accounted for
    //unlock curNumDrones mutex;
    //lock takeoff mutex
    //lock curNumDrones mutex;
    //re-add to cur#drones
    //unlock curNumDrones mutex;
    //printf("attempting to takeoff\n");
    curX = path.front().second;
    curY = path.front().first;
//    cout<<"("<<curY<<","<<curX<<")\n";
    returnPath.push((path.front()));
    path.pop();
    World::placeDrone(curY,curX,droneID);
    move();
}

void Drone::land() {
    printf("landing %d\n", droneID);
    World::removeDrone(curY,curX);
    pthread_mutex_lock(&Mthread::mCurNumDrones);
    --Mthread::curNumDrones;
    printf("%d landed, %d drones left\n", droneID,Mthread::curNumDrones);
    if (Mthread::curNumDrones >= Mthread::numOfDronesMoved) {
        pthread_cond_signal(&Mthread::allDronesMoved);
    }
    pthread_mutex_unlock(&Mthread::mCurNumDrones);
    pthread_mutex_unlock(&Mthread::mTakeoff);
    //unlock takeoff mutex
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
            //printf("landing at airport\n");
            pthread_mutex_lock(&Mthread::mCurNumDrones);
            --Mthread::curNumDrones;
            pthread_mutex_unlock(&Mthread::mCurNumDrones);
            pthread_mutex_lock(&Mthread::mTakeoff);
            pthread_mutex_lock(&Mthread::mCurNumDrones);
            ++Mthread::curNumDrones;
            pthread_mutex_unlock(&Mthread::mCurNumDrones);
            //lock curNumDrones mutex;
            //decrement cur#drones so don't wait for this one to move
            //lock curNumDrones mutex;
            //wait unlock takeoff unlocked
            //lock curNumDrones mutex;
            //re-add to cur#drones so this one moving counts again
            //lock curNumDrones mutex;
        }
//        cout<<"("<<nextY<<","<<nextX<<")\n";
        while (curY != nextY) {
            World::removeDrone(curY,curX);
            if (curY < nextY) {
                ++curY;
            } else {
                --curY;
            }
            World::placeDrone(curY,curX,droneID);
            pthread_mutex_lock(&Mthread::mDronesMoving);
            pthread_mutex_lock(&Mthread::mCurNumDrones);
            ++Mthread::numOfDronesMoved;
            if (Mthread::numOfDronesMoved >= Mthread::curNumDrones) {
                pthread_cond_signal(&Mthread::allDronesMoved);
            }
            pthread_mutex_unlock(&Mthread::mDronesMoving);
            pthread_mutex_unlock(&Mthread::mCurNumDrones);
            printf("%d waiting for map print y\n",droneID);
            pthread_cond_wait(&Mthread::dronesCanMove,&Mthread::mDronesCanMove);
            printf("%d map printed y\n",droneID);
            //lock move mutex
            //update moving shared resource
            //check if == cur#drones
                //yes = send allDronesMoves cv unlock
            //unlock move mutex
            //wait until droneCanMove cv unlocked
        }

        pthread_mutex_unlock(&Mthread::mTakeoff);
        //unlock takeoff mutex

        while (curX != nextX) {
            World::removeDrone(curY,curX);
            if (curX < nextX) {
                ++curX;
            } else {
                --curX;
            }
            World::placeDrone(curY,curX,droneID);
            pthread_mutex_lock(&Mthread::mDronesMoving);
            pthread_mutex_lock(&Mthread::mCurNumDrones);
            ++Mthread::numOfDronesMoved;
            if (Mthread::numOfDronesMoved >= Mthread::curNumDrones) {
                pthread_cond_signal(&Mthread::allDronesMoved);
            }
            pthread_mutex_unlock(&Mthread::mDronesMoving);
            pthread_mutex_unlock(&Mthread::mCurNumDrones);
            printf("%d waiting for map print x\n",droneID);
            pthread_cond_wait(&Mthread::dronesCanMove,&Mthread::mDronesCanMove);
            printf("%d map printed x\n",droneID);            //lock move mutex
            //update moving shared resource
            //unlock move mutex
            //wait until droneCanMove cv unlocked
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