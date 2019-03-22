#include "Arduino.h"

#include "Constants.h"
#include "Globals.h"

#include "Movement.h"
#include "Mapping.h"
#include "PathFinding.h"

#include "UltrasonicSensor.h"
#include "IMU.h"
#include "ColourSensor.h"
#include "FlameSensor.h"
#include "LaserSensor.h"
#include "IRSensor.h"

#include "Encoder.h"
#include "PriorityQueue.h"

#include "DetectionHelper.h"
#include "MathHelper.h"

//Timer
//IntervalTimer flameTimer;

//read values
int rgbArray[3]; //red,green,blue

//General Variables
bool flameInfront;

long lenHC_1, lenHC_2;
long lenPing = 10000;

void setup() {
  delay(1000);
  // colourSetup();
  // hcUltrasonicSetup();
  // setUpLaserSensor();
  // setupIMU();
  // delay(1000);
  // getIMUData();
  // calibrateIMU();
  
  Serial.begin(115200);
  
  pinMode(LEFT_MOTOR_SPEED,OUTPUT);
  pinMode(RIGHT_MOTOR_SPEED,OUTPUT);
  pinMode(LEFT_MOTOR_DIR,OUTPUT);
  pinMode(RIGHT_MOTOR_DIR,OUTPUT);
  pinMode(FLAME_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(IR_SENSOR, INPUT);
  pinMode(fanPin, OUTPUT);
  
  // timerSetup();
}


//Lower value, higher priority
void timerSetup() {
  //flameTimer.begin(detectFlame, 500);
  //flameTimer.priority(3);
  /*
  ultrasonicTimer.begin(ultrasonicPulse, 125);
  ultrasonicTimer.priority(3);
  */
}


void loop() {
  //didDetectMagnet();
  //constructionCheckLoop();
  //testEncoders();
  //testIRSensor();
  //testRotationWithIMU();
  //testLaserSensor();
  // int cData[3];
  // colourRead(cData);
  // printColorValues(cData);
  // testTileDetection();
  // detectFlame();
  // analogWrite(fanPin, 255);
  // moveForward(10);
  //testRotateLeft();

  // Move Forward One Tile =======================================
  // moveForwardTile();
  // delay(5000);
  // =============================================================
  
  // Calibrate Left And Right Turns ==============================
  // calibrateRotateRight();
  // calibrateRotateLeft();
  // delay(5000);
  // =============================================================
  // rowScanSequence();
  //rotateRight(355);
  //initialScan();

  /*
  int results[MAX_PATH_FINDING_SIZE];
  Coordinate testLocation(0, 4, '-');
  int numHouseOneMoves = getPath(results, testLocation);
  executeMovementInstructions(results, numHouseOneMoves);
  */
  //lookForMagnet();

  Coordinate flameCodeCoord(1, 3,'c');
  flameTile = flameCodeCoord;
  flameDetected = true;

  Coordinate house1Coord(2, 5,'h');
  houseTile1 = house1Coord;
  houseOneDetected = true;

  Coordinate house2Coord(5, 1,'h');
  houseTile2 = house2Coord;
  houseTwoDetected = true;

  courseLogic();

  Serial.println("END OF EXECUTION, INFINITE LOOP");
  while(true) {

  }
}

// Sort by tile with the closts euclidean distance
bool compareTile(Coordinate c1, Coordinate c2){
  return euclideanDist(c1.x, xPos, c1.y, yPos) < euclideanDist(c2.x, xPos, c2.y, yPos);
}

/*
  a set of movement instructions will be given as an array of integers that will be decoded
  0 - forward
  1 - rotate 0
  2 - rotate 90
  3 - rotate 180
  4 - rotate 270
*/
void executeMovementInstructions(int movements[], int numMoves) {

  for (int i = 0; i < numMoves; i++) {
    int movement = movements[i];

    if (movement == 0) {
      moveForwardTile();

    } else if (movement == 1) {
      if (shouldTurnLeft(cwHeading, 0.0)) {
        //rotateLeft(0);
      } else {
        //rotateRight(0);
      }
      cwHeading = 0.0; //FOR TESTING

    } else if (movement == 2) {
      if (shouldTurnLeft(cwHeading, 90.0)) {
        //rotateLeft(90);
      } else {
        //rotateRight(90);
      }
      cwHeading = 90.0; //FOR TESTING

    } else if (movement == 3) {;
      if (shouldTurnLeft(cwHeading, 180.0)) {
        //rotateLeft(180);
      } else {
        //rotateRight(180);
      }
      cwHeading = 180.0; //FOR TESTING

    } else if (movement == 4) {
      if (shouldTurnLeft(cwHeading, 270.0)) {
        //rotateLeft(270);
      } else {
        //rotateRight(270);
      }
      cwHeading = 270.0; //FOR TESTING

    } else {
      //SHOULD SIGNAL SOMETHING
    }

  }
}

void goToHouse() {

  int results[MAX_PATH_FINDING_SIZE];
  //TODO: SETUP LOGIC IF NO HOUSES FOUND, ONE FOUND ... cases that we need to scan more ...
  if (houseOneDetected && !houseOneDone) {
    int numHouseOneMoves = getPath(results, houseTile1);
    executeMovementInstructions(results, numHouseOneMoves);
    // TODO: Detect house type? flash for delivering food...
    houseOneDone = true;
    signalComplete();
  }

  if (houseTwoDetected && !houseTwoDone) {
    int numHouseTwoMoves = getPath(results, houseTile2);
    executeMovementInstructions(results, numHouseTwoMoves);
    // TODO: Detect house type? flash for delivering food...
    houseTwoDone = true;
    signalComplete();
  }
}

// Will go to all magnet tiles (sand tiles that we havent confirmed a magnet is in)
void lookForMagnet() {
  PriorityQueue<Coordinate> pq = PriorityQueue<Coordinate>(compareTile);
  for(int y = 0; y < 6; y++) {
    for (int x = 0; x < 6; x++) {

      if (levelMap[y][x] == 'm') {
        Coordinate magnetTile(x, y,'m');
        pq.push(magnetTile);
      }

    }
  }

  int results[MAX_PATH_FINDING_SIZE];
  while(!pq.isEmpty()) {
    Coordinate closestMagnetTile = pq.pop();
    int numMoves = getPath(results, closestMagnetTile);
    executeMovementInstructions(results, numMoves);

    /*
    if (didDetectMagnet()) {
      magnetDetected = true;
      signalComplete();
      // TODO: Update the state of the grid? ...
      break;
    } else {
      delay(1000);
      // TODO: Update the state of the grid? ...
    }*/
    Serial.println("AT MAGNET LOCATION");
    Serial.println();
  }
}

void courseLogic() {

  int flamePath[MAX_PATH_FINDING_SIZE];
  int flameMoves = getPath(flamePath, flameTile);
  executeMovementInstructions(flamePath, flameMoves);
  signalComplete();
  delay(1000);

  /*
  lookForMagnet();
  //at this point, food has been found
  
  int house1Path[MAX_PATH_FINDING_SIZE];
  int house1Moves = getPath(house1Path, houseTile1);
  executeMovementInstructions(house1Path, house1Moves);
  signalComplete();
  //SCAN HOUSE HERE - CUSTOM DELAY BASED ON COLOUR

  int house2Path[MAX_PATH_FINDING_SIZE];
  int house2Moves = getPath(house2Path, houseTile2);
  executeMovementInstructions(house2Path, house2Moves);
  signalComplete();

  // Return home
  int homePath[MAX_PATH_FINDING_SIZE];
  Coordinate homeTile(0, 3, 'r');
  int homeMoves = getPath(homePath, homeTile);
  executeMovementInstructions(homePath, homeMoves);
  signalComplete();
*/
  while(true){
  }
}

void signalComplete(){
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}
