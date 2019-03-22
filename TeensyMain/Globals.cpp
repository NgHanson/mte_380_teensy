#include "Globals.h"
#include "Constants.h"

#include "Coordinate.h"

bool flameDetected = false;
bool flameDone = false;
Coordinate flameTile;

bool survivorsDetected = false;
bool foodDelivered = false;
bool lostDetected = false;
bool lostDone = false;

bool magnetDetected = false;

/*
 u - unknown (default)
 g - gravel
 s - sand (checked for magnet, nothing there)
 p - pit
 o - object location (not sure which object though)
 c - candle
 m - magnet (potentital magnet locations/ unchecked sand)
 h - house
 s - survivor
 r - robot start
 a - avoid (just avoid this tile)
 */

  /*
    VIEW DIRECTIONS AS
           down
            ^
            |
 left < _ _ |_ _ > right
            |
            |
            v
            up
   */

// Global map/position/orientation

// -- GRID 1 -- //
char levelMap[6][6] = {          //  y
  {'u', 'u', 'p', 'r', 'u', 'u'},//  0
  {'u', 'm', 'u', 'u', 'g', 'u'},//  1
  {'u', 'u', 'u', 'u', 'u', 'p'},//  2
  {'g', 'u', 'm', 'u', 'u', 'u'},//  3
  {'u', 'p', 'u', 'u', 'm', 'u'},//  4
  {'u', 'u', 'u', 'g', 'u', 'u'}};// 5
//x 0    1    2    3    4    5

// -- GRID 2 -- //
/*
char levelMap[6][6] = {          //  y
  {'u', 'u', 'p', 'r', 'u', 'u'},//  0
  {'u', 'g', 'u', 'u', 'm', 'u'},//  1
  {'u', 'u', 'u', 'u', 'u', 'g'},//  2
  {'p', 'u', 'u', 'm', 'u', 'u'},//  3
  {'u', 'm', 'u', 'u', 'p', 'u'},//  4
  {'u', 'u', 'u', 'g', 'u', 'u'}};// 5
//x 0    1    2    3    4    5
*/

// -- GRID 3 -- //
/*
char levelMap[6][6] = {          //  y
  {'u', 'u', 'g', 'r', 'u', 'u'},//  0
  {'u', 'm', 'u', 'u', 'p', 'u'},//  1
  {'u', 'u', 'u', 'm', 'u', 'g'},//  2
  {'p', 'u', 'u', 'u', 'u', 'u'},//  3
  {'u', 'g', 'u', 'u', 'm', 'u'},//  4
  {'u', 'u', 'u', 'p', 'u', 'u'}};// 5
//x 0    1    2    3    4    5
*/

// -- GRID 4 -- //
/*
char levelMap[6][6] = {          //  y
  {'u', 'u', 'g', 'r', 'u', 'u'},//  0
  {'u', 'p', 'u', 'u', 'm', 'u'},//  1
  {'u', 'u', 'm', 'u', 'u', 'p'},//  2
  {'g', 'u', 'u', 'u', 'u', 'u'},//  3
  {'u', 'm', 'u', 'u', 'g', 'u'},//  4
  {'u', 'u', 'u', 'p', 'u', 'u'}};// 5
//x 0    1    2    3    4    5
*/

//Assume always starting from location levelMap[0][3];
//levelMap[y][x]
int xPos = 3;
int yPos = 0;

float cwHeading = 0.0;
float frontTilt = 0.0;
float ccwRollFromBack = 0.0;

bool inPit = false;

float initialSweepDistances[5000][2];
int curr_sweep_meas_idx = 0;
float initialSweepFlameVals[360];

void printSweepDistanceArray() {
	Serial.println("printSweepDistanceArray");
  for (int i = 0; i < 500; i++) {
  	Serial.print(initialSweepDistances[i][0]);
  	Serial.print(" ");
  	Serial.println(initialSweepDistances[i][1]);
  }
  Serial.println();
}

Encoder leftEncoder(LEFT_ENC_1, LEFT_ENC_2);
Encoder rightEncoder(RIGHT_ENC_1, RIGHT_ENC_2);
