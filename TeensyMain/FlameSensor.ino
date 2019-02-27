bool flameDetected(int pin, int valToIgnore){
  int flameValue = analogRead(pin);

  if(flameValue < valToIgnore){
    return false;
  }else{
    return true;
  }
  
}
