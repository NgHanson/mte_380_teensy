#include "FlameSensor.h"
#include "Constants.h"

const int flameIgnoreValue = 70;

bool flamePresent(){
  int flameValue = analogRead(FLAME_PIN);
  //Serial.println(flameValue);
  if(flameValue < flameIgnoreValue){
    return false;
  }else{
    return true;
  }
}
