#include "Mapping.h"
#include "Globals.h"
#include "Constants.h"
#include "Movement.h"
#include "IMU.h"
#include "LaserSensor.h"
#include "DetectionHelper.h"
#include "FlameSensor.h"

#include <math.h>       /* modf */

void updateMap(int x, int y, char type) {
	Serial.println("UpdateMap");
	Serial.println(x);
	Serial.println(y);
	Serial.println(type);
	if (x >= 0 && x < 6 && y >= 0 && y < 6) {
		levelMap[y][x] = type;		
	} else {
		Serial.println("OUTSIDE OF MAP... SKIPPING");
	}
  // Serial.println(levelMap[y][x]);
}

double getDecimalPlaces(float number) {
Serial.print("getDecimalPlaces");
Serial.print(number);
  double param, fractpart, intpart;
  fractpart = number - (int)number;
  Serial.print(" fractpart: ");
  Serial.print(fractpart);
  return fractpart;
}

void printMap() {
	Serial.println();
	for (int i = 5; i >= 0; i--) {
		for (int j = 0; j < 6; j++) {
			Serial.print(levelMap[i][j]);
			Serial.print(" ");
		}
		Serial.println();
	}
}

void toGridOffset(float angle_deg, float distance, float offset[]) {
	// if (angle_deg >= 180 && angle_deg < 360) {
	// 	angle_deg = 360- angle_deg;
	// 	//y
	// 	offset[0] = (distance+0.15)*sin(angle_deg/DEG_PER_RAD);
	// 	//x
	// 	offset[1] = -(distance+0.15)*cos(angle_deg/DEG_PER_RAD);
	// } else if (angle_deg >= 0 && angle_deg < 180) {
		//y
		offset[0] = (distance)*cos(angle_deg/DEG_PER_RAD);
		//x
		offset[1] = (distance)*sin(angle_deg/DEG_PER_RAD);
	// } else {

	// }
	Serial.println("toGridOffset ");
	Serial.println(angle_deg);

}

void toTileOffset(float offset[], int tileOffset[]) {
	Serial.println("toTileOffset");

	float tileOffsety = offset[0] / TILE_DIST_M;
	float tileOffsetx = offset[1] / TILE_DIST_M;
	double decimals = abs(getDecimalPlaces(tileOffsety));
	if (decimals >= 0.9) {
		tileOffsety += 1;
	}
	decimals = abs(getDecimalPlaces(tileOffsetx));
	if (decimals >= 0.9) {
		tileOffsetx += 1;
	}

	// Serial.println(tileOffsety);
	tileOffset[0] = ceil(tileOffsety);
	tileOffset[1] = ceil(tileOffsetx);
	// if (tileOffsety < 0)
	// Serial.print("tileOffsety ");
	// Serial.print(tileOffsety);
	// double decimals = abs(getDecimalPlaces(tileOffsety));
	// if (decimals >= 0.32) {
	// 	Serial.println("dec at 0 greater than 0.5");
	// 	if (tileOffsety > 0) {
	// 		tileOffsety += 1;
	// 	} else {
	// 		tileOffsety -= 1;
	// 	}
	// }
	// Serial.print(" tileOffsety fixed: ");
	// Serial.println(tileOffsety);
	// tileOffset[0] = round(tileOffsety);
	// float tileOffsetx = offset[1] / TILE_DIST_M;
	// Serial.print("tileOffsetx ");
	// Serial.print(tileOffsetx);
	// decimals = abs(getDecimalPlaces(tileOffsetx));
	// Serial.print("decimals: ");
	// Serial.println(decimals);
	// if (decimals >= 0.32) {
	// 	Serial.println("dec at 1 greater than 0.5");
	// 	if (tileOffsetx > 0) {
	// 		tileOffsetx += 1;
	// 	} else {
	// 		tileOffsetx -= 1;
	// 	}
	// }
	// Serial.print("tileOffsetx fixed ");
	// Serial.println(tileOffsetx);
	// tileOffset[1] = round(tileOffsetx);
	// Serial.print("y: ");
	// Serial.print(tileOffset[0]);
	// Serial.print(" x: ");
	// Serial.print(tileOffset[1]);
	// Serial.println();
}

