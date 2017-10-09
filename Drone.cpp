/*
 * Created by jared on 9/24/17.
 *
 * Class that abstracts a drone. Has a takeoff function which then
 * transitions to move which loops until it gets back to the airport
 * and transitions to land. Also has the ability to detect and avoid
 * collisions.
 */

#include <pthread.h>
#include "World.h"
#include "Drone.h"
#include "Mthread.h"

using namespace std;

/*
 * Drone constructor, sets the droneID, creates the path
 * for the drone and sets the takingOff bool to true. Transitions
 * to takeoff after.
 *
 * @param ID - ID the drone will have
 */
Drone::Drone(long ID) {
    droneID = ID;
    path = World::generatePath(droneID);
    takingOff = true;
    takeoff();
}

/*
 * Simulates a drone taking off from the airport. Places
 * the drone at the airport, initializes its current x and y
 * position and increments the number of drones in the air.
 * Then signals the drone to move.
 */
void Drone::takeoff() {
    curX = path.front().second;
    curY = path.front().first;
    returnPath.push((path.front()));
    path.pop();
    World::placeDrone(curY,curX,droneID);
    Mthread::numDronesInAir += 1;
    move();
}

/*
 * Simulates a drone landing at the airport. Decrements number
 * of drones in the air and removes the drone from the map. Incase
 * this is the last drone to move it checks if all other drones
 * have moved and if true signals that they can move again.
 */
void Drone::land() {
    Mthread::numDronesInAir -= 1;
    World::removeDrone(curY,curX);
    pthread_mutex_lock(&Mthread::mDroneMoving);
    //If last drone to move this will be true. To avoid a deadlock where all drones
    //are waiting for this drone to move but this drone won't move because it has
    //landed, this drone checks if all the other drones are waiting for this one to
    //move, and if so, signals that they can move.
    if (Mthread::numDronesMoved == Mthread::numDronesInAir) {
        World::printMap();
        Mthread::numDronesMoved = 0;
        pthread_cond_broadcast(&Mthread::cDronesCanMove);
    }
    pthread_mutex_unlock(&Mthread::mDroneMoving);
}

/*
 * Collision avoidance simulation for drones. If a drone tries to
 * move to a position and a drone is there, this is triggered. If the
 * drone was moving vertically it will try to move horizontally to
 * avoid a collision. If both positions are taken, it will try to
 * move opposite the direction it originally tried to move. It does the
 * same thing for horizontal.
 *
 * @param vertical - true if drone attempted to move vertically
 *                   false if drone attempted to move horizontally
 * @param positive - true if drone attempted to move in a positive direction
 *                   false if drone attempted to move in a negative direction
 */
void Drone::avoidCollision(bool vertical, bool positive){
    if (vertical) { //if attempted to move vertically
        if (World::placeDrone(curY,curX+1,droneID) == 0) { //try to move right
            ++curX;
        } else if (World::placeDrone(curY,curX-1,droneID) == 0) { //try to move left
            --curX;
        } else if (positive) { //left and right occupied, check original attempted movement direction
            if ((World::placeDrone(curY-1,curX,droneID) == 0)) { //try to move up
                --curY;
            } else {
                printf("%d drone surrounded\n",droneID); //drone surrounded, doesn't move
                World::placeDrone(curY,curX,droneID); //drone stays where it was
            }
        } else {
            if ((World::placeDrone(curY+1,curX,droneID) == 0)) { //try to move down, drone doesn't move
                ++curY;
            } else {
                printf("%d drone surrounded\n",droneID); //drone surrounded, doesn't move
                World::placeDrone(curY,curX,droneID); //drone stays where it was
            }
        }
        printf("%d avoided vertical collison\n", droneID);
    } else { //drone original tred to move horizontally
        if (World::placeDrone(curY+1,curX,droneID) == 0) { //try to move down
            ++curY;
        } else if (World::placeDrone(curY-1,curX,droneID) == 0) { //try to move up
            --curY;
        } else if (positive) { //up and down occupies, check original attempted movement direction
            if ((World::placeDrone(curY,curX-1,droneID) == 0)) { //try to move left
                --curX;
            } else {
                printf("%d drone surrounded\n",droneID); //drone surrounded, doesn't move
                World::placeDrone(curY,curX,droneID); //drone stays where it was
            }
        } else {
            if ((World::placeDrone(curY,curX+1,droneID) == 0)) { //try to move right
                ++curX;
            } else {
                printf("%d drone surrounded\n",droneID); //drone surrounded, doesn't move
                World::placeDrone(curY,curX,droneID); //drone stays where it was
            }
        }
        printf("%d avoided horizontal collison\n", droneID);
    }
}

