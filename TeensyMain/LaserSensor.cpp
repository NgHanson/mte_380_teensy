#include <Wire.h>
#include "LIDARLite.h"
#include "LaserSensor.h"
LIDARLite myLidarLite;

void setUpLaserSensor() {
  myLidarLite.begin(0, true);  // Set configuration to default and I2C to 400 kHz
  myLidarLite.configure(0); // Change this number to try out alternate configurations
}

float getLaserDistance() {
	return myLidarLite.distance();	
}
