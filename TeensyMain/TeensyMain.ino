#include "Arduino.h"
#include "Encoder.h"
#include "Constants.h"
#include "Globals.h"
#include "LaserSensor.h"
#include "IRSensor.h"
#include "DetectionHelper.h"
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BNO055.h>
//#include <utility/imumaths.h>


#include "MathHelper.h"
#include "ColourSensor.h"
#include "FlameSensor.h"
#include "Movement.h"
#include "UltrasonicSensor.h"
#include "IMU.h"
#include "Mapping.h"
#include "PathFinding.h"

#include "PriorityQueue.h"

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
  //setUpLaserSensor();
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
  
  hallTimer.begin(detectMagnet, 125);
  hallTimer.priority(3);
  */
  Serial.print("Hello: Start");
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

void testTileDetection() {
  detectTileInFront();
}

void testRotationWithIMU() {
  Serial.println("rotateRight(90) ============================");
  rotateRight(90);
  delay(5000);
  Serial.println("rotateRight(180) ============================");
  rotateRight(180);
  delay(5000);
  Serial.println("rotateRight(270) ============================");
  rotateRight(270);
  delay(5000);
  Serial.println("rotateRight(0) ============================");
  rotateRight(0);
  delay(5000);  
}

void testRotateLeft() {
  Serial.println("rotateLeft(270) ============================");
  rotateLeft(270);
  delay(5000);
  Serial.println("rotateLeft(180) ============================");
  rotateLeft(180);
  delay(5000);
  Serial.println("rotateLeft(90) ============================");
  rotateLeft(90);
  delay(5000);
  Serial.println("rotateLeft(0) ============================");
  rotateLeft(0);
  delay(5000);   
}

void testIRSensor() {
  getIRDistance();
}

void testLaserSensor() {
   Serial.println(getLaserDistance());
}

void testEncoders() {
  Serial.print("Left = ");
  Serial.print(leftEncoder.read());
  Serial.print(", Right = ");
  Serial.print(rightEncoder.read());
  Serial.println();
}

// Sort by tile with the closts euclidean distance
bool compareTile(Coordinate c1, Coordinate c2){
  return euclideanDist(c1.x, xPos, c1.y, yPos) < euclideanDist(c2.x, xPos, c2.y, yPos);
}

bool shouldTurnLeft(float startAngle, float finalAngle) {
  float left = startAngle - finalAngle;
  if (left < 0.0) {
    left += 360.0;
  }

  float right = finalAngle - startAngle;
  if (right < 0.0) {
    right += 360.0;
  }

  return left < right;
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
      Serial.println("MOVE FORWARD");
      moveForwardTile();

    } else if (movement == 1) {
      Serial.println("ROTATE TO 0");
      if (shouldTurnLeft(cwHeading, 0.0)) {
        rotateLeft(0);
      } else {
        rotateRight(0);
      }
      
    } else if (movement == 2) {
      Serial.println("ROTATE TO 90");
      if (shouldTurnLeft(cwHeading, 90.0)) {
        rotateLeft(90);
      } else {
        rotateRight(90);
      }

    } else if (movement == 3) {
      Serial.println("ROTATE TO 180");
      if (shouldTurnLeft(cwHeading, 180.0)) {
        rotateLeft(180);
      } else {
        rotateRight(180);
      }
    } else if (movement == 4) {
      Serial.println("ROTATE TO 270");
      if (shouldTurnLeft(cwHeading, 270.0)) {
        rotateLeft(270);
      } else {
        rotateRight(270);
      }
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
    Serial.println("ARRIVE ON TOP OF LOCATION");
    Serial.println("CHECK NEXT LOCATION");
    Serial.println();
    Serial.println();
  }

  Serial.println("WENT TO 3 MAGNET TILE LOCATIONS");
  while(true) {
    
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
