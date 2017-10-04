//
// Created by jared on 9/20/17.
//

#include <iostream>
#include <pthread.h>
#include <mutex>
#include <vector>
#include "Drone.h"
#include "World.h"
#include "Mthread.h"

static const int numDrones = 3;
pthread_t drones[numDrones];
int threadReturn;
long curNumDrones = 0;

void* droneCreate(void* droneId) {
    Drone d = Drone((long)droneId);
}

void* printMap(void*) {
    while (Mthread::numDronesTakenOff != 0 || !Mthread::allDronesLaunched) { //while there are still drones flying or they haven't all launched
//        printf("map waiting\n");
        pthread_cond_wait(&Mthread::allDronesMoved,&Mthread::mAllDronesMoved);
        pthread_mutex_lock(&Mthread::mNumDronesMoved);
        World::printMap();
        Mthread::numOfDronesMoved = 0;
//        printf("map signal\n");
        pthread_mutex_unlock(&Mthread::mNumDronesMoved);
        pthread_cond_broadcast(&Mthread::dronesCanMove);
    }
    printf("Killing map\n");
    pthread_exit(NULL);
}

int main () {

    Mthread::init();

    int size;
    char db;
    printf("What size map would you like? (>=6)\n");
    scanf("%d",&size);
    while (size < 6) {
        printf("Woah! That's way too small, try 6 or higher\n");
        scanf("%d",&size);
    }
    printf("Are you debugging?\n");
    scanf(" %c",&db);
    while (db != 'y' && db != 'n') {
        printf("Please enter y or n\n");
        scanf(" %c",&db);
    }
    if (db == 'y') {
        World::debug = true;
    } else {
        World::debug = false;
    }
    World::createWorld(size);

    pthread_t printMapThread;
    threadReturn = pthread_create(&printMapThread,NULL,printMap,NULL);
    if (threadReturn) {
        std::cout << "map thread creation error" << std::endl;
    }

    nanosleep(&Mthread::waitTime,NULL);

    for (int i = 0; i < numDrones; ++i) {
        pthread_mutex_lock(&Mthread::mTakeoff);
        threadReturn = pthread_create(&drones[curNumDrones], NULL, droneCreate, (void *) (curNumDrones));
        curNumDrones += 1;

        if (threadReturn) {
            std::cout << "thread creation error" << std::endl;
        }
    }

    Mthread::allDronesLaunched = true;

    pthread_exit(NULL);

}
