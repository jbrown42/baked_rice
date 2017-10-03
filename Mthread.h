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
    static pthread_mutex_t mNumDronesMoved;
    static pthread_mutex_t mDronesCanMove;
    static pthread_mutex_t mAllDronesMoved;
    static pthread_mutex_t mCurNumDrones;
    static pthread_mutex_t mTakeoff;

    //conditional vars
    static pthread_cond_t dronesCanMove;
    static pthread_cond_t allDronesMoved;

    //shared resources
    static int numOfDronesMoved;
    static long curNumDrones;
    static bool allDronesLaunched;

    static struct timespec waitTime;

    static void init();
};


#endif //BAKED_RICE_MTHREAD_H
