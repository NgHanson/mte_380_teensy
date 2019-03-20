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
char levelMap[6][6] = {{'u', 'u', 'u', 'u', 'u', 'u'},
{'u', 'u', 'u', 'u', 'u', 'u'},
{'u', 'u', 'u', 'u', 'u', 'u'},
{'u', 'u', 'u', 'u', 'u', 'u'},
{'u', 'u', 'u', 'u', 'u', 'u'},
{'u', 'u', 'u', 'u', 'u', 'u'}};
float cwHeading = 0.0;
float frontTilt = 0.0;
float ccwRollFromBack = 0.0;

//Assume starting from location (3, 0);
int xPos = 3;
int yPos = 0;

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
