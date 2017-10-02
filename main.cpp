//
// Created by jared on 9/20/17.
//

#include <iostream>
#include <pthread.h>
#include <mutex>
#include <vector>
#include "Drone.h"
#include "World.h"

const int numDrones = 2;
pthread_t drones[numDrones];
int threadReturn;

//mutexs
pthread_mutex_t mDronesMoving;
pthread_mutex_t mDronesCanMove;
pthread_mutex_t mAllDronesMoved;
pthread_mutex_t mCurNumDrones;

//conditional vars
pthread_cond_t dronesCanMove;
pthread_cond_t allDronesMoved;

//shared resources
int numOfDronesMoved = 0;
long curNumDrones = 0;
bool allDronesLaunched = false;

void* droneCreate(void* droneId) {
    std::cout<<"YAY "<<(long)droneId<<std::endl;
    Drone d = Drone((long)droneId);
}

void* printMap(void*) {
    while (curNumDrones != 0 || !allDronesLaunched) { //while there are still drones flying or they haven't all launched
        //wait until allMove cv unlocked
        //printMap
        //reset numMoved shared resource
        //unlock droneCanMove cv
    }
}

int main () {
    //init all mutexs and cvs

    //create pthread for printMap(void*)

    int size;
    char db;
    printf("What size map would you like? (>=8)\n");
    scanf("%d",&size);
    while (size < 8) {
        printf("Woah! That's way too small, try 8 or higher\n");
        scanf("%d",&size);
    }
    printf("Are you debugging? (y/n)\n");
    scanf("%c",&db);
    while (db != 'y' && db != 'n') {
        printf("Please enter y or n\n");
        scanf("%c",&db);
    }
    if (db == 'y') {
        World::debug = true;
    } else {
        World::debug = false;
    }
    World::createWorld(size);

    for (int i = 0; i < numDrones; ++i) {
        threadReturn = pthread_create(&drones[curNumDrones], NULL, droneCreate, (void *) (curNumDrones));
        //lock curNumDrones mutex;
        curNumDrones++;
        //unlock curNumDrones mutex;

        if (threadReturn) {
            std::cout << "thread creation error" << std::endl;
        }
    }

    allDronesLaunched = true;

    pthread_exit(NULL);

}
