#include "ColourSensor.h"
#include "Constants.h"
#include "IRSensor.h"

void colourSetup() {
  pinMode(colorS0, OUTPUT);
  pinMode(colorS1, OUTPUT);
  pinMode(colorS2, OUTPUT);
  pinMode(colorS3, OUTPUT);
  pinMode(colorOut, INPUT); //output of the sensor, we want to read this wave
  
  // Setting frequency-scaling to 20%. Setting to output to tell sensor what scaling we want
  digitalWrite(colorS0,HIGH);
  digitalWrite(colorS1,LOW);

}

void printColourValues(int cData[]) {
  Serial.print("Red= ");
  Serial.print(cData[0]);
  Serial.print("  ");
  Serial.print("G= ");
  Serial.print(cData[1]);
  Serial.print("  ");
  Serial.print("B= ");
  Serial.print(cData[2]);
  Serial.print("  ");
  float r_val = cData[0];
  float g_val = cData[1];
  float b_val = cData[2];
  float colMagAvg = sqrt(pow(r_val, 2) + pow(g_val, 2) + pow(b_val, 2));
  Serial.print("R/Mag: ");
  Serial.print(r_val/colMagAvg);
  Serial.print(" G/Mag: ");
  Serial.print(g_val/colMagAvg);
  Serial.print(" B/Mag: ");
  Serial.print(b_val/colMagAvg);
  Serial.print(" Mag: ");
  Serial.print(colMagAvg);
  float distance = getIRDistance();
  Serial.print(" IR Distance: ");
  Serial.println(distance);
  // TODO: Implement this logic...  
  bool in_front_of_obj = false;
  
  if (in_front_of_obj) {
    if (r_val/colMagAvg > 0.29 && r_val/colMagAvg < 0.31 &&
        g_val/colMagAvg > 0.57 && g_val/colMagAvg < 0.64 &&
        b_val/colMagAvg > 0.70 && b_val/colMagAvg < 0.75) {// &&
        // (colMagAvg > 299.0 && colMagAvg < 308.0 ||
        // colMagAvg > 280.0 && colMagAvg < 288.0)) {
        Serial.println("Yellow House");
    } else {
      Serial.println("Red House");
    }
  } else {
      if (distance > 14) {
        Serial.println("Hole");
      } else if (distance < 13) {
        Serial.println("Wood Tile");
      } else {
        if (r_val/colMagAvg > 0.28 && r_val/colMagAvg < 0.4 &&
        g_val/colMagAvg > 0.64 && g_val/colMagAvg < 0.69 &&
        b_val/colMagAvg > 0.64 && b_val/colMagAvg < 0.69 && colMagAvg > 400) {
          Serial.println("Sand");
        } else {
          Serial.println("Rocks");
        }
      }
     // else if (r_val/colMagAvg > 0.25 && r_val/colMagAvg < 0.33 &&
     //    g_val/colMagAvg > 0.605 && g_val/colMagAvg < 0.68 &&
     //    b_val/colMagAvg > 0.70 && b_val/colMagAvg < 0.74) {// &&
     //    // colMagAvg > 259.0 && colMagAvg < 265.0) {
     //    Serial.println("Wood Tile");
     //  } else if (r_val/colMagAvg > 0.25 && r_val/colMagAvg < 0.35 &&
     //    g_val/colMagAvg > 0.635 && g_val/colMagAvg < 0.67 &&
     //    b_val/colMagAvg > 0.685 && b_val/colMagAvg < 0.72){
     //    Serial.println("Hole");
     //  } else if (r_val/colMagAvg > 0.25 && r_val/colMagAvg < 0.35 &&
     //    g_val/colMagAvg > 0.635 && g_val/colMagAvg < 0.665 &&
     //    b_val/colMagAvg > 0.69 && b_val/colMagAvg < 0.72) {
     //    Serial.println("Rocks");
     //  } else {
     //    Serial.println("Other");
     //  }
  }
  // else if (r_val/colMagAvg > 0.25 && r_val/colMagAvg < 0.30 &&
  //   g_val/colMagAvg > 0.64 && g_val/colMagAvg < 0.66 &&
  //   b_val/colMagAvg > 0.65 && b_val/colMagAvg < 0.71){// &&
  //   // (colMagAvg > 300.0 && colMagAvg < 307.0 ||
  //   // colMagAvg > 252.0 && colMagAvg < 255.0)) {
  //   Serial.println("Red House");
  // } else {
  //   Serial.println("Other");
  // }
}

void colourRead(int cData[]) {
    // Setting red filtered photodiodes to be read
  digitalWrite(colorS2,LOW);
  digitalWrite(colorS3,LOW);
  // Reading the output frequency
  cData[0] = pulseIn(colorOut, LOW); //red
 
  // Setting Green filtered photodiodes to be read
  digitalWrite(colorS2,HIGH);
  digitalWrite(colorS3,HIGH);
  cData[1] = pulseIn(colorOut, LOW); //green
   
  // Setting Blue filtered photodiodes to be read
  digitalWrite(colorS2,LOW);
  digitalWrite(colorS3,HIGH);
  cData[2] = pulseIn(colorOut, LOW); //blue
  printColourValues(cData);
   /* 
  will need to do some comparison checking if freqColor is in some range to see if lego was found
  i.e: if( (freqRed > X || freqRed < Y) && etc etc) ) for a specific color.
  */
}


