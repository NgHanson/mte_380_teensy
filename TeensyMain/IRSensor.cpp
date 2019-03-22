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
  // Convert cm to m
  float filteredDistance = filteredMean(distance) / 100.0;
  filteredDistance = -161.93559466001017*pow(filteredDistance, 8) + 1060.3453677908835*pow(filteredDistance, 7) + -3147.829933822888*pow(filteredDistance, 6) + 5684.712695603654*pow(filteredDistance, 5) + -6813.57993204337*pow(filteredDistance, 4) + 5357.072457674748*pow(filteredDistance, 3) + -2576.4560006809734*pow(filteredDistance, 2) + 761.8393170720706*pow(filteredDistance, 1) + -66.8629970143353;
  return filteredDistance / 100.0;
}