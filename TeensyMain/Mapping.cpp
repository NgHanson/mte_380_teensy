#include "Mapping.h"
#include "Globals.h"
#include "Constants.h"
#include "Movement.h"
#include "IMU.h"

//NEED SOME SORT OF ENUMS for objectives

/*
 u - unknown (default)
 v - visited -> do we want to differentiate visited to water, sand, gravel, and normal tiles?
 o - object location (not sure which object though)
 c - candle
 m - magnet
 h - house
 s - survivor
 */

void updateMap(int x, int y, char type) {
  levelMap[x][y] = type;
}


//IF THIS DOESNT WORK WELL/WE DONT HAVE STANDARD STARTING LOCATION, CHANGE TO JUST A ROTATING SCAN
// We will face forward at the same location everytime
// Assume we're starting X: 3 - Y: 0
void initialScan() {
  rotateLeft(90);
  /* @ 270
   X: 0 - Y:0
   X: 1 - Y:0
   X: 2 - Y:0
   */

  //CHANGE THIS TO A FOR LOOP OF ANGLES
  rotateRight(18);
  /* @ 288.434948822922
   X: 0 - Y:1
   */

  rotateRight(8);
  /* @ 296.565051177078
   X: 1 - Y:1
  */

  rotateRight(7);
  /* 303.69006752597977
   X: 0 - Y:2
  */

  rotateRight(11);
  /* 315.0
  X: 0 - Y:3
  X: 1 - Y:2
  X: 2 - Y:1
  */

  rotateRight(8);
  /* 323.13010235415595
  X: 0 - Y:4
  */

  rotateRight(4);
  /*326.30993247402023
  X: 1 - Y:3
  */

  rotateRight(3);
  /*329.03624346792645
  X: 0 - Y:5
  */

  rotateRight(4);
  /* 333.434948822922
  X: 1 - Y:4
  X: 2 - Y:2
  */

  rotateRight(5);
  /*  338.1985905136482
  X: 1 - Y:5
  */

  rotateRight(3);
  /* 341.565051177078
  X: 2 - Y:3
  */

  rotateRight(4);
  /* 345.96375653207355
  X: 2 - Y:4
  */

  rotateRight(3);
  /* 348.69006752597977
  X: 2 - Y:5
  */

  rotateRight(12);
  //ENSURE OUR CURRENT HEADING IS AT 0
  /* 0.0
  X: 3 - Y:0
  X: 3 - Y:1
  X: 3 - Y:2
  X: 3 - Y:3
  X: 3 - Y:4
  X: 3 - Y:5
  */

  rotateRight(11);
  /* 11.309932474020213
  X: 4 - Y:5
  */

  rotateRight(3);
  /*14.036243467926477
  X: 4 - Y:4
  */

  rotateRight(4);
  /*18.43494882292201
  X: 4 - Y:3
  */

  rotateRight(3);
  /*21.80140948635181
  X: 5 - Y:5
  */

  rotateRight(5);
  /*26.56505117707799
  X: 4 - Y:2
  X: 5 - Y:4
  */

  rotateRight(7);
  /* 33.690067525979785
  X: 5 - Y:3
  */

  rotateRight(12);
  /*45.0
  X: 4 - Y:1
  X: 5 - Y:2
  */

  rotateRight(18);
  /* 63.43494882292201
  X: 5 - Y:1
  */

  rotateLeft(90);
  /* @90
   X: 4 - Y:0
   X: 5 - Y:0
   */
  
  //ENSURE OUR CURRENT HEADING IS AT 0
  
}

void sensorMeasurements() {
  
}


// to help figure out if the lidar is seeing the wall or if theres an object present
float lidarToWallDist(int x, int y) {

  float currX = x * TILE_DIST;
  float currY = y * TILE_DIST;
  getIMUData();

  
}
