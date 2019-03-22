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

void setup() {
  delay(1000);
  // colourSetup();
  // hcUltrasonicSetup();
  //setUpLaserSensor();
  //setupIMU();
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
  // Serial.println("Start loop");
  // debugIMUData();
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

   moveForward(120);
   flameDetectionAndExtinguishTest();
   delay(20000);
  //houseDistinguisherTest();
  
   
  // analogWrite(fanPin, 255);
  // moveForward(10);
  //testRotateLeft();
  // rowScanSequence();

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
  //rotateRight(355);
  //initialScan();
  // delay(1000);
}

void houseDistinguisherTest(){
   moveForwardTile();
   rotateLeft(90);
   moveForwardTile();
   rotateRight(90);
   detectTileInFront();
}

void flameDetectionAndExtinguishTest(){
   bool test = flamePresent();
   if(test){
   digitalWrite(fanPin, HIGH);
   delay(1000);
    if(flamePresent()){
      moveForward(100);
      delay(200);
      digitalWrite(fanPin,LOW);
      delay(100);
      flameDetectionAndExtinguishTest();
    }else{
      taskComplete(100);
      taskComplete(100);
      delay(500);
      return;
    }
   }else{
    flameDetectionAndExtinguishTest();
   }
   /*
   if(test){
    digitalWrite(fanPin, HIGH);
    delay(1500);
    digitalWrite(fanPin,LOW);
    taskComplete(100);
    return;
   }else{
    flameDetectionAndExtinguishTest();
   }
  */ 
}

void taskComplete(int blinkLength){
  int blinked = 1;
  digitalWrite(LED_PIN, HIGH);
  for(int i = 0; i < 10; i++){
    blinked ^= 1;
    digitalWrite(LED_PIN, blinked);
    delay(blinkLength);  
  }
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
}
