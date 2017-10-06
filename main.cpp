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

int numDrones;
int threadReturn;

void* droneCreate(void* droneId) {
    Drone d = Drone((long)droneId);
}

int main () {

    Mthread::init();
    srand((int)time(0)); //done so rand() is actually random everytime

    int size;
    char db;
    printf("What size map would you like? (>=9)\n");
    scanf("%d",&size);
    while (size < 9) {
        printf("Woah! That's way too small, try 9 or higher\n");
        scanf("%d",&size);
    }
    printf("How many drones? (0<n<10)\n");
    scanf("%d",&numDrones);
    while(numDrones <= 0 || numDrones >= 10) {
        printf("Come on now, at least one drone, at most ten.\n");
        scanf("%d",&numDrones);
    }
    pthread_t drones[numDrones];
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

    for (int i = 0; i < numDrones; ++i) {
        pthread_mutex_lock(&Mthread::mTakeoff);
        threadReturn = pthread_create(&drones[i], NULL, droneCreate, (void *) i);

        if (threadReturn) {
            std::cout << "thread creation error" << std::endl;
            return -1;
        }
    }

    pthread_exit(NULL);

}
