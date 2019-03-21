#include "Encoder.h"
#include "Coordinate.h"

#ifndef GLOBALS_H
#define GLOBALS_H

//State Variables
extern bool flameDetected;
extern bool flameDone;
extern Coordinate flameTile;

extern bool survivorsDetected;
extern bool foodDelivered;
extern bool lostDetected;
extern bool lostDone;
extern bool detectedFlame;
extern bool magnetDetected;

// Global map/position/orientation
extern char levelMap[6][6];
extern float cwHeading;
extern float frontTilt;
extern float ccwRollFromBack;

extern int xPos;
extern int yPos;

extern bool inPit;

extern float initialSweepDistances[5000][2];
extern int curr_sweep_meas_idx;
extern float initialSweepFlameVals[360];
//Encoders
extern Encoder leftEncoder;
extern Encoder rightEncoder;


#endif

void printSweepDistanceArray(void);