float getMinDist(float minAngle, float maxAngle, float array[][3], int array_length) {
	Serial.println("getMinDist");
	float min = 9999.0;
	float angle = -1;
	float count = -0;
	float sum = 0;
	for (int i = 0; i < array_length; i++) {
		if (maxAngle > minAngle) {
			if (array[i][0] > minAngle && array[i][0] < maxAngle) {
				// if (array[i][1] < min) {
					// min = array[i][1];
					// angle=  array[i][0];
				// }
				sum += array[i][1];
				count += 1;
			}			
		} else if (maxAngle < minAngle) {
			if (array[i][0] > minAngle && array[i][0] < 360 || array[i][0] < maxAngle && array[i][0] >= 0) {
				// if (array[i][1] < min) {
				// 	min = array[i][1];
				// 	angle=  array[i][0];
				// }
				sum += array[i][1];
				count += 1;
			}
		} else {
			Serial.println("Equal");
		}
	}
	// Serial.print("min: ");
	// Serial.print(min);
	// Serial.print(" angle: ");
	// Serial.println(angle);
	return sum / count;
	// float min = 9999.0;
	// for (int curr = minAngle; curr < maxAngle; curr++) {
	// 	// Serial.print(" curr: ");
	// 	// Serial.print(curr);
	// 	// Serial.print(" maxAngle: ");
	// 	// Serial.print(curr);
	// 	// Serial.print(" minAngle: ");
	// 	// Serial.print(minAngle);
	// 	// Serial.print(" m in ");
	// 	// Serial.println(min);
	// 	if (array[curr] < min) {
	// 		min = array[curr];
	// 	}
	// 	// min = (array[curr] < min) ? array[curr] : min;
	// 	// Serial.print(" curr: ");
	// 	// Serial.print(curr);
	// 	// Serial.print(" maxAngle: ");
	// 	// Serial.print(curr);
	// 	// Serial.print(" minAngle: ");
	// 	// Serial.print(minAngle);
	// 	// Serial.print(" m in ");
	// 	// Serial.println(min);
	// }
	// Serial.println(min);
	return 0.00;
}
// void arrayMax(int arr[]) {

