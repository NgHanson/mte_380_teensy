#include <Wire.h>
#include "LIDARLite.h"
#include "LaserSensor.h"
#include "Constants.h"
#include "MathHelper.h"
#include "IMU.h"
#include "Globals.h"	
#include <algorithm>    // std::min_element, std::max_element
#include <math.h>
LIDARLite myLidarLite;
#define MOVE_SPEED 200
#define ROTATION_SPEED 60

void setUpLaserSensor() {
  myLidarLite.begin(0, true);  // Set configuration to default and I2C to 400 kHz
  myLidarLite.configure(1); // Change this number to try out alternate configurations
}

float getLaserDistance() {
  float distance[SENSOR_NUM_SAMPLES];
  for (int i = 0; i < SENSOR_NUM_SAMPLES; i++) {
	distance[i] = myLidarLite.distance();
  	while (distance[i] < 1) {
  		Serial.println("less than 1");
  		Serial.println(distance[i]);
  		distance[i] = myLidarLite.distance();	
  	}
  }
  float filteredDistance = filteredMean(distance);
  // Calibrated this boi in google sheets with measurements every 10cm up to 1m.  It is accurate for front biased weight distribution 
  // return 23.1 - 2.42*filteredDistance + 0.195*pow(filteredDistance, 2) - 6.66*pow(10, 3)*pow(filteredDistance, 3) + 1.11*pow(10, -4)*pow(filteredDistance, 4) - 8.97*pow(10, -7)*pow(filteredDistance, 5) + 2.76*pow(10, -9)*pow(filteredDistance, 6);
  // Need to change this from cm to m
  if (filteredDistance < 120) {
  	return (5+-112+13.4*filteredDistance-0.587*pow(filteredDistance, 2) + 0.0135*pow(filteredDistance, 3) - 1.63*pow(10, -4)*pow(filteredDistance, 4) + 9.87*pow(10, -7)*pow(filteredDistance, 5) - 2.35*pow(10, -9)*pow(filteredDistance, 6)) / 100.0;	
  } else {
  	return filteredDistance / 100.0;
  }
  
  // return 1.01*filteredDistance - 15.6;
}

float getFilteredLaserDistance() {
  float distances[10];
  for (int i = 0; i < 10; i++) {
  	distances[i] = getLaserDistance();
  }
  return *std::max_element(distances, distances+10);
  // return filteredMean(distances);
}



void updateRollingMin(float distance) {
	int headingDeg = (int) round(cwHeading);
	if (initialSweepDistances[headingDeg] != 9999) {
		Serial.print("overwriting value at angle ");
		Serial.println(headingDeg);
	}
	if (distance < initialSweepDistances[headingDeg]) {
		initialSweepDistances[headingDeg] = distance;
	}
}
// float minDist = 99999;
// void updateRollingMin(float meas) {
// 	if (meas < minDist) {
// 		minDist = meas;
// 	}
// }

// void resetRollingMin() {
// 	minDist = 99999;
// }
void rotateRightWhileSweeping(int angle) {
  float distance;
  if (angle < 0 || angle >= 360) {
    Serial.println("Angle is invalid");
    return;
  }
  digitalWrite(LEFT_MOTOR_DIR, 1);
  digitalWrite(RIGHT_MOTOR_DIR, 0); 
  getIMUData();
  analogWrite(LEFT_MOTOR_SPEED, ROTATION_SPEED);
  analogWrite(RIGHT_MOTOR_SPEED, ROTATION_SPEED);
  getIMUData();
  // Serial.println(angle);
  // Serial.println(cwHeading);
  if (angle == 0) {
    // angle = 357.5;
    while (!(cwHeading >= 0 && cwHeading < 1)) {
      getIMUData();
              Serial.print("cwHeading: ");
        Serial.print(cwHeading);
        Serial.print(" angle: ");
        Serial.println(angle);
        distance = getLaserDistance();
        updateRollingMin(distance);
      // Serial.println(cwHeading);      
    }
  }
  else {
    Serial.println("rotateright else");
    // angle = angle - 2.5;
    if (cwHeading < angle) {
      Serial.println("cwHeading < angle");
      getIMUData();
      while(cwHeading < angle - 1.5) { //DEFINITELY NEED TO TUNE THIS
        getIMUData();
        Serial.print("cwHeading: ");
        Serial.print(cwHeading);
        Serial.print(" angle: ");
        Serial.println(angle);
        distance = getLaserDistance();
        updateRollingMin(distance);
        Serial.println(distance);
      }
    } else if (cwHeading > angle) {
      Serial.println("cwHeading > angle");
      while (cwHeading < 360 && cwHeading > 10) {
        getIMUData();
        Serial.print("cwHeading: ");
        Serial.print(cwHeading);
        Serial.print(" angle: ");
        Serial.println(angle);
        distance = getLaserDistance();
        updateRollingMin(distance);
      }
      Serial.println("eifeiefjioejfeoj");
      while (cwHeading < angle) {
        getIMUData();
        Serial.print("cwHeading: ");
        Serial.print(cwHeading);
        Serial.print(" angle: ");
        Serial.println(angle);
        distance = getLaserDistance();
        updateRollingMin(distance);
      }
    }    
  }
  // float avg =minDist;
  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);
  delay(100);
  getIMUData();
  Serial.println();
  Serial.println("Printing out distance array");
  // return avg;
}
