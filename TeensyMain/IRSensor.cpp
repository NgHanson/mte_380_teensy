#include <Arduino.h>
#include "Constants.h"
#include "MathHelper.h"
float getIRDistance() {
  // Serial.println("The pin for this function is not set up right now... Should probably implement it");
  float distance[SENSOR_NUM_SAMPLES];
  for (int i = 0; i < SENSOR_NUM_SAMPLES; i++) {
    float volts = analogRead(IR_SENSOR)*(3.3/1024.0);   // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
    distance[i] = 65*pow(volts, -1.10);          // worked out from graph 65 = theretical distance / (1/Volts)S - luckylarry.co.uk=
  }
  float filteredDistance = filteredMean(distance);
  Serial.println(filteredDistance);                       // print the distance
  return filteredDistance;
}