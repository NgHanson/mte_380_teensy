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
  colorSetup();
  hc_sr04Setup();
  
  Serial.begin(115200);
}

void loop() {
  //code ignores interrupts, etc. will need to implement later
  
  colorRead(rgbArray);
  detectedFlame = flameDetected(flamePin, flameIgnore); //bool to tell if flame was detected
  if(detectedFlame){
    analogWrite(fanPin, 255); //add logic to look at timers and other stuff. not entirely sure as of yet.
  }
  lenHC_1 = ultraPulse(trigPin_HCSR04_1, echoPin_HCSR04_1);
  lenHC_2 = ultraPulse(trigPin_HCSR04_2, echoPin_HCSR04_2);
  lenPing = pingPulse(pingPin); //length reading from parallax Ping
  magnetDetected = digitalRead(hallPin); //slightly more complicated than this, but this should be fine
}

/*
 COLOR FUNCTIONS
*/

void colorSetup(){
  pinMode(colorS0, OUTPUT);
  pinMode(colorS1, OUTPUT);
  pinMode(colorS2, OUTPUT);
  pinMode(colorS3, OUTPUT);
  pinMode(colorOut, INPUT); //output of the sensor, we want to read this wave
  
  // Setting frequency-scaling to 20%. Setting to output to tell sensor what scaling we want
  digitalWrite(colorS0,HIGH);
  digitalWrite(colorS1,LOW);
}

void colorRead(int cData[]){
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
 
  Serial.print("Red= ");
  Serial.print(cData[0]);
  Serial.print("  ");
  Serial.print("G= ");
  Serial.print(cData[1]);
  Serial.print("  ");
  Serial.print("B= ");
  Serial.print(cData[2]);
  Serial.println("  ");
   /* 
  will need to do some comparison checking if freqColor is in some range to see if lego was found
  i.e: if( (freqRed > X || freqRed < Y) && etc etc) ) for a specific color.
  */
}

/*
FLMAE FUNCTIONS
*/

bool flameDetected(int pin, int valToIgnore){
  int flameValue = analogRead(pin);

  if(flameValue < valToIgnore){
    return false;
  }else{
    return true;
  }
  
}

/*
 * HC-SR04 FUNCTIONS
 */

 void hc_sr04Setup(){
  pinMode(trigPin_HCSR04_1, OUTPUT);
  pinMode(echoPin_HCSR04_1, INPUT);
  pinMode(trigPin_HCSR04_2, OUTPUT);
  pinMode(echoPin_HCSR04_2, INPUT);
}

long ultraPulse(const int triggerPin, const int echoPin){
  long duration, cm;
  
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin,LOW);
  
  duration = pulseIn(echoPin,HIGH);
  cm = duration * 0.017;
  return cm;
}

/*
 * PARALLAX PING FUNCTIONS
 */

long pingPulse(const int ultraPin){
  long duration, cm;
  
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(ultraPin, OUTPUT);
  digitalWrite(ultraPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultraPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(ultraPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(ultraPin, INPUT);
  duration = pulseIn(ultraPin, HIGH);
  cm = duration * 0.017;
  return cm;
}

void motorEncoder() {

}
