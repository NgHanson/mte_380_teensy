#include "FlameSensor.h"
const int flameIgnoreValue = 70;

bool flamePresent(const int pin){
  int flameValue = analogRead(pin);
  Serial.println(flameValue);
  if(flameValue < flameIgnoreValue){
    return false;
  }else{
    return true;
  }
}

// int flameValue(const int pin) {
// 	// TODO: Might need input filtering but probably not
// 	return analogRead(pin);
// }