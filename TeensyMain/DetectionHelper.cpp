#include "DetectionHelper.h"
#include "ColourSensor.h"
#include "Constants.h"
#include "IRSensor.h"
#include "UltrasonicSensor.h"
#include "FlameSensor.h"
#include "Globals.h"

void detectTileInFront() {
  // int cData[3];
  // colourRead(cData);
  // float r_val = cData[0];
  // float g_val = cData[1];
  // float b_val = cData[2];
  // float colMagAvg = sqrt(pow(r_val, 2) + pow(g_val, 2) + pow(b_val, 2));
  // printColourDistribution(r_val, g_val, b_val);
  // float distance = getIRDistance();
  // Serial.print(" IR Distance: ");
  // Serial.println(distance);
  // // TODO: Implement this logic...  
  // bool in_front_of_obj = false;
  
  // if (in_front_of_obj) {
  //   if (r_val/colMagAvg > 0.29 && r_val/colMagAvg < 0.31 &&
  //       g_val/colMagAvg > 0.57 && g_val/colMagAvg < 0.64 &&
  //       b_val/colMagAvg > 0.70 && b_val/colMagAvg < 0.75) {
  //       Serial.println("Yellow House");
  //   } else {
  //     Serial.println("Red House");
  //   }
  // } else {
  //   if (distance > 14) {
  //     Serial.println("Hole");
  //   } else if (distance < 13) {
  //     Serial.println("Wood Tile");
  //   } else {
  //     if (r_val/colMagAvg > 0.28 && r_val/colMagAvg < 0.4 &&
  //     g_val/colMagAvg > 0.64 && g_val/colMagAvg < 0.69 &&
  //     b_val/colMagAvg > 0.64 && b_val/colMagAvg < 0.69 && colMagAvg > 400) {
  //       Serial.println("Sand");
  //     } else {
  //       Serial.println("Rocks");
  //     }
  //   }
  // }
}

float getMergedDistance() {
  Serial.print("IR Distance: ");
  float ir_distance = getIRDistance();
  Serial.print(ir_distance);
  // IR sensor is 20-150
  if (ir_distance < 0.3) {
    Serial.print("Need us distance - Distance: ");
    float us_distance = parallaxPulse();
    Serial.println(us_distance);
    return us_distance;
  }
  return ir_distance;
}

void updateHeadingVals() {
  float distance = getMergedDistance();
  int flameVal = getFlameValue();
  Serial.println(flameVal);
  initialSweepDistancesAndFlames[curr_sweep_meas_idx][0] = cwHeading;
  initialSweepDistancesAndFlames[curr_sweep_meas_idx][1] = distance;
  initialSweepDistancesAndFlames[curr_sweep_meas_idx][2] = flameVal;
  curr_sweep_meas_idx += 1;
}