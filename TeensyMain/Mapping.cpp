#include "Mapping.h"
#include "Globals.h"
#include "Constants.h"
#include "Movement.h"
#include "IMU.h"
#include "LaserSensor.h"

#include <math.h>       /* modf */


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

double getDecimalPlaces(float number) {
  double param, fractpart, intpart;
  fractpart = modf(param , &intpart);
  return fractpart;
}

void toGridOffset(float angle_deg, float distance, float offset[]) {
	if (angle_deg >= 270 && angle_deg < 360) {
		angle_deg = angle_deg - 270;
	} else if (angle_deg >= 0 && angle_deg < 90) {
		angle_deg = 90 - angle_deg;
	}
	offset[0] = distance*cos(angle_deg/DEG_PER_RAD);
	offset[1] = distance*sin(angle_deg/DEG_PER_RAD);
}

void toTileOffset(float offset[], float tileOffset[]) {
	tileOffset[0] = offset[0] / TILE_DIST_M;
	double decimals = getDecimalPlaces(tileOffset[0]);
	if (decimals > 0.5) {
		tileOffset[0] += 1;
	}
	tileOffset[1] = offset[1] / TILE_DIST_M;
	decimals = getDecimalPlaces(tileOffset[1]);
	if (decimals > 0.5) {
		tileOffset[1] += 1;
	}
}

// void arrayMax(int arr[]) {

// }
//IF THIS DOESNT WORK WELL/WE DONT HAVE STANDARD STARTING LOCATION, CHANGE TO JUST A ROTATING SCAN
// We will face forward at the same location everytime
// Assume we're starting X: 3 - Y: 0
void initialScan() {
  float distances[10];
  float offset[2];
  float tileOffset[2];
  Serial.println("Rotating 90");
  rotateLeft(270);
  /* @ 270
   X: 0 - Y:0
   X: 1 - Y:0
   X: 2 - Y:0
   */
  Serial.print("IMU Data: ");
  Serial.print(cwHeading);
  float distance = getFilteredLaserDistance();

  // for (int i = 0; i < 10; i++) {
  // 	distances[i] = getLaserDistance();
  // }
  // float distance = *std::max_element(distances, distances+10);
  Serial.print(" Distance: ");
  Serial.print(distance);
  toGridOffset(cwHeading, distance, offset);
  toTileOffset(offset, tileOffset);
  updateMap(xTile + (int)tileOffset[0], yTile + (int)tileOffset[1], 'o');
  Serial.print(" Offset: ");
  Serial.print(offset[0]);
  Serial.print(" ");
  Serial.print(offset[1]);
  Serial.print(" Tile Offset: ");
  Serial.print(tileOffset[0]);
  Serial.print(" ");
  Serial.print(tileOffset[1]);
  delay(5000);
  //CHANGE THIS TO A FOR LOOP OF ANGLES
  Serial.println("Rotating 18");
  rotateRight(288.434948822922);
  /* @ 288.434948822922
   X: 0 - Y:1
   */
  Serial.print("IMU Data: ");
  Serial.print(cwHeading);
  distance = getLaserDistance();
  Serial.print(" Distance: ");
  Serial.println(distance);
  delay(5000);
  rotateRight(296.565051177078);
  /* @ 296.565051177078
   X: 1 - Y:1
  */
  delay(5000);
  rotateRight(303.69006752597977);
  /* 303.69006752597977
   X: 0 - Y:2
  */
  delay(5000);
  rotateRight(315.0);
  /* 315.0
  X: 0 - Y:3
  X: 1 - Y:2
  X: 2 - Y:1
  */

  rotateRight(323.13010235415595);
  /* 323.13010235415595
  X: 0 - Y:4
  */

  rotateRight(326.30993247402023);
  /*326.30993247402023
  X: 1 - Y:3
  */

  rotateRight(329.03624346792645);
  /*329.03624346792645
  X: 0 - Y:5
  */

  rotateRight(333.434948822922);
  /* 333.434948822922
  X: 1 - Y:4
  X: 2 - Y:2
  */

  rotateRight(338.1985905136482);
  /*  338.1985905136482
  X: 1 - Y:5
  */

  rotateRight(341.565051177078);
  /* 341.565051177078
  X: 2 - Y:3
  */

  rotateRight(345.96375653207355);
  /* 345.96375653207355
  X: 2 - Y:4
  */

  rotateRight(348.69006752597977);
  /* 348.69006752597977
  X: 2 - Y:5
  */

  rotateRight(0.0);
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

  float currX = x * TILE_DIST_M;
  float currY = y * TILE_DIST_M;
  getIMUData();

  
}
