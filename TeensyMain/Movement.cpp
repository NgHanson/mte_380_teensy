#include "Movement.h"
#include "Globals.h"
#include "Constants.h"
#include "IMU.h"

#define MOVE_SPEED 130
#define LEFT_MOVE_SPEED 135
#define RIGHT_MOVE_SPEED 130
#define ROTATION_SPEED 115

#define FORWARD_ENCODER_DIST 5100
#define FORWARD_ENCODER_DIST_PIT 1000

//if we only want to move in up/down/left/right our relative angles will always be either 0, 90, 180, 270
//IMU does CW increasing 
//A6 - 9 for reg motors, A4,A5,A21,A22 for the motor encoders
// THIS WILL MOD THE vert and horz counts
//DEPRECATE THIS
void moveForward(int dist) {
  if (dist < 0) {
    digitalWrite(LEFT_MOTOR_DIR, 1);
    digitalWrite(RIGHT_MOTOR_DIR, 1);  
  } else if (dist > 0) {
    digitalWrite(LEFT_MOTOR_DIR, 0);
    digitalWrite(RIGHT_MOTOR_DIR, 0);
  }
  
  analogWrite(LEFT_MOTOR_SPEED, LEFT_MOVE_SPEED);
  analogWrite(RIGHT_MOTOR_SPEED, RIGHT_MOVE_SPEED);

  delay(1000); //Change these to motor encoder counts
  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);
  delay(1000);
}

void moveForwardTile() {
  Serial.println("here");
  leftEncoder.write(0);
  rightEncoder.write(0);
  Serial.println("done writing encoders");
  digitalWrite(LEFT_MOTOR_DIR, 0);
  digitalWrite(RIGHT_MOTOR_DIR, 0);
  Serial.println("wrote/ firection");
  analogWrite(LEFT_MOTOR_SPEED, MOVE_SPEED);
  analogWrite(RIGHT_MOTOR_SPEED, MOVE_SPEED);
  Serial.println("wrote speed");
  int encoder_dist = FORWARD_ENCODER_DIST;

  int avgEncoderVal = 0;
  while(avgEncoderVal < encoder_dist) {
    if (frontTilt > 100.0) { //TEST VALUE AND SET THIS AS A CONST
      inPit = true;
    }
    avgEncoderVal = (leftEncoder.read() + rightEncoder.read())/2;
    Serial.print("leftEncoder: ");
    Serial.print(leftEncoder.read());
    Serial.print(" rightEncoder: ");
    Serial.println(rightEncoder.read());
    // delay(200);
    // Serial.println(avgEncoderVal);
  }


  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);

  //REALIGN TO ITS CURRENT ORIENTATION (0, 90, 180, 270)
}

//We will only rotate with either 90 or 180 ... 360 isnt necessary and 270 will just use the other rotation
//The casting stuff for the rotations are wack ...

// Takes in number from 0-360 with +ve cw
void rotateRight(int angle) {
  if (angle < 0 || angle >= 360) {
    Serial.println("Angle is invalid");
    return;
  }
  digitalWrite(LEFT_MOTOR_DIR, 0);
  digitalWrite(RIGHT_MOTOR_DIR, 1); 
  getIMUData();
  analogWrite(LEFT_MOTOR_SPEED, ROTATION_SPEED);
  analogWrite(RIGHT_MOTOR_SPEED, ROTATION_SPEED);
  getIMUData();
  // Serial.println(angle);
  // Serial.println(cwHeading);
  if (angle == 0) {
    // angle = 357.5;
    while (!(cwHeading >= 0 && cwHeading < 1)) {
      getIMUData();
      // Serial.println(cwHeading);      
    }
  }
  else {
    Serial.println("rotateright else");
    // angle = angle - 2.5;
    if (cwHeading < angle) {
      Serial.println("cwHeading < angle");
      getIMUData();
      while(cwHeading < angle - 1.5) { //DEFINITELY NEED TO TUNE THIS
        getIMUData();
        Serial.print("cwHeading: ");
        Serial.print(cwHeading);
        Serial.print(" angle: ");
        Serial.println(angle);
      }
    } else if (cwHeading > angle) {
      Serial.println("cwHeading > angle");
      while ((cwHeading - 360) < angle) {
        getIMUData();
        Serial.println(cwHeading);
      }
    }    
  }
  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);
  delay(100);
  getIMUData();
}
// Takes in number from 0-360, with +ve cw (turning left 90 deg from 0 would be rotateLeft(270))
void rotateLeft(int angle) {
  Serial.println("rotateLeft");
  if (angle < 0 || angle >= 360) {
    Serial.println("Angle is invalid");
    return;
  }
  digitalWrite(LEFT_MOTOR_DIR, 1);
  digitalWrite(RIGHT_MOTOR_DIR, 0);
  
  analogWrite(LEFT_MOTOR_SPEED, ROTATION_SPEED);
  analogWrite(RIGHT_MOTOR_SPEED, ROTATION_SPEED);
  if (angle == 0) {
    while (!(cwHeading > 1 && cwHeading < 3)) {
      getIMUData();
      Serial.print("Heading ");
      Serial.println(cwHeading);
    }
  } else {
    angle = angle + 2.5;
    // Serial.print("cwHeading");
    if (cwHeading < angle) {
      Serial.println("cwHeading < angle");
      while (!(cwHeading < angle && cwHeading > angle - 2.5)) {
        getIMUData();
        Serial.println(cwHeading);
      }
    } else if (cwHeading > angle) {
      Serial.println("cwHeading > angle");
      while (cwHeading > angle) {
        getIMUData();
        Serial.println(cwHeading);
      }
    }
  }
  // float target = (cwHeading - angle);
  // if (target < 0) {
  //   target = target + 360;
  // }
  
  // while(cwHeading > target || cwHeading < target - 3) { //DEFINITELY NEED TO TUNE THIS
  //   getIMUData();
  //   Serial.println(cwHeading);
  // }
  // Serial.println("Good");
  //RUN A FINE TUNE FUNCTION TO GET EXACT HEADING
  
  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);  
  delay(100);
  getIMUData();
}


void rotate180() {
  rotateRight(180);
}


//EVERYTIME WE DO WACK STUFF, LIKE COME OUT OF A DITCH, we want to do this?
void realignHeading() {

}

void alignToAngle(int angle) {

}
