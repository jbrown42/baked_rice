//
// Created by jared on 10/2/17.
//

#include <pthread.h>

#ifndef BAKED_RICE_MTHREAD_H
#define BAKED_RICE_MTHREAD_H


class Mthread {
private:
    Mthread();
public:
    //mutexs
    static pthread_mutex_t mNumDronesInAir;
    static pthread_mutex_t mDroneMoving;
    static pthread_mutex_t mAllDronesMoved;
    static pthread_mutex_t mNumDronesMoved;
    static pthread_mutex_t mTakeoff;

    //conditional vars
    static pthread_cond_t cDronesCanMove;
    static pthread_cond_t cAllDronesMoved;
    static pthread_cond_t cDroneTakeOff;

    //shared resources
    static int numDronesInAir;
    static long numDronesMoved;
    static bool allDronesLaunched;
    static bool droneTakingOff;
    static bool allDronesMoved;

    static struct timespec waitTime;

    static void init();
};


#endif //BAKED_RICE_MTHREAD_H
