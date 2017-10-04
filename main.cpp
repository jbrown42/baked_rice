//
// Created by jared on 9/20/17.
//

#include <iostream>
#include <pthread.h>
#include <mutex>
#include <vector>
#include <sys/wait.h>
#include "Drone.h"
#include "World.h"
#include "Mthread.h"

const int numDrones = 3;
pthread_t drones[numDrones];
int threadReturn;

void* droneCreate(void* droneId) {
    Drone d = Drone((long)droneId);
}

void* printMap(void*) {
    while (true/*num drones in air != 0 && droneTakingOff false*/) {
        pthread_mutex_lock(&Mthread::mAllDronesMoved);
        pthread_cond_wait(&Mthread::cAllDronesMoved,&Mthread::mAllDronesMoved);
        World::printMap();
        pthread_mutex_lock(&Mthread::mNumDronesMoved);
        Mthread::numDronesMoved = 0;
        pthread_mutex_unlock(&Mthread::mNumDronesMoved);
        pthread_cond_broadcast(&Mthread::cDronesCanMove);
        pthread_mutex_unlock(&Mthread::mAllDronesMoved);
    }
}

int main () {

    Mthread::init();

    int size;
    char db;
    printf("What size map would you like? (>=8)\n");
    scanf("%d",&size);
    while (size < 8) {
        printf("Woah! That's way too small, try 8 or higher\n");
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
        std::cout << "thread creation error" << std::endl;
    }

    for (int i = 0; i < numDrones; ++i) {
        pthread_mutex_lock(&Mthread::mTakeoff);
        printf("%d new drone taking off\n",i);
        Mthread::droneTakingOff = true;
        threadReturn = pthread_create(&drones[i], NULL, droneCreate, (void *) i);

        if (threadReturn) {
            std::cout << "thread creation error" << std::endl;
        }
    }

    Mthread::allDronesLaunched = true;

    pthread_exit(NULL);

}
