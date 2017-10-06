/*
 * Created by jared on 9/20/17.
 *
 * Main class that runs the project. Creates the world and
 * all drones, each being a pthread running concurrently, and
 * exits.
 */

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
    printf("What size map would you like? (>=9)\n");
    scanf("%d",&size);
    while (size < 9) {
        printf("Woah! That's way too small, try 9 or higher\n");
        scanf("%d",&size);
    }
    printf("How many drones? (0<n<11)\n");
    scanf("%d",&numDrones);
    while(numDrones <= 0 || numDrones >= 11) {
        printf("Come on now, at least one drone, at most ten.\n");
        scanf("%d",&numDrones);
    }
    pthread_t drones[numDrones];

    World::createWorld(size);

    for (long i = 0; i < numDrones; ++i) {
        pthread_mutex_lock(&Mthread::mTakeoff); //lock so drones launch staggered
        threadReturn = pthread_create(&drones[i], NULL, droneCreate, (void *) i);

        if (threadReturn) {
            std::cout << "thread creation error" << std::endl;
            return -1;
        }
    }

    pthread_exit(NULL);

}
