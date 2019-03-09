#include "Movement.h"
#include "Globals.h"
#include "Constants.h"
#include "IMU.h"

#define MOVE_SPEED 200
#define ROTATION_SPEED 150

//if we only want to move in up/down/left/right our relative angles will always be either 0, 90, 180, 270
//IMU does CW increasing 
//A6 - 9 for reg motors, A4,A5,A21,A22 for the motor encoders
// THIS WILL MOD THE vert and horz counts
void moveForward(int dist) {
  if (dist < 0) {
    digitalWrite(LEFT_MOTOR_DIR, 0);
    digitalWrite(RIGHT_MOTOR_DIR, 0);  
  } else if (dist > 0) {
    digitalWrite(LEFT_MOTOR_DIR, 1);
    digitalWrite(RIGHT_MOTOR_DIR, 1);
  }
  
  analogWrite(LEFT_MOTOR_SPEED, MOVE_SPEED);
  analogWrite(RIGHT_MOTOR_SPEED, MOVE_SPEED);

  delay(1000);
  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);
  delay(1000);
}

//We will only rotate with either 90 or 180 ... 360 isnt necessary and 270 will just use the other rotation
//The casting stuff for the rotations are wack ...

void rotateRight90() {
  digitalWrite(LEFT_MOTOR_DIR, 1);
  digitalWrite(RIGHT_MOTOR_DIR, 0); 
  
  analogWrite(LEFT_MOTOR_SPEED, ROTATION_SPEED);
  analogWrite(RIGHT_MOTOR_SPEED, ROTATION_SPEED);

  int target = (90 + (int) cwHeading)%360;
  Serial.println(target);
  Serial.println(cwHeading);
  while(cwHeading < target || cwHeading > target + 5) { //DEFINITELY NEED TO TUNE THIS
    getIMUData();
    Serial.println(cwHeading);
  }
  Serial.println("Good");
  //RUN A FINE TUNE FUNCTION TO GET EXACT HEADING
  
  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);
}

void rotateLeft90() {
  digitalWrite(LEFT_MOTOR_DIR, 0);
  digitalWrite(RIGHT_MOTOR_DIR, 1);
  
  analogWrite(LEFT_MOTOR_SPEED, ROTATION_SPEED);
  analogWrite(RIGHT_MOTOR_SPEED, ROTATION_SPEED);

  float target = (cwHeading - 90);
  if (target < 0) {
    target = target + 360;
  }
  
  while(cwHeading > target || cwHeading < target - 5) { //DEFINITELY NEED TO TUNE THIS
    getIMUData();
    Serial.println(cwHeading);
  }
  Serial.println("Good");
  //RUN A FINE TUNE FUNCTION TO GET EXACT HEADING
  
  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);  
}


void rotate180() {
  rotateRight90();
  rotateRight90();
}


//EVERYTIME WE DO WACK STUFF, LIKE COME OUT OF A DITCH, we want to do this?
void realignHeading() {

}
