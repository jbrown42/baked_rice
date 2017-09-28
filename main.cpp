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
long dronesSize = 0;
int threadReturn;
void* result;

void* droneCreate(void* droneId) {
    std::cout<<"YAY "<<(long)droneId<<std::endl;
    Drone d = Drone();
}

int main () {
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
    World::printMap();
//    Drone d = Drone();

    for (int i = 0; i < numDrones; ++i) {
        threadReturn = pthread_create(&drones[dronesSize], NULL, droneCreate, (void *) (dronesSize));
        dronesSize++;
    }

    if (threadReturn) {
        std::cout<<"thread creation error"<<std::endl;
    }

    pthread_exit(NULL);

}
