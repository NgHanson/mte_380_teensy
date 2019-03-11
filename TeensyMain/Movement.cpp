#include "Movement.h"
#include "Globals.h"
#include "Constants.h"
#include "IMU.h"

#define MOVE_SPEED 200
#define ROTATION_SPEED 80

#define FORWARD_ENCODER_DIST 5000
#define FORWARD_ENCODER_DIST_PIT 1000

//if we only want to move in up/down/left/right our relative angles will always be either 0, 90, 180, 270
//IMU does CW increasing 
//A6 - 9 for reg motors, A4,A5,A21,A22 for the motor encoders
// THIS WILL MOD THE vert and horz counts
//DEPRECATE THIS
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

  delay(1000); //Change these to motor encoder counts
  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);
  delay(1000);
}

void moveForwardTile() {
  leftEncoder.write(0);
  rightEncoder.write(0);
  
  digitalWrite(LEFT_MOTOR_DIR, 1);
  digitalWrite(RIGHT_MOTOR_DIR, 1);

  analogWrite(LEFT_MOTOR_SPEED, MOVE_SPEED);
  analogWrite(RIGHT_MOTOR_SPEED, MOVE_SPEED);

  int encoder_dist = FORWARD_ENCODER_DIST;
  if (inPit) { //MIGHT NEED TO START DIFFERENTIATING MOVEMENT BETWEEN SAND, PITS, and ROCKS
    encoder_dist = FORWARD_ENCODER_DIST_PIT;
    //GET OUT OF PIT FIRST
    while(inPit){
      
    }
    
  }

  int avgEncoderVal = 0;
  while(avgEncoderVal < encoder_dist) {
    if (frontTilt > 100.0) { //TEST VALUE AND SET THIS AS A CONST
      inPit = true;
    }
    avgEncoderVal = (leftEncoder.read() + rightEncoder.read())/2;
  }


  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);

  //REALIGN TO ITS CURRENT ORIENTATION (0, 90, 180, 270)
}

//We will only rotate with either 90 or 180 ... 360 isnt necessary and 270 will just use the other rotation
//The casting stuff for the rotations are wack ...

void rotateRight(int angle) {
  if (angle < 0 || angle > 360) {
    Serial.println("Angle is invalid");
    return;
  }
  digitalWrite(LEFT_MOTOR_DIR, 1);
  digitalWrite(RIGHT_MOTOR_DIR, 0); 
  
  analogWrite(LEFT_MOTOR_SPEED, ROTATION_SPEED);
  analogWrite(RIGHT_MOTOR_SPEED, ROTATION_SPEED);
  // int target = (angle + (int) cwHeading)%360;
  //cwHeading = (int) cwHeading;
  Serial.println(angle);
  Serial.println(cwHeading);
  if (angle == 0) {
    // angle = 357.5;
    while (!(cwHeading >= 0 && cwHeading < 1)) {
      getIMUData();
      Serial.println(cwHeading);      
    }
  }
  else {
    angle = angle - 2.5;
    if (cwHeading < angle) {

      while(cwHeading < angle) { //DEFINITELY NEED TO TUNE THIS
        getIMUData();
        Serial.println(cwHeading);
      }
    } else if (cwHeading > angle) {
      bool started = false;
      while ((cwHeading - 360) < angle) {
        getIMUData();
        Serial.println(cwHeading);
      }
    }    
  }

  // while(cwHeading < angle || (cwHeading-360) < angle) { //DEFINITELY NEED TO TUNE THIS
  //   getIMUData();
  //   Serial.println(cwHeading);
  // }
  Serial.println("Good");
  //RUN A FINE TUNE FUNCTION TO GET EXACT HEADING
  
  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);
}

void rotateLeft(int angle) {
  digitalWrite(LEFT_MOTOR_DIR, 0);
  digitalWrite(RIGHT_MOTOR_DIR, 1);
  
  analogWrite(LEFT_MOTOR_SPEED, ROTATION_SPEED);
  analogWrite(RIGHT_MOTOR_SPEED, ROTATION_SPEED);

  float target = (cwHeading - angle);
  if (target < 0) {
    target = target + 360;
  }
  
  while(cwHeading > target || cwHeading < target - 3) { //DEFINITELY NEED TO TUNE THIS
    getIMUData();
    Serial.println(cwHeading);
  }
  Serial.println("Good");
  //RUN A FINE TUNE FUNCTION TO GET EXACT HEADING
  
  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);  
}


void rotate180() {
  rotateRight(180);
}


//EVERYTIME WE DO WACK STUFF, LIKE COME OUT OF A DITCH, we want to do this?
void realignHeading() {

}

void alignToAngle(int angle) {

}
