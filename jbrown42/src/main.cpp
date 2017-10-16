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

//Number of drones to run the simulation with
int numDrones = 10;

//size of the map to run the simulation with
int size = 20;

//stores the return value of a thread to check for errors
int threadReturn;

//array holding pthread ids of all the drones
pthread_t drones[10];

//holds the pthread id of the map printing thread
pthread_t map;

//map waits this long before printing
struct timespec mapRate;

//airport waits this long before launching a drone
//done to avoid takeoff collisions
struct timespec launchRate;

/*
 * Pthread function called to create a drone thread.
 * @param - ID of the drone to be created
 */
void* droneCreate(void* droneId) {
    Drone d = Drone((long)droneId);
}

/*
 * Pthread function used to print the map. Loops until all drones have
 * gone out and come back.
 */
void* printMap(void*) {
    while (World::numDronesLanded != numDrones) {
        nanosleep(&mapRate, NULL);
        World::printMap();
    }
}

int main () {
    mapRate.tv_sec = 0;
    mapRate.tv_nsec = 50000000; //.05 seconds
    launchRate.tv_sec = 2;

    srand((int)time(0)); //done so rand() is actually random everytime

    World::createWorld(size);

    threadReturn = pthread_create(&map, NULL, printMap,NULL);
    if (threadReturn) {
        std::cout<<"Error creating map thread"<<std::endl;
        return -1;
    }

    for (long i = 0; i < numDrones; ++i) {
        nanosleep(&launchRate,NULL);
        threadReturn = pthread_create(&drones[i], NULL, droneCreate, (void *) i);

        if (threadReturn) {
            std::cout << "thread creation error" << std::endl;
            return -1;
        }
    }

    pthread_exit(NULL); //done so program doesn't terminate after main finishes

}
