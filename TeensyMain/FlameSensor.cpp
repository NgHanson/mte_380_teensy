#include "FlameSensor.h"
const int flameIgnoreValue = 200;

bool flameDetected(const int pin){
  int flameValue = analogRead(pin);
  Serial.println(flameValue);
  if(flameValue < flameIgnoreValue){
    return false;
  }else{
    return true;
  }
}
