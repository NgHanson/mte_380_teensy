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

//Timer
//IntervalTimer flameTimer;

//read values
int rgbArray[3]; //red,green,blue

//General Variables
bool flameInfront;

long lenHC_1, lenHC_2;
long lenPing = 10000;
void detectFlame() {
  Serial.println("detectFlame not implemented...");
  // detectedFlame = flameDetected(flamePin); //bool to tell if flame was detected
}


void setup() {
  delay(1000);
  //colourSetup();
  // hcUltrasonicSetup();
  setupIMU();
  // delay(1000);
  // getIMUData();
  // calibrateIMU();
  //setUpLaserSensor();
  Serial.begin(115200);

  // timerSetup();
  pinMode(LEFT_MOTOR_SPEED,OUTPUT);
  pinMode(RIGHT_MOTOR_SPEED,OUTPUT);
  pinMode(LEFT_MOTOR_DIR,OUTPUT);
  pinMode(RIGHT_MOTOR_DIR,OUTPUT);
  pinMode(flamePin, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(IR_SENSOR, INPUT);
  
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
  // debugIMUData();
  //didDetectMagnet();
  //constructionCheckLoop();
  //testEncoders();
  //testIRSensor();
  //testRotationWithIMU();
  // testLaserSensor();
  // int cData[3];
  // colourRead(cData);
  testTileDetection();
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

// void constructionCheckLoop(){

//   moveForward(100);
//   moveForward(-100);
  
//   while(!detectedFlame) {
//     detectFlame();
//   }
//   detectedFlame = false;

//   rotateRight(90);
//   rotateLeft(90);

//   while(lenPing > 10) {
//     ultrasonicPulse();
//   }
//   lenPing = 10000;

//   rotateRight(90);
//   rotateLeft(90);

//   delay(5000);
// }

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

void goToLocation() {

}

void signalComplete(){
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
  Serial.println("LED pin not chosen yet...");
  // digitalWrite(ledPin, HIGH);
  // delay(1000);
  // digitalWrite(ledPin, LOW);
}
