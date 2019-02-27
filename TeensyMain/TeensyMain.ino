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

//read values
int rgbArray[3]; //red,green,blue
int flameIgnore; //flame values to ignore if below
bool detectedFlame, magnetDetected;
long lenHC_1, lenHC_2, lenPing;

void setup() {
  colourSetup();
  hc_sr04Setup();

  Serial.begin(115200);
}

void loop() {
  //code ignores interrupts, etc. will need to implement later
  
  colourRead(rgbArray);
  detectedFlame = flameDetected(flamePin, flameIgnore); //bool to tell if flame was detected
  if(detectedFlame){
    analogWrite(fanPin, 255); //add logic to look at timers and other stuff. not entirely sure as of yet.
  }
  lenHC_1 = ultraPulse(trigPin_HCSR04_1, echoPin_HCSR04_1);
  lenHC_2 = ultraPulse(trigPin_HCSR04_2, echoPin_HCSR04_2);
  lenPing = pingPulse(pingPin); //length reading from parallax Ping
  magnetDetected = digitalRead(hallPin); //slightly more complicated than this, but this should be fine
}