// }
//IF THIS DOESNT WORK WELL/WE DONT HAVE STANDARD STARTING LOCATION, CHANGE TO JUST A ROTATING SCAN
// We will face forward at the same location everytime
// Assume we're starting X: 3 - Y: 0
void initialScan() {
  // for (int i = 0; i < 360; i++) {
  //   initialSweepDistancesAndFlames[i] = 9999;
  // }
  float distances[10];
  float offset[2];
  int tileOffset[2];
  Serial.println("Rotating 90");
  // rotateLeft(265);
  /* @ 270
   X: 0 - Y:0
   X: 1 - Y:0
   X: 2 - Y:0
   */
  rotateRightWhileSweeping(180);
  rotateRightWhileSweeping(0);
  printSweepDistanceArray();
  Serial.println("eieiei");
  Serial.println(curr_sweep_meas_idx);
  // TODO ====================================================================================
  // NEED PROPER ANGLES HERE =================================================================
  // YA ======================================================================================
  // float angles[] = {270, 288.434948822922, 296.565051177078, 303.69006752597977, 315.0, 323.13010235415595, 326.30993247402023, 329.03624346792645, 333.434948822922, 338.1985905136482, 341.565051177078, 345.96375653207355, 348.69006752597977, 0.0, 11.309932474020213, 14.036243467926477, 18.43494882292201, 21.80140948635181, 26.56505117707799, 33.690067525979785, 45.0, 63.43494882292201, 90};
  // ANGLES FOR CASES 1, 2, 4
  float angles[] = {0.0, 333.434948822922, 288.434948822922, 45.0, 326.30993247402023, 18.43494882292201, 303.69006752597977, 63.43494882292201, 315.0, 341.565051177078, 296.565051177078, 33.690067525979785, 26.56505117707799};
  for (int i = 0; i < 23; i++) {
  	float minAngle = angles[i] - 0.5;
  	float maxAngle = angles[i] + 0.5;
  	minAngle = minAngle < 0 ? 360+minAngle : minAngle;
  	maxAngle = maxAngle > 360 ? maxAngle - 360 : maxAngle;
  	Serial.print("minAngle: ");
  	Serial.print(minAngle);
  	Serial.print(" maxAngle: ");
  	Serial.println(maxAngle);
  	float minDist = getMinDist(minAngle, maxAngle, initialSweepDistancesAndFlames, curr_sweep_meas_idx);
  	Serial.print(" minDist: ");
  	Serial.println(minDist);
  	toGridOffset(angles[i], minDist, offset);
  	Serial.print("y: ");
  	Serial.print(offset[0]);
  	Serial.print(" x: ");
  	Serial.println(offset[1]);
  	toTileOffset(offset, tileOffset);
  	Serial.print("Current x pos: ");
  	Serial.print(xPos);
  	Serial.print(" Current y pos: ");
  	Serial.print(yPos);
  	updateMap(xPos + tileOffset[1], yPos + tileOffset[0], 'o');
  	printMap();
  }
  return;
  // Add check for wall - do sweep of walls (empty map) - if we are within a certain tolerance of that, we are a wall otherwise we are an object
  Serial.print("IMU Data: ");
  Serial.print(cwHeading);
  Serial.println("=======================================================");
  // float distance1 = getFilteredLaserDistance();
  // Serial.println("here1");
  // rotateRight(269);
  // float distance2 = getFilteredLaserDistance();
  // Serial.println("here2");
  // rotateRight(273);
  // float distance3 = getFilteredLaserDistance();
  // Serial.println("here3");
  delay(10000);
  Serial.println();
  Serial.println("here");
  // Serial.println(distance0);
  // Serial.println(distance1);
  // Serial.println(distance2);
  // Serial.println(distance3);
  Serial.println("eieieie");
  delay(1000);
  float distance = 100;
  // for (int i = 0; i < 10; i++) {
  // 	distances[i] = getLaserDistance();
  // }
  // float distance = *std::max_element(distances, distances+10);
  Serial.print(" Distance: ");
  Serial.print(distance);
  toGridOffset(cwHeading, distance, offset);
  toTileOffset(offset, tileOffset);
  Serial.print("xPos: ");
  Serial.print(xPos);
  Serial.print("yPos: ");
  Serial.print(yPos);
  updateMap(xPos + (int)tileOffset[0], yPos + (int)tileOffset[1], 'o');
  Serial.print(" Offset: ");
  Serial.print(offset[0]);
  Serial.print(" ");
  Serial.print(offset[1]);
  Serial.print(" Tile Offset: ");
  Serial.print(tileOffset[0]);
  Serial.print(" ");
  Serial.print(tileOffset[1]);
  printMap();
  Serial.print("Map ^");
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

// ------------------------- NEW SCAN METHOD STUFF -------------------------------------- //
float getMinDistance() {
  // float min = 9999.0;

  // int toCheck[] = {358, 359, 0, 1, 2};

  // for (int i = 0; i < 5; i++) {
  //   if (initialSweepDistancesAndFlames[toCheck[i]] < min) {
  //     min = initialSweepDistancesAndFlames[toCheck[i]];
  //   }
  // }
  // return min;
}

void verticalScan(int colNum) {
  // float offset[2];
  // int tileOffset[2];
  // float minDist = getMinDistance();
  // Serial.print("mindist: ");
  // Serial.println(minDist);
  // offset[0] = minDist;
  // offset[1] = 0;
  // //WE FOUND AN OBJECT
  // if (minDist < TILE_DIST_M * 6) {
  // 	toTileOffset(offset, tileOffset);
  // 	Serial.print("Offset x: ");
  // 	Serial.print(offset[1]);
  // 	Serial.print("Offset y: ");
  // 	Serial.println(offset[0]);
  // 	Serial.print("tileOffset x: ");
  // 	Serial.print(tileOffset[1]);
  // 	Serial.print("tileOffset y");
  // 	Serial.println(tileOffset[0]);
  //   // int tileLocation = minDist/TILE_DIST_M;
  //   // double decimals = getDecimalPlaces(minDist);
  //   // if (decimals >= 0.4) {
  //   //   tileLocation++;
  //   // }
    
  //   /*
  //   if (flamePresent()) {
  //     updateMap(colNum, tileLocation,'c');
  //   } else {
  //     updateMap(colNum, tileLocation,'o');
  //   }*/
  //   updateMap(colNum, tileOffset[0],'o');
  // }
}

//ASSUME WE ARE AT col 3, row 0 and the entire row 0 is flat
void rowScanSequence() {
  // for (int i = 0; i < 360; i++) {
  //   initialSweepDistancesAndFlames[i] = 9999;
  // }
  // rotateRight(90);
  // for (int i = 0; i < 2; i++) {
  //   moveForwardTile();
  // }
  // Serial.println("moved fwd 2 tiles");
  // //GET TO col 0, row 0
  // rotateLeft(356);// GET CLOSE ENOUGH TO 0
  // Serial.println("rotateleft 357");
  // rotateRightWhileSweeping(3);
  // Serial.println("rotaterightwhilesleeping");
  // for (int i = 0; i < 360; i++) {
  //   initialSweepDistancesAndFlames[i] = 9999;
  // }
  // //SCAN
  // verticalScan(5);
  // Serial.println("verticalscan");
  // rotateLeft(270);
  // Serial.println("rotateleft 270");
  // for (int j = 4; j >= 0; j--) {
		// 	  for (int i = 0; i < 360; i++) {
		// 	    initialSweepDistancesAndFlames[i] = 9999;
		// 	  }  	
  //     moveForwardTile();
  //     rotateRight(357);// GET CLOSE ENOUGH TO 0
  //     rotateRightWhileSweeping(3); //GET sweep value of 4 degrees
  //     verticalScan(j);
  //     rotateLeft(270);
  // }
  // //ALL COLUMNS SHOULD BE SCANNED AT THIS POINT
  // printMap();
}
