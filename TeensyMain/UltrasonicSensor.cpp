#include "UltrasonicSensor.h"
#include "Constants.h"
#define trigPin_HCSR04_1 A19
#define echoPin_HCSR04_1 A20
#define trigPin_HCSR04_2 A0
#define echoPin_HCSR04_2 A1
/*
 * HCSR04
 */
void hcUltrasonicSetup(){
  pinMode(trigPin_HCSR04_1, OUTPUT);
  pinMode(echoPin_HCSR04_1, INPUT);
  pinMode(trigPin_HCSR04_2, OUTPUT);
  pinMode(echoPin_HCSR04_2, INPUT);
}

long hcPulse(const int triggerPin, const int echoPin){
  long duration, cm;
  
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  cm = duration * 0.017;
  return cm;
}

/*
 * PARALLAX
 */

float parallaxPulse(){
  float duration, cm;
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(5);
  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  cm = duration * 0.017;
  delay(300);
  return cm / 100.0;
}
