/*
 * Created by jared on 10/2/17.
 *
 * Class that contains all mutexes, conditional variables
 * and shared resources.
 */

#include "Mthread.h"

int Mthread::numDronesInAir = 0;
long Mthread::numDronesMoved = 0;

pthread_mutex_t Mthread::mDroneMoving;
pthread_mutex_t Mthread::mTakeoff;

//conditional vars
pthread_cond_t Mthread::cDronesCanMove;

/*
 * Initializes all mutexes and conditional variables.
 */
void Mthread::init() {
    //init all mutexes and cvs
    pthread_mutex_init(&mDroneMoving,NULL);
    pthread_mutex_init(&mTakeoff,NULL);

    pthread_cond_init(&cDronesCanMove,NULL);

}
