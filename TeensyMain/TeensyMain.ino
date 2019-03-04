/*
A6 - 9 for reg motors
IMU code and Motor Encoders/Motor Control not included. Many algorithms neccesary ignored.
Have not included Hall Effect. Wanna Test a bit more
*/

//pins
const int colorS0 = A12;
const int colorS1 = A13;
const int colorS2 = A14;
const int colorS3 = A15;
const int colorOut = A16;

const int flamePin = A17;
const int fanPin = A19;

const int trigPin_HCSR04_1 = A20;
const int echoPin_HCSR04_1 = A21;
const int trigPin_HCSR04_2 = A0;
const int echoPin_HCSR04_2 = A1;
const int pingPin = A2;

const int hallPin = A3;

//Timer
IntervalTimer flameTimer;
IntervalTimer ultrasonicTimer;
IntervalTimer hallTimer;
//IntervalTimer motorEncoder1Timer;
//IntervalTimer motorEncoder2Timer;

//read values
int rgbArray[3]; //red,green,blue
bool detectedFlame, magnetDetected;
long lenHC_1, lenHC_2, lenPing;

void setup() {
  colourSetup();
  //hcUltrasonicSetup();

  Serial.begin(115200);

  timerSetup();
}


//Lower value, higher priority
void timerSetup() {
  flameTimer.begin(detectFlame, 125);
  flameTimer.priority(3);
  
  ultrasonicTimer.begin(ultrasonicPulse, 125);
  ultrasonicTimer.priority(3);
  
  hallTimer.begin(detectMagnet, 125);
  hallTimer.priority(3);
}

void loop() {
  /*
  colourRead(rgbArray);
  
  if(detectedFlame){
    analogWrite(fanPin, 255); //add logic to look at timers and other stuff. not entirely sure as of yet.
  }*/
  constructionCheckLoop();
}

void constructionCheckLoop(){

  moveForward(100);
  moveForward(-100);

  while(!detectedFlame) {
  }

  rotateCW(90);
  rotateCCW(90);

  while(lenPing > 50) {
  }

  rotateCW(90);
  rotateCCW(90);
  
}

void detectFlame() {
  detectedFlame = flameDetected(flamePin); //bool to tell if flame was detected
}

void ultrasonicPulse() {
  //lenHC_1 = hcPulse(trigPin_HCSR04_1, echoPin_HCSR04_1);
  //lenHC_2 = hcPulse(trigPin_HCSR04_2, echoPin_HCSR04_2);
  lenPing = parallaxPulse(pingPin); //length reading from parallax Ping
}

void detectMagnet() {
  magnetDetected = digitalRead(hallPin); //slightly more complicated than this, but this should be fine
}