/*
 * Movement function for the drone. The drone will move in an
 * alternating pattern of vertical until it reaches its
 * vertical coordinate then horizontal until it reaches its horizontal
 * coordinate. Pops the position of the path into returnPath, the path
 * the drone follows to get back to the airport. returnPath is a stack
 * so the path in the queue path will be popped out in reverse order from
 * returnPath.
 */
void Drone::move(){
    while (!path.empty() || !returnPath.empty()) { //while drone isn't back at airport
        if (path.empty()) { //if returning
            nextX = returnPath.top().second;
            nextY = returnPath.top().first;
        } else { //going to destination
            nextX = path.front().second;
            nextY = path.front().first;
        }
        //move vertically to y coordinate
        while (curY != nextY) {
            pthread_mutex_lock(&Mthread::mDroneMoving);
            World::removeDrone(curY,curX);
            if (curY < nextY) {
                if (World::placeDrone(curY + 1,curX,droneID) == -1) {
                    avoidCollision(true,true);
                } else {
                    ++curY;
                }
            } else {
                if (World::placeDrone(curY - 1,curX,droneID) == -1) {
                    avoidCollision(true,false);
                } else {
                    --curY;
                }
            }
            Mthread::numDronesMoved += 1;
            //if all drones moved, print the map. Done so all drones can move
            //concurrently but the map only can print once or it will print over
            //itself
            if (Mthread::numDronesMoved == Mthread::numDronesInAir) { //last drone to move
                World::printMap();
                Mthread::numDronesMoved = 0;
                pthread_cond_broadcast(&Mthread::cDronesCanMove); //signal all drones moved
            }else{
                pthread_cond_wait(&Mthread::cDronesCanMove,&Mthread::mDroneMoving); //wait until all drones moved
            }
            pthread_mutex_unlock(&Mthread::mDroneMoving);
        }

        // at this point the drone reached its starting
        // altitude. If the drone is takingOff, unlock
        // the takeoff mutex so the next drone can takeoff
        if (takingOff) {
            takingOff = false;
            pthread_mutex_unlock(&Mthread::mTakeoff);
        }

        //move horizontally to horizontal coordinate
        while (curX != nextX) {
            pthread_mutex_lock(&Mthread::mDroneMoving);
            for (int i = 0; i < 2; ++i) {
                World::removeDrone(curY, curX);
                if (curX < nextX) {
                    if (World::placeDrone(curY, curX + 1, droneID) == -1) {
                        avoidCollision(false, true);
                    } else {
                        ++curX;
                    }
                } else if (curX > nextX){
                    if (World::placeDrone(curY, curX - 1, droneID) == -1) {
                        avoidCollision(false, false);
                    } else {
                        --curX;
                    }
                } else {
                    if (World::placeDrone(curY, curX, droneID) == -1) {
                        avoidCollision(false, false);
                    }
                    break;
                }
            }
            Mthread::numDronesMoved += 1;
            //if all drones moved, print the map. Done so all drones can move
            //concurrently but the map only can print once or it will print over
            //itself
            if (Mthread::numDronesMoved == Mthread::numDronesInAir) { //last drone to move
                World::printMap();
                Mthread::numDronesMoved = 0;
                pthread_cond_broadcast(&Mthread::cDronesCanMove); //signal all drones moved
            }else{
                pthread_cond_wait(&Mthread::cDronesCanMove,&Mthread::mDroneMoving); //wait until all drones moved
            }
            pthread_mutex_unlock(&Mthread::mDroneMoving);
        }
        //this is done because the last position of the path is the same as the first position
        //of the returnPath. We don't want the drone to wait at its location so we
        //pop the top of returnPath so the drone will move to the next real position.
        if (path.empty()) {
            returnPath.pop();
        } else {
            returnPath.push((path.front()));
            path.pop();
        }
    }
    //drone back at airport
    land();
}