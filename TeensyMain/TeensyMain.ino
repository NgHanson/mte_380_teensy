#include "Arduino.h"

#include "Constants.h"
#include "Globals.h"
#include "LaserSensor.h"
// #include "IRSensor.h"
#include "DetectionHelper.h"
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BNO055.h>
//#include <utility/imumaths.h>


#include "Movement.h"
#include "Mapping.h"
#include "PathFinding.h"
#include "DetectionHelper.h"

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

  // TESTING COMBINED SENSOR =========================================
  // float merged_dist = getMergedDistance();
  // Serial.println(merged_dist);
  // =================================================================

  // TESTING ULTRASONIC SENSOR =======================================
  // Serial.println(parallaxPulse());
  // =================================================================

  // TESTING IR SENSOR ===============================================
  // Serial.println(testIRSensor());
  // =================================================================

  // TESTING LASER SENSOR ============================================
  // Serial.println(getLaserDistance()*100);
  // =================================================================

  // TESTING TIMINGS =================================================
  // unsigned long time = millis();
  // getIMUData();
  // Serial.print("Time required for IMU: ");
  // Serial.print(millis() - time);
  // time = millis();
  // getLaserDistance();
  // Serial.print(" Time required for laserDistance: ");
  // Serial.println(millis() - time);
  // =================================================================

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
  // TINGS==================================================================================

  // MAPS 1, 2, 4 ==========================================================================
  // Make sure to uncomment correct map in mapping file!!!
  // moveForwardTile();
  // moveForwardTile();
  // =======================================================================================

  // MAP 3 =================================================================================
  moveForwardTile();
  rotateLeft(270);
  moveForwardTile();
  rotateRight(0);
  moveForwardTile();
  // =======================================================================================  
  initialScan();
  extinguishFlame();
  rotateRight(180);
  // MAPS 1, 2, 4 ==========================================================================
  // moveForwardTile();
  // moveForwardTile();
  // =======================================================================================  

  // MAP 3 =================================================================================
  moveForwardTile();
  rotateLeft(90);
  moveForwardTile();
  rotateRight(180);
  moveForwardTile();
  // =======================================================================================  
  rotateRight(0);

  lookForMagnet();
  Serial.println("END OF EXECUTION, INFINITE LOOP");
  while(true) {

  }
}

// Sort by tile with the closts euclidean distance
bool compareTile(Coordinate c1, Coordinate c2){
  return euclideanDist(c1.x, xPos, c1.y, yPos) < euclideanDist(c2.x, xPos, c2.y, yPos);
}

void extinguishFlame() {
  int angle = -1;
  int maxFlameVal = 0;
  for (int i = 0; i < curr_sweep_meas_idx; i++) {
    if (initialSweepDistancesAndFlames[i][2] > maxFlameVal) {
      Serial.print("Found more shit ");
      Serial.print(angle);
      Serial.print(" ");
      Serial.println(maxFlameVal);
      maxFlameVal = initialSweepDistancesAndFlames[i][2];
      angle = initialSweepDistancesAndFlames[i][0];
    }
  }
  rotateRight(angle);
  stopMotors();
  digitalWrite(fanPin, HIGH);
  delay(10000);
  Serial.print("Blow dat boi out");
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
    delay(3000);
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
    // GOING TO BE FACING MAGNET
    dipIntoMagnet();
    if (magnetDetected) {
      Serial.println("Magnet detected in pqueue hsit");
      // magnetDetected = true;
      // signalComplete();
      // TODO: Update the state of the grid? ...
      //break;
    } else {
      Serial.println("doneaz");
      delay(1000);
      // TODO: Update the state of the grid? ...
    }
  }
}

void courseLogic() {

  while(!flameDetected) {
    //LOOK FOR FLAME
  }

  while(!flameDone) {
    //TAKE OUT FLAME
  }

  //at this point, flame stuff is done
  while (!magnetDetected) {
    lookForMagnet();
  }

  //at this point, food has been found
  
  // WE NEED UPDATED LOGIC HERE
  if (!survivorsDetected) {
    //look for survivors
  } else if (survivorsDetected && !foodDelivered){
    //go to survivors
  }

  if (!lostDetected) {
    //look for lost
  } else if (lostDetected && !lostDone) {
    // go to lost
  }

  // Return home
  int homePath[MAX_PATH_FINDING_SIZE];
  Coordinate homeTile(0, 3, 'r');
  int homeMoves = getPath(homePath, homeTile);
  executeMovementInstructions(homePath, homeMoves);
  signalComplete();

  while(true){
  }
}

// void signalComplete(){
//   digitalWrite(LED_PIN, HIGH);
//   delay(1000);
//   digitalWrite(LED_PIN, LOW);
//   delay(1000);
// }
