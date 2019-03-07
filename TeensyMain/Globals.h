// IF ANY OF OUR GLOBAL VARS START FUCKING UP, THIS MIGHT BE WHY:
// https://stackoverflow.com/questions/5121782/can-non-static-function-modify-a-static-variable-in-c

// Jessens wheel changes make the rock worse.  Keep this in mind for later

//State Variables
static bool flameDetected = false;
static bool flameDone = false;
static bool survivorsDetected = false;
static bool foodDelivered = false;
static bool lostDetected = false;
static bool lostDone = false;
static bool detectedFlame = false;
static bool magnetDetected = false;

// Global map
static int levelMap[6][6];