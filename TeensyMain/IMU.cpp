#include "IMU.h"
#include "MathHelper.h"
#include "Globals.h"
#include "Constants.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define MIN_GARBAGE_THRESH 1000
#define MIN_MAGNET_THRESH 110

Adafruit_BNO055 bno = Adafruit_BNO055();
float calibXAngle = 0;
float calibYAngle = 0;
float calibZAngle = 0;

void setupIMU() {
  if(!bno.begin()) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
}

// void calibrateIMU() {
//   delay(1000);
//   float xAngle[10];
//   float yAngle[10];
//   float zAngle[10];
//   for (int i = 0; i < SENSOR_NUM_SAMPLES; i++) {
//     sensors_event_t event; 
//     bno.getEvent(&event);
//     xAngle[i] = event.orientation.x;
//     yAngle[i] = event.orientation.y;
//     zAngle[i] = event.orientation.z;
//   }
//   calibXAngle = filteredMean(xAngle);
//   calibYAngle = filteredMean(yAngle);
//   calibZAngle = filteredMean(zAngle);
// }

// NOTE - CLOCKWISE IS POSITIVE
void _getIMUData() {
  int count = 0;
  float xAngle[10];
  float yAngle[10];
  sensors_event_t event; 
  for (int i = 0; i < SENSOR_NUM_SAMPLES; i++) {
    
    bno.getEvent(&event);  
    xAngle[i] = event.orientation.x;
    yAngle[i] = event.orientation.y;
  }
  cwHeading = filteredMean(xAngle);
  frontTilt = filteredMean(yAngle);

  // if (cwHeading > 0.0) {
  //   digitalWrite(LED_PIN, HIGH);
  // } else {
  //   digitalWrite(LED_PIN, LOW);
  // }
  
  // ccwHeading = event.orientation.x;//(float)((int)(event.orientation.x + 360 - calibXAngle) % 360);
  // // if 
  // frontTilt = event.orientation.y;//(float)((int)(event.orientation.y + 360 - calibYAngle) % 360);
  // ccwRollFromBack = 999;
}

void getIMUData() {
  int currHeading = cwHeading;
  _getIMUData();
  while ((cwHeading > 360 || cwHeading < 0) || abs(currHeading - cwHeading) > 10 && abs(currHeading - cwHeading) < 350) {
    Serial.print("IMU data is bad... ");
    Serial.println(cwHeading);
    cwHeading = currHeading;  
    _getIMUData();
  }

}

bool didDetectMagnet() {
  float mags[10];
  int i = 0;
  while (i < 10) {
    imu::Vector<3> mag_vector = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
    mags[i] = sqrt(mag_vector.x()*mag_vector.x()+mag_vector.y()*mag_vector.y()+mag_vector.z()*mag_vector.z());
    if (mags[i]*mags[i] < MIN_GARBAGE_THRESH) {
      i -= 1;
    }
    i += 1; 
  }
  float averagedMag = filteredMean(mags);
  if (averagedMag > MIN_MAGNET_THRESH) {
    return true;
  } else {
    return false;
  }
}

// Debugging Functions...
void debugIMUData() {
  getIMUData();
  int count = 0;
  sensors_event_t event; 
  bno.getEvent(&event);
  Serial.print("Calibrated - X: ");
  Serial.print(calibXAngle);
  Serial.print(" Y: ");
  Serial.print(calibYAngle);
  Serial.print(" Z: ");
  Serial.println(calibZAngle);
  Serial.print("Sensor Readings - X: ");
  Serial.print(event.orientation.x);
  Serial.print(" Y: ");
  Serial.print(event.orientation.y);
  Serial.print(" Z: ");
  Serial.println(event.orientation.z);
  Serial.println("Eat shit hanson");
  Serial.print("Xcal: ");
  Serial.print(cwHeading);
  Serial.print(" Ycal: ");
  Serial.print(frontTilt);
  Serial.print(" Zcal: ");
  Serial.println(ccwRollFromBack);
  Serial.println("Donez");
  Serial.println("Need to calibrate the IMU data by either adding or subtracting the calibration angle...");
  delay(1000);
}
