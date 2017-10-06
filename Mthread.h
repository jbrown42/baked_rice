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
    //mutexes
    static pthread_mutex_t mDroneMoving;
    static pthread_mutex_t mTakeoff;

    //conditional vars
    static pthread_cond_t cDronesCanMove;

    //shared resources
    static int numDronesInAir;
    static long numDronesMoved;

    static void init();
};


#endif //BAKED_RICE_MTHREAD_H
