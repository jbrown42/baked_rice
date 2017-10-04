//
// Created by jared on 10/2/17.
//

#include "Mthread.h"

//shared resources
int Mthread::numOfDronesMoved = 0;
bool Mthread::allDronesLaunched = false;
int Mthread::numDronesTakenOff = 0;
struct timespec Mthread::waitTime = {0};

//mutexs
pthread_mutex_t Mthread::mNumDronesMoved;
pthread_mutex_t Mthread::mDronesCanMove;
pthread_mutex_t Mthread::mAllDronesMoved;
pthread_mutex_t Mthread::mNumDronesTakenOff;
pthread_mutex_t Mthread::mTakeoff;

//conditional vars
pthread_cond_t Mthread::dronesCanMove;
pthread_cond_t Mthread::allDronesMoved;

void Mthread::init() {
    //init all mutexs and cvs
    pthread_mutex_init(&mNumDronesMoved,NULL);
    pthread_mutex_init(&mDronesCanMove,NULL);
    pthread_mutex_init(&mAllDronesMoved,NULL);
    pthread_mutex_init(&mNumDronesTakenOff,NULL);
    pthread_mutex_init(&mTakeoff,NULL);

    pthread_cond_init(&dronesCanMove,NULL);
    pthread_cond_init(&allDronesMoved,NULL);

    waitTime.tv_sec = 0;
    waitTime.tv_nsec = 500000;
}
