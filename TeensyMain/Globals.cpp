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
bool detectedFlame = false;
bool magnetDetected = false;

/*
 u - unknown (default)
 g - gravel
 s - sand
 p - pit
 o - object location (not sure which object though)
 c - candle
 m - magnet
 h - house
 s - survivor
 r - robot start
 */

  /*
    VIEW DIRECTIONS AS
           down
            ^
            |
right < _ _ |_ _ > left
            |
            |
            v
            up
   */

// Global map/position/orientation
char levelMap[6][6] = {          //  y
  {'u', 'u', 'r', 'p', 'u', 'u'},//  0
  {'u', 'g', 'u', 'u', 's', 'u'},//  1
  {'p', 'u', 'u', 'u', 'u', 'u'},//  2
  {'u', 'u', 'u', 's', 'u', 'g'},//  3
  {'u', 's', 'u', 'u', 'p', 'u'},//  4
  {'u', 'u', 'g', 'u', 'u', 'u'}};// 5
//x 0    1    2    3    4    5
// --- THE TV IS ON THIS SIDE --- //

float cwHeading = 0.0;
float frontTilt = 0.0;
float ccwRollFromBack = 0.0;

//Assume starting from location (2, 0);
int xPos = 2;
int yPos = 0;

bool inPit = false;

float initialSweepDistances[360];
float initialSweepFlameVals[360];

void printSweepDistanceArray() {
  for (int i = 0; i < 360; i++) {
  	Serial.print(initialSweepDistances[i]);
  	Serial.print(" ");
  }
  Serial.println();
}

Encoder leftEncoder(LEFT_ENC_1, LEFT_ENC_2);
Encoder rightEncoder(RIGHT_ENC_1, RIGHT_ENC_2);
