#include "FlameSensor.h"
#include "Constants.h"

const int flameIgnoreValue = 400;

bool flamePresent(){
  int flameValue = analogRead(FLAME_PIN);
  Serial.println(flameValue);
  if(flameValue < flameIgnoreValue){
    return false;
  }else{
    return true;
  }
}

int getFlameValue() {
  int flameValue = analogRead(FLAME_PIN);
  return flameValue;
}
