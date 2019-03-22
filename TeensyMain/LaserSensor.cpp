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
  float filteredDistance = filteredMean(distance) / 100.0;
  // double filteredDistance = (double)filteredDistance0;
  // Calibrated this boi in google sheets with measurements every 10cm up to 1m.  It is accurate for front biased weight distribution 
  // return 23.1 - 2.42*filteredDistance + 0.195*pow(filteredDistance, 2) - 6.66*pow(10, 3)*pow(filteredDistance, 3) + 1.11*pow(10, -4)*pow(filteredDistance, 4) - 8.97*pow(10, -7)*pow(filteredDistance, 5) + 2.76*pow(10, -9)*pow(filteredDistance, 6);
  // Need to change this from cm to m
  Serial.print("Distance: ");
  	Serial.println(filteredDistance);
  if (filteredDistance < 0.120) {
  	// Serial.print("Filtereddistance: ");
  	// Serial.print(filteredDistance);
  	// return (filteredDistance/100)-10;
  	// return (-215 + 27*filteredDistance - 1.33*pow(filteredDistance, 2) + 0.0362*pow(filteredDistance, 3) - 5.85*pow(10, -4)*pow(filteredDistance, 4) + 5.77*pow(10, -6)*pow(filteredDistance, 5) - 3.35*pow(10, -8)*pow(filteredDistance, 6) + 1.03*pow(10, -10)*pow(filteredDistance, 7) - 1.19*pow(10, -13)*pow(filteredDistance, 8));
  	//return (41.2 - 8.87*filteredDistance + 0.821*pow(filteredDistance, 2) - 0.0355*pow(filteredDistance, 3) + 8.86*pow(10, -4)*pow(filteredDistance, 4) - 1.33*pow(10, -5)*pow(filteredDistance, 5) + 1.18*pow(10, -7)*pow(filteredDistance, 6) - 5.77*pow(10, -10)*pow(filteredDistance, 7) + 1.18*pow(10, -12)*pow(filteredDistance, 8)) / 100.0;
  	// return filteredDistance / 100.0; 
  	// Serial.print(" adjusted: ");
  	// Serial.println();
  	// Serial.print((-29.75 + 10.509*filteredDistance - 0.97408*pow(filteredDistance, 2) + 0.050537*pow(filteredDistance, 3) - 0.0014796*pow(filteredDistance, 4) + 0.00002569393*pow(filteredDistance, 5) - 2.628012*pow(10,-7)*pow(filteredDistance, 6) + 1.46227*pow(10,-9)*pow(filteredDistance, 7) -3.4102*pow(10, -12)*pow(filteredDistance, 8)) / 100.0);
  	// Serial.print((41.2 - 8.87*filteredDistance + 0.821*pow(filteredDistance, 2) - 0.0355*pow(filteredDistance, 3) + 8.86*pow(10, -4)*pow(filteredDistance, 4) - 1.33*pow(10, -5)*pow(filteredDistance, 5) + 1.18*pow(10, -7)*pow(filteredDistance, 6) - 5.77*pow(10, -10)*pow(filteredDistance, 7) + 1.18*pow(10, -12)*pow(filteredDistance, 8)) / 100.0);
  	// Serial.print(" adjusted 6th degree: ");
  	// Serial.println((5+-112+13.4*filteredDistance-0.587*pow(filteredDistance, 2) + 0.0135*pow(filteredDistance, 3) - 1.63*pow(10, -4)*pow(filteredDistance, 4) + 9.87*pow(10, -7)*pow(filteredDistance, 5) - 2.35*pow(10, -9)*pow(filteredDistance, 6)) / 100.0);
  	// Serial.println(" new 6th dg ");
  	// Serial.println((-1.84+9.7*pow(10, -4)*filteredDistance + 0.0816*pow(filteredDistance, 2) - 2.84*pow(10, -3)*pow(filteredDistance, 3) + 4.5*pow(10, -5)*pow(filteredDistance, 4) - 3.3*pow(10, -7)*pow(filteredDistance, 5) + 9.12*pow(10, -10)*pow(filteredDistance, 6)) / 100.0);

  	return (-1.84 - 0.097*filteredDistance + 816*pow(filteredDistance,2) - 2840*pow(filteredDistance,3) + 4504*pow(filteredDistance,4) - 3303*pow(filteredDistance,5) + 912*pow(filteredDistance,6));
  	// return (5+-112+13.4*filteredDistance-0.587*pow(filteredDistance, 2) + 0.0135*pow(filteredDistance, 3) - 1.63*pow(10, -4)*pow(filteredDistance, 4) + 9.87*pow(10, -7)*pow(filteredDistance, 5) - 2.35*pow(10, -9)*pow(filteredDistance, 6)) / 100.0;	
  	// return (5+-112+13.4*filteredDistance-0.587*pow(filteredDistance, 2) + 0.0135*pow(filteredDistance, 3) - 1.63*pow(10, -4)*pow(filteredDistance, 4) + 9.87*pow(10, -7)*pow(filteredDistance, 5) - 2.35*pow(10, -9)*pow(filteredDistance, 6)) / 100.0;	
  } else {
  	return filteredDistance / 100.0;
  }
  
  // return 1.01*filteredDistance - 15.6;
}

float getRawLaserDistance() {
	return getLaserDistance();
} 

float getFilteredLaserDistance() {
  float distances[10];
  for (int i = 0; i < 10; i++) {
  	distances[i] = getLaserDistance();
  }
  return *std::max_element(distances, distances+10);
  // return filteredMean(distances);
}



// void updateRollingMin(float distance) {
// 	Serial.print("update rolling min with distance: ");
// 	Serial.println(distance);
// 	initialSweepDistances[curr_sweep_meas_idx][0] = cwHeading;
// 	initialSweepDistances[curr_sweep_meas_idx][1] = distance;
// 	curr_sweep_meas_idx += 1;
// }
// float minDist = 99999;
// void updateRollingMin(float meas) {
// 	if (meas < minDist) {
// 		minDist = meas;
// 	}
// }

// void resetRollingMin() {
// 	minDist = 99999;
// }
// void rotateRightWhileSweeping(int angle) {
//   float distance;
//   if (angle < 0 || angle >= 360) {
//     Serial.println("Angle is invalid");
//     return;
//   }
//   digitalWrite(LEFT_MOTOR_DIR, 1);
//   digitalWrite(RIGHT_MOTOR_DIR, 0); 
//   getIMUData();
//   analogWrite(LEFT_MOTOR_SPEED, ROTATION_SPEED);
//   analogWrite(RIGHT_MOTOR_SPEED, ROTATION_SPEED);
//   getIMUData();
//   // Serial.println(angle);
//   // Serial.println(cwHeading);
//   if (angle == 0) {
//     // angle = 357.5;
//     while (!(cwHeading >= 0 && cwHeading < 1)) {
//       getIMUData();
//               Serial.print("cwHeading: ");
//         Serial.print(cwHeading);
//         Serial.print(" angle: ");
//         Serial.println(angle);
//         distance = getRawLaserDistance();
//         updateRollingMin(distance);
//       // Serial.println(cwHeading);      
//     }
//   }
//   else {
//     Serial.println("rotateright else");
//     // angle = angle - 2.5;
//     if (cwHeading < angle) {
//       Serial.println("cwHeading < angle");
//       getIMUData();
//       while(cwHeading < angle - 1.5) { //DEFINITELY NEED TO TUNE THIS
//         getIMUData();
//         Serial.print("cwHeading: ");
//         Serial.print(cwHeading);
//         Serial.print(" angle: ");
//         Serial.println(angle);
//         distance = getRawLaserDistance();
//         updateRollingMin(distance);
//         Serial.println(distance);
//       }
//     } else if (cwHeading > angle) {
//       Serial.println("cwHeading > angle");
//       while (cwHeading < 360 && cwHeading > 10) {
//         getIMUData();
//         Serial.print("cwHeading: ");
//         Serial.print(cwHeading);
//         Serial.print(" angle: ");
//         Serial.println(angle);
//         distance = getRawLaserDistance();
//         updateRollingMin(distance);
//       }
//       Serial.println("eifeiefjioejfeoj");
//       while (cwHeading < angle) {
//         getIMUData();
//         Serial.print("cwHeading: ");
//         Serial.print(cwHeading);
//         Serial.print(" angle: ");
//         Serial.println(angle);
//         distance = getRawLaserDistance();
//         updateRollingMin(distance);
//       }
//     }    
//   }
//   // float avg =minDist;
//   analogWrite(LEFT_MOTOR_SPEED, 0);
//   analogWrite(RIGHT_MOTOR_SPEED, 0);
//   delay(100);
//   getIMUData();
//   Serial.println();
//   Serial.println("Printing out distance array");
//   // return avg;
// }
