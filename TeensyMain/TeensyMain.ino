/*
A6 - 9 for reg motors, A4,A5,A21,A22 for the motor encoders
IMU code and Motor Encoders/Motor Control not included. Many algorithms neccesary ignored.
Have not included Hall Effect. Wanna Test a bit more]
IMU pind are pins 3 and 4 (not Analog)
*/
#include "Arduino.h"
//pins
#define colorS0 A12
#define colorS1 A13
#define colorS2 A14
#define colorS3 A15
#define colorOut A16

#define flamePin A17
#define fanPin A18
#define trigPin_HCSR04_1 A19
#define echoPin_HCSR04_1 A20
#define trigPin_HCSR04_2 A0
#define echoPin_HCSR04_2 A1

const int pingPin = A2;

#define LEFT_MOTOR_SPEED A6
#define RIGHT_MOTOR_SPEED A7
#define LEFT_MOTOR_DIR A8
#define RIGHT_MOTOR_DIR A9
#define LEFT_ENC_1 A4
#define LEFT_ENC_2 A5
#define RIGHT_ENC_1 A21
#define RIGHT_ENC_2 A22
#define IMU_3 A3
#define IMU_4 A4

const int hallPin = A3;

//#include <Adafruit_Sensor.h>
//#include <Adafruit_BNO055.h>
//#include <utility/imumaths.h>

#include "MathHelper.h"
#include "ColourSensor.h"
#include "FlameSensor.h"
#include "Movement.h"
#include "UltrasonicSensor.h"
#include "IMU.h"

//Timer
//IntervalTimer flameTimer;
//IntervalTimer ultrasonicTimer;
//IntervalTimer hallTimer;
//IntervalTimer motorEncoder1Timer;
//IntervalTimer motorEncoder2Timer;

//read values
int rgbArray[3]; //red,green,blue
bool detectedFlame = false;
bool magnetDetected = false;
long lenHC_1, lenHC_2;
long lenPing = 10000;
void detectFlame() {
  detectedFlame = flameDetected(flamePin); //bool to tell if flame was detected
}

void ultrasonicPulse() {
  //lenHC_1 = hcPulse(trigPin_HCSR04_1, echoPin_HCSR04_1);
  //lenHC_2 = hcPulse(trigPin_HCSR04_2, echoPin_HCSR04_2);
  float data[10];
  float mean, std_dev;
  for (int i = 0;i < 10; i++){
    data[i] = parallaxPulse(pingPin);
  }
  lenPing = filteredMean(data);
  Serial.print(lenPing);
  Serial.println(" cm");
}

void detectMagnet() {
  magnetDetected = digitalRead(hallPin); //slightly more complicated than this, but this should be fine
}



void setup() {
//  colourSetup();
  //hcUltrasonicSetup();

  Serial.begin(115200);

  //timerSetup();
  pinMode(LEFT_MOTOR_SPEED,OUTPUT);
  pinMode(RIGHT_MOTOR_SPEED,OUTPUT);
  pinMode(LEFT_MOTOR_DIR,OUTPUT);
  pinMode(RIGHT_MOTOR_DIR,OUTPUT);
  pinMode(flamePin, INPUT);
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
//  constructionCheckLoop();
}

void constructionCheckLoop(){

  moveForward(100);
  moveForward(-100);
  
  while(!detectedFlame) {
    detectFlame();
  }
  detectedFlame = false;

  rotateCW(90);
  rotateCCW(90);

  while(lenPing > 10) {
    ultrasonicPulse();
  }
  lenPing = 10000;

  rotateCW(90);
  rotateCCW(90);

  delay(5000);
}
