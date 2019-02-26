//#include <i2c_t3.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055();

void imuTestSetup() {

  Serial.begin(115200);
  Serial.println("Orientation Sensor Test");
  /* Initialise the sensor */
  if(!bno.begin())
  {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
  int8_t temp = bno.getTemp();
  Serial.print("D Current Temperature: ");
  Serial.print(temp);
  Serial.println(" C");
  bno.setExtCrystalUse(true);
  Serial.println("Calibration status values: 0=uncalibrated, 3=fully calibrated");
  Serial.println("D IMU Initialized");
}

int count = 0;
void imuTestLoop() {
  sensors_event_t event; 
  bno.getEvent(&event);
  
  /* Display the floating point data */
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);
  Serial.println("");
  int8_t temp = bno.getTemp();
  Serial.print("D Current Temperature: ");
  Serial.print(temp);
  Serial.println(" C");
  delay(1000);
  Serial.print("Count: ");
  Serial.println(count);
  count += 1;
  
}
