const int flameIgnoreValue = 0; //TODO: Kelvin to find this value

bool flameDetected(const int pin){
  int flameValue = analogRead(pin);

  if(flameValue < flameIgnoreValue){
    return false;
  }else{
    return true;
  }
}
