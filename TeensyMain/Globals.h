// Jessens wheel changes make the rock worse.  Keep this in mind for later

#ifndef GLOBALS_H
#define GLOBALS_H

//State Variables
static bool flameDetected;
static bool flameDone;
static bool survivorsDetected;
static bool foodDelivered;
static bool lostDetected;
static bool lostDone;
static bool detectedFlame;
static bool magnetDetected;

// Global map/position/orientation
static char levelMap[6][6];
extern float cwHeading;
static float frontTilt;
static float ccwRollFromBack;

static bool movementUpDown;

static bool inPit;

#endif
