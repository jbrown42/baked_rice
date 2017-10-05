//
// Created by jared on 10/2/17.
//

#include "Mthread.h"

int Mthread::numDronesInAir = 0;
long Mthread::numDronesMoved = 0;
bool Mthread::allDronesLaunched = false;
bool Mthread::droneTakingOff = true;
bool Mthread::allDronesMoved = false;
struct timespec Mthread::waitTime = {0};

pthread_mutex_t Mthread::mNumDronesInAir;
pthread_mutex_t Mthread::mDroneMoving;
pthread_mutex_t Mthread::mAllDronesMoved;
pthread_mutex_t Mthread::mNumDronesMoved;
pthread_mutex_t Mthread::mTakeoff;

//conditional vars
pthread_cond_t Mthread::cDronesCanMove;
pthread_cond_t Mthread::cAllDronesMoved;
pthread_cond_t Mthread::cDroneTakeOff;

void Mthread::init() {
    //init all mutexs and cvs
    pthread_mutex_init(&mNumDronesInAir,NULL);
    pthread_mutex_init(&mDroneMoving,NULL);
    pthread_mutex_init(&mAllDronesMoved,NULL);
    pthread_mutex_init(&mNumDronesMoved,NULL);
    pthread_mutex_init(&mTakeoff,NULL);

    pthread_cond_init(&cDronesCanMove,NULL);
    pthread_cond_init(&cAllDronesMoved,NULL);
    pthread_cond_init(&cDroneTakeOff,NULL);

    waitTime.tv_sec = 0;
    waitTime.tv_nsec = 500000000;
}
