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
  setupIMU();
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

  lookForMagnet();
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
        rotateLeft(0);
      } else {
        rotateRight(0);
      }
      
    } else if (movement == 2) {
      if (shouldTurnLeft(cwHeading, 90.0)) {
        rotateLeft(90);
      } else {
        rotateRight(90);
      }

    } else if (movement == 3) {;
      if (shouldTurnLeft(cwHeading, 180.0)) {
        rotateLeft(180);
      } else {
        rotateRight(180);
      }

    } else if (movement == 4) {
      if (shouldTurnLeft(cwHeading, 270.0)) {
        rotateLeft(270);
      } else {
        rotateRight(270);
      }
    } else {
      //SHOULD SIGNAL SOMETHING
    }

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

    if (didDetectMagnet()) {
      magnetDetected = true;
      signalComplete();
      // TODO: Update the state of the grid? ...
      break;
    } else {
      // TODO: Update the state of the grid? ...
    }

  }
}

int determinePriority() {

  if (!flameDetected){
    return 1; //look for flame
  } else if (flameDetected && !flameDone) {
    return 2; //go to flame
  }

  //at this point, flame stuff is done
  if (!magnetDetected) {
    return 3; //look for magnets
  }

  //at this point, food has been found
  
  //WE MAY NEED A GENERAL LEGO HAS BEEN DETECTED
  if (!survivorsDetected) {
    return 4; //look for survivors
  } else if (survivorsDetected && !foodDelivered){
    return 5; //go to survivors
  }

  if (!lostDetected) {
    return 6; //look for lost
  } else if (lostDetected && !lostDone) {
    return 7; // go to lost
  }

  return 8; // Return home
}

void signalComplete(){
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}
