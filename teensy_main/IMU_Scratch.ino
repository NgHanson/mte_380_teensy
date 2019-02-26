//#include <i2c_t3.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
//extern TwoWire Wire2;
Adafruit_BNO055 bno = Adafruit_BNO055();

void setup() {
//Wire2.begin();
//  CORE_PIN18_CONFIG = 0;  // turn off primary pins before enable alternates
//  CORE_PIN19_CONFIG = 0;
//  CORE_PIN4_CONFIG = PORT_PCR_MUX(2)|PORT_PCR_ODE|PORT_PCR_SRE|PORT_PCR_DSE;
//  CORE_PIN3_CONFIG = PORT_PCR_MUX(2)|PORT_PCR_ODE|PORT_PCR_SRE|PORT_PCR_DSE;
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Orientation Sensor Test");
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
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
void loop() {
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
