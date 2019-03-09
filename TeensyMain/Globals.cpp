#include "Globals.h"
#include "Constants.h"

bool flameDetected = false;
bool flameDone = false;
bool survivorsDetected = false;
bool foodDelivered = false;
bool lostDetected = false;
bool lostDone = false;
bool detectedFlame = false;
bool magnetDetected = false;

// Global map/position/orientation
char levelMap[6][6];
float cwHeading;
float frontTilt = 0.0;
float ccwRollFromBack = 0.0;

bool movementUpDown = true;

bool inPit = false;

Encoder leftEncoder(LEFT_ENC_1, LEFT_ENC_2);
Encoder rightEncoder(RIGHT_ENC_1, RIGHT_ENC_2);
