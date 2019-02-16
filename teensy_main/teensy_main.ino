#include <Ping.h>

//Pins
const int ultrasonicPin = 14;
const int flamePin = 16;
const int hallPin = 18;

const int colourS0Pin = 20;
const int colourS1Pin = 21;
const int colourS2Pin = 22;
const int colourS3Pin = 23;
const int colourOUTPin = 24;

Ping ultrasonic = Ping(ultrasonicPin);  // Ultrasonic signal on pin 13

//Read values
int flameValue = 0;
int hallState = 0;
int colourFreq = 0;

void setup() {
  Serial.begin(115200);

  //---Colour Sensor Setup---//
  pinMode(colourS0Pin, OUTPUT);
  pinMode(colourS1Pin, OUTPUT);
  pinMode(colourS2Pin, OUTPUT);
  pinMode(colourS3Pin, OUTPUT);
  pinMode(colourOUTPin, INPUT);
  // frequency Scaling 100%
  digitalWrite(colourS0Pin, HIGH);
  digitalWrite(colourS1Pin, HIGH);
}

void loop() {
  Serial.println();
  Serial.print("New Set of Readings");
  Serial.println();

  ultrasonic.fire();
  Serial.print("Microseconds: ");
  Serial.print(ultrasonic.microseconds());
  Serial.print(" | Centimeters: ");
  Serial.print(ultrasonic.centimeters());
  Serial.println();

  flameValue = analogRead(flamePin);
  Serial.print("Flame Sensor Value: ");
  Serial.print(flameValue);
  Serial.println();

  hallState = digitalRead(hallPin);
  Serial.print("Hall Effect Val: ");
  Serial.print(hallState);
  Serial.println();

  detectColour();
  delay(500);
}

void motorEncoder() {

}

void detectColour() {
  // Setting red filtered photodiodes to be read
  digitalWrite(colourS2Pin, LOW);
  digitalWrite(colourS3Pin, LOW);
  colourFreq = pulseIn(colourOUTPin, LOW);

  Serial.print("R= ");
  Serial.print(colourFreq);
  Serial.print("  ");
  delay(50);

  // Setting Green filtered photodiodes to be read
  digitalWrite(colourS2Pin, HIGH);
  digitalWrite(colourS3Pin, HIGH);
  colourFreq = pulseIn(colourOUTPin, LOW);

  Serial.print("G= ");
  Serial.print(colourFreq);
  Serial.print("  ");
  delay(50);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(colourS2Pin, LOW);
  digitalWrite(colourS3Pin, HIGH);
  colourFreq = pulseIn(colourOUTPin, LOW);

  Serial.print("B= ");
  Serial.print(colourFreq);
  Serial.println("  ");
  delay(50);
}
