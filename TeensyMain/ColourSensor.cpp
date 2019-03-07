#include "ColourSensor.h"
#define colorS0 A12
#define colorS1 A13
#define colorS2 A14
#define colorS3 A15
#define colorOut A16

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
 
   /* 
  will need to do some comparison checking if freqColor is in some range to see if lego was found
  i.e: if( (freqRed > X || freqRed < Y) && etc etc) ) for a specific color.
  */
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
  Serial.println("  ");
}
