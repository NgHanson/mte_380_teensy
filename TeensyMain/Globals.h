#include "Encoder.h"
#include "Coordinate.h"

#ifndef GLOBALS_H
#define GLOBALS_H

//State Variables

// -- FLAME -- //
extern bool flameDetected;
extern bool flameDone;
extern Coordinate flameTile;

// -- HOUSES -- //
extern bool survivorsDetected;
extern bool foodDelivered;
extern bool lostDetected;
extern bool lostDone;

// MIGHT BE EASIER TO JUSET SET IT UP LIKE THIS ....
// If we hit the magnets first, doesnt really matter what type of house ...
extern bool houseOneDetected;
extern bool houseTwoDetected;

extern bool houseOneDone;
extern bool houseTwoDone;

extern Coordinate houseTile1;
extern Coordinate houseTile2;
// -- MAGNET -- //
extern bool magnetDetected;

// -- Global Map/Position/Orientation -- //
extern char levelMap[6][6];
extern float cwHeading;
extern float frontTilt;
extern float ccwRollFromBack;

extern int xPos;
extern int yPos;

extern bool inPit;

// -- SCAN ARRAYS -- //
extern float initialSweepDistancesAndFlames[5000][3];
extern int curr_sweep_meas_idx;

// -- ENCODER -- //
extern Encoder leftEncoder;
extern Encoder rightEncoder;


#endif

void printSweepDistanceArray(void);
