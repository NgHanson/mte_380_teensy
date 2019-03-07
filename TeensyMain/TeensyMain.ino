/*
A6 - 9 for reg motors, A4,A5,A21,A22 for the motor encoders
IMU code and Motor Encoders/Motor Control not included. Many algorithms neccesary ignored.
Have not included Hall Effect. Wanna Test a bit more]
IMU pind are pins 3 and 4 (not Analog)
*/
#include "Arduino.h"
//pins
#include "Constants.h"

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
//IntervalTimer ultrasonicTimer;
//IntervalTimer hallTimer;
//IntervalTimer motorEncoder1Timer;
//IntervalTimer motorEncoder2Timer;

//read values
int rgbArray[3]; //red,green,blue
bool detectedFlame = false;
bool magnetDetected = false;

//State Variables
bool flameDetected, flameDone, survivorsDetected, foodDelivered, lostDetected, lostDone;

//General Variables
bool flameInfront;

long lenHC_1, lenHC_2;
long lenPing = 10000;
void detectFlame() {
  Serial.println("detectFlame not implemented...");
  // detectedFlame = flameDetected(flamePin); //bool to tell if flame was detected
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
  magnetDetected = analogRead(hallPin); //slightly more complicated than this, but this should be fine
}


void setup() {
  // colourSetup();
  // hcUltrasonicSetup();
  setupIMU();
  Serial.begin(115200);

  // timerSetup();
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
  int count = 0;
  
  getIMUData();
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
  Serial.println("LED pin not chosen yet...");
  // digitalWrite(ledPin, HIGH);
  // delay(1000);
  // digitalWrite(ledPin, LOW);
}
