/*
 * Created by jared on 9/24/17.
 *
 * Static class that represents the world. Contains methods
 * to create the initial instance of the map, place and remove
 * drones from the map and generate paths for the drones.
 */

#include <iostream>
#include "World.h"
using namespace std;

//mapSize of the map (mapSize x mapSize map)
int World::mapSize;

//Tracks the number of drones landed so map knows to stop printing.
int World::numDronesLanded = 0;

/*coordinates stored (y,x)
  top left is (0,0)
  bottom left is (mapSize-1,0)*/
map<int,vector<string>> World::world;
map<int,vector<pthread_mutex_t>> World::mWorld;

/*
 * Creates the instance of the map, should only be
 * called once since map is static. The map has a
 * height of mapSize and a width of mapSize*2. The Airport
 * is placed at the bottom left corner of the map.
 *
 * @param mapSize - mapSize of the map
 */
void World::createWorld(int size) {
    size = size;
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j) {
            pthread_mutex_t newMutex;
            pthread_mutex_init(&newMutex,NULL);
            mWorld[i].push_back(newMutex);
            world[i].push_back("~");
        }
    }
    world[size-1][0] = "A"; //drone airport
}

/*
 * Prints the map out.
 */
void World::printMap() {
    for(int i = 0; i < mapSize; ++i) {
        for (int j = 0; j < mapSize; ++j) {
            cout<<" "<<world[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

/*
 * Places drone onto the map at given x and y coordinate,
 * drone at a position is identified by the given drone's ID.
 * If placed outside the boundaries of the map or if a
 * drone is already a the position, an error is signaled.
 *
 * @param y - y position on map to place drone
 * @param x - x position on map to place drone
 * @param ID - id of drone
 *
 * @return 0 if successful, -1 if error occurs
 */
int World::placeDrone(int y, int x, Drone* drone) {
    if (y < 0 || y >= mapSize) {
        return -1;
    }
    if (x < 0 || x >= mapSize) {
        return -1;
    }
    if (pthread_mutex_trylock(&mWorld[y][x]) == EBUSY) {
        return -1;
    }
    world[y][x] = to_string(drone->getID());
    pair<int,int> pos = drone->getPos();
    int oldy = pos.first;
    int oldx = pos.second;
    if ((oldy == mapSize -1) && (oldx == 0)) {
        world[oldy][oldx] = "A";
    } else {
        world[oldy][oldx] = "~";
    }
    pthread_mutex_unlock(&mWorld[oldy][oldx]);
    return 0;
}

/*
 * Removes the drone from the airport on the map when it lands.
 */
void World::land() {
    pthread_mutex_unlock(&mWorld[mapSize -1][0]);
    world[mapSize -1][0] = 'A';
    numDronesLanded += 1;
}

/*
 * Generates a random path for a drone and returns it to
 * the calling object. The first coordinate is the airport
 * and the last is the landing location of the drone. All
 * paths are randomly generated so that every time the scenario
 * is unique.
 *
 * @param startAlt - starting altitude for the given drone
 * @return the path made up of pair coordinates (y,x)
 */
queue<pair<int,int>> World::generatePath(long startAlt){
    queue<pair<int,int>> ret;
    pair<int,int> curPair; //coordinates stored (y,x)
    bool firstPos = false;

    int numSteps = rand()%4; //number of steps to path

    //want odd number of steps so that we can add final desination
    if (numSteps%2 == 0) { //if even
        numSteps += 1; //need at least 1 step and an odd amount
    }

    curPair.first = mapSize -1;
    curPair.second = 0;
    ret.push(curPair);
    curPair.first = startAlt;
    ret.push(curPair);

    //first pos is airport(mapSize -1,0) (needed for move function)
    //second is (startAlt,0)
    //third is (last,rand)
    //fourth is (rand,last)
    //....
    //last is (mapSize -1, last)
    for(int i = 0; i < numSteps; ++i){
        if (firstPos) {
            curPair.first = rand() % mapSize;
            firstPos = false;
        } else {
            curPair.second = rand() % mapSize;
            if (curPair.second == 0) {
                curPair.second += 1;
            }
            firstPos = true;
        }
        ret.push(curPair);
    }

    //want to add final destination at bottom of map (ground)
    curPair.first = mapSize - 1;
    ret.push(curPair);

    return ret;
}