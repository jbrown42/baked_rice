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

using namespace std;

/*
 * Drone constructor, sets the droneID, creates the path
 * for the drone and sets the takingOff bool to true. Transitions
 * to takeoff after.
 *
 * @param ID - ID the drone will have
 */
Drone::Drone(long ID) {
    waitTime.tv_sec = 0;
    waitTime.tv_nsec = 156349822 * ((rand() % 5) + 1); //5 arbitrary speeds possible
    droneID = ID;
    path = World::generatePath(droneID);
    takeoff();
}

/*
 * Simulates a drone taking off from the airport. Places
 * the drone at the airport, initializes its current x and y
 * position and signals the drone to move.
 */
void Drone::takeoff() {
    curX = path.front().second;
    curY = path.front().first;
    returnPath.push((path.front()));
    path.pop();
    move();
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
        if (World::placeDrone(curY,curX+1,this) == 0) { //try to move right
            ++curX;
        } else if (World::placeDrone(curY,curX-1,this) == 0) { //try to move left
            --curX;
        } else if (positive) { //left and right occupied, check original attempted movement direction
            if ((World::placeDrone(curY-1,curX,this) == 0)) { //try to move up
                --curY;
            } else {
                World::placeDrone(curY,curX,this); //drone stays where it was
            }
        } else {
            if ((World::placeDrone(curY+1,curX,this) == 0)) { //try to move down, drone doesn't move
                ++curY;
            } else {
                World::placeDrone(curY,curX,this); //drone stays where it was
            }
        }
    } else { //drone original tred to move horizontally
        if (World::placeDrone(curY+1,curX,this) == 0) { //try to move down
            ++curY;
        } else if (World::placeDrone(curY-1,curX,this) == 0) { //try to move up
            --curY;
        } else if (positive) { //up and down occupies, check original attempted movement direction
            if ((World::placeDrone(curY,curX-1,this) == 0)) { //try to move left
                --curX;
            } else {
                World::placeDrone(curY,curX,this); //drone stays where it was
            }
        } else {
            if ((World::placeDrone(curY,curX+1,this) == 0)) { //try to move right
                ++curX;
            } else {
                World::placeDrone(curY,curX,this); //drone stays where it was
            }
        }
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
            nanosleep(&waitTime,NULL);
            if (curY < nextY) {
                if (World::placeDrone(curY + 1,curX, this) != 0) {
                    avoidCollision(true,false);
                } else {
                    ++curY;
                }
            } else {
                if (World::placeDrone(curY - 1,curX, this) != 0) {
                    avoidCollision(true,true);
                } else {
                    --curY;
                }
            }
        }

        //move horizontally to horizontal coordinate
        while (curX != nextX) {
            nanosleep(&waitTime,NULL);
            if (curX < nextX) {
                if (World::placeDrone(curY,curX + 1, this) != 0) {
                    avoidCollision(false,true);
                } else {
                    ++curX;
                }
            } else {
                if (World::placeDrone(curY, curX - 1, this) != 0) {
                    avoidCollision(false, false);
                } else {
                    --curX;
                }
            }
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
    World::land();
}

/*
 * Getter function to get DroneID
 * @return the drone's ID this is called on
 */
int Drone::getID() {
    return droneID;
}

/*
 * Getter function to get the Drone's position
 * @return pair representing the calling drone's position
 */
std::pair<int,int> Drone::getPos() {
    return {curY,curX};
}
