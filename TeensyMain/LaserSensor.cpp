#include <Wire.h>
#include "LIDARLite.h"
#include "LaserSensor.h"
#include "Constants.h"
#include "MathHelper.h"
LIDARLite myLidarLite;

void setUpLaserSensor() {
  myLidarLite.begin(0, true);  // Set configuration to default and I2C to 400 kHz
  myLidarLite.configure(0); // Change this number to try out alternate configurations
}

float getLaserDistance() {
  float distance[10];
  for (int i = 0; i < SENSOR_NUM_SAMPLES; i++) {
    distance[i] = myLidarLite.distance();
  }
  float filteredDistance = filteredMean(distance);
  // Calibrated this boi in google sheets with measurements every 10cm up to 1m.  It is accurate for front biased weight distribution 
  // return 23.1 - 2.42*filteredDistance + 0.195*pow(filteredDistance, 2) - 6.66*pow(10, 3)*pow(filteredDistance, 3) + 1.11*pow(10, -4)*pow(filteredDistance, 4) - 8.97*pow(10, -7)*pow(filteredDistance, 5) + 2.76*pow(10, -9)*pow(filteredDistance, 6);
  return 1.01*filteredDistance - 15.6;//-112+13.4*filteredDistance-0.587*pow(filteredDistance, 2) + 0.0135*pow(filteredDistance, 3) - 1.63*pow(10, -4)*pow(filteredDistance, 4) + 9.87*pow(10, -7)*pow(filteredDistance, 5) - 2.35*pow(10, -9)*pow(filteredDistance, 6);
}
