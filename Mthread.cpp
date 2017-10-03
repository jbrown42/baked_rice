//
// Created by jared on 10/2/17.
//

#include "Mthread.h"

int Mthread::numOfDronesMoved = 0;
long Mthread::curNumDrones = 0;
bool Mthread::allDronesLaunched = false;
struct timespec Mthread::waitTime = {0};

pthread_mutex_t Mthread::mNumDronesMoved;
pthread_mutex_t Mthread::mDronesCanMove;
pthread_mutex_t Mthread::mAllDronesMoved;
pthread_mutex_t Mthread::mCurNumDrones;
pthread_mutex_t Mthread::mTakeoff;

//conditional vars
pthread_cond_t Mthread::dronesCanMove;
pthread_cond_t Mthread::allDronesMoved;

void Mthread::init() {
    //init all mutexs and cvs
    pthread_mutex_init(&mNumDronesMoved,NULL);
    pthread_mutex_init(&mDronesCanMove,NULL);
    pthread_mutex_init(&mAllDronesMoved,NULL);
    pthread_mutex_init(&mCurNumDrones,NULL);
    pthread_mutex_init(&mTakeoff,NULL);

    pthread_cond_init(&dronesCanMove,NULL);
    pthread_cond_init(&allDronesMoved,NULL);

    waitTime.tv_sec = 0;
    waitTime.tv_nsec = 500000;
}
