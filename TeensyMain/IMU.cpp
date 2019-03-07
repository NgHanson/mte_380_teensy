#include "IMU.h"
#include "MathHelper.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define NUM_FILTERS 10

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

void calibrateIMU(){
  delay(1000);
  float xAngle[10];
  float yAngle[10];
  float zAngle[10];
  for (int i = 0; i < NUM_FILTERS; i++) {
    sensors_event_t event; 
    bno.getEvent(&event);
    xAngle[i] = event.orientation.x;
    yAngle[i] = event.orientation.y;
    zAngle[i] = event.orientation.z;
  }
  calibXAngle = filteredMean(xAngle);
  calibYAngle = filteredMean(yAngle);
  calibZAngle = filteredMean(zAngle);
}

void getIMUData() {
  int count = 0;
  sensors_event_t event; 
  bno.getEvent(&event);
  Serial.println("Here");
  Serial.println(calibXAngle);
  Serial.println(calibYAngle);
  Serial.println(calibZAngle);
  Serial.println("Donez");
  Serial.println("Need to calibrate the IMU data by either adding or subtracting the calibration angle...");
}
