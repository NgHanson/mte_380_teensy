// Jessens wheel changes make the rock worse.  Keep this in mind for later
#include "Encoder.h"

#ifndef GLOBALS_H
#define GLOBALS_H

//State Variables
extern bool flameDetected;
extern bool flameDone;
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

extern int totalHorzCount;
extern int totalVertCount;

extern bool inPit;

//Encoders
extern Encoder leftEncoder;
extern Encoder rightEncoder;


#endif
