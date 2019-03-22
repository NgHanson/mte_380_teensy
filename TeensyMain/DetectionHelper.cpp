#include "DetectionHelper.h"
#include "ColourSensor.h"
#include "Constants.h"
#include "IRSensor.h"

void detectTileInFront() {
  int cData[3];
  colourRead(cData);
  float r_val = cData[0];
  float g_val = cData[1];
  float b_val = cData[2];
  float colMagAvg = sqrt(pow(r_val, 2) + pow(g_val, 2) + pow(b_val, 2));
  printColourDistribution(r_val, g_val, b_val);
  float distance = getIRDistance();
  //Serial.print(" IR Distance: ");
  //Serial.println(distance);
  // TODO: Implement this logic...  
  bool in_front_of_obj = true; //ignoring algorithm
  
  if (in_front_of_obj) {
    if (r_val/colMagAvg > 0.29 && r_val/colMagAvg < 0.31 &&
        g_val/colMagAvg > 0.57 && g_val/colMagAvg < 0.64 &&
        b_val/colMagAvg > 0.70 && b_val/colMagAvg < 0.75) {
        //Serial.println("Yellow House");
        digitalWrite(LED_PIN,HIGH);
    } else {
      //Serial.println("Red House");
      int blinking = 1;
      for(int i = 0; i < 10; i++){
        blinking ^= 1;
        digitalWrite(LED_PIN, blinking);
        delay(250);
      }
    }
  } else {
    if (distance > 14) {
      //Serial.println("Hole");
    } else if (distance < 13) {
      //Serial.println("Wood Tile");
    } else {
      if (r_val/colMagAvg > 0.28 && r_val/colMagAvg < 0.4 &&
      g_val/colMagAvg > 0.64 && g_val/colMagAvg < 0.69 &&
      b_val/colMagAvg > 0.64 && b_val/colMagAvg < 0.69 && colMagAvg > 400) {
        //Serial.println("Sand");
      } else {
        //Serial.println("Rocks");
      }
    }
  }
}
