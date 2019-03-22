#include "Movement.h"
#include "Globals.h"
#include "Constants.h"
#include "IMU.h"

#define MOVE_SPEED 130
#define LEFT_MOVE_SPEED 135
#define RIGHT_MOVE_SPEED 130
#define RIGHT_ROTATION_SPEED 75
#define LEFT_ROTATION_SPEED 66
// Left
int LEFT_MOVE_SPEEDS[] = {96, 100, 109, 117, 135, 151};
  // Right
int RIGHT_MOVE_SPEEDS[] = {94, 96, 107, 114, 130, 144};

#define FORWARD_ENCODER_DIST 4797
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

void moveForwardForDistance(float angle, int speed_idx, unsigned long encoder_counts) {
  leftEncoder.write(0);
  rightEncoder.write(0);
  digitalWrite(LEFT_MOTOR_DIR, 0);
  digitalWrite(RIGHT_MOTOR_DIR, 0);
  analogWrite(LEFT_MOTOR_SPEED, LEFT_MOVE_SPEEDS[speed_idx]);
  analogWrite(RIGHT_MOTOR_SPEED, RIGHT_MOVE_SPEEDS[speed_idx]);
  int avgEncoderVal = 0;
  while(avgEncoderVal < encoder_counts) {
    if (frontTilt > 100.0) { //TEST VALUE AND SET THIS AS A CONST
      inPit = true;
    }
    getIMUData();
    if (angle == 0) {
      if (cwHeading > 355) {
        float correction_factor = (360 - cwHeading);
        analogWrite(LEFT_MOTOR_SPEED, LEFT_MOVE_SPEEDS[speed_idx] + round(correction_factor));
        analogWrite(RIGHT_MOTOR_SPEED, RIGHT_MOVE_SPEEDS[speed_idx] - round(correction_factor)); 
      } else if (cwHeading < 5) {
        float correction_factor = cwHeading;
        analogWrite(LEFT_MOTOR_SPEED, LEFT_MOVE_SPEEDS[speed_idx] - round(correction_factor));
        analogWrite(RIGHT_MOTOR_SPEED, RIGHT_MOVE_SPEEDS[speed_idx] + round(correction_factor)); 
      }
    } else {
      float correction_factor = angle - cwHeading;
      analogWrite(LEFT_MOTOR_SPEED, LEFT_MOVE_SPEEDS[speed_idx] + round(correction_factor));
      analogWrite(RIGHT_MOTOR_SPEED, RIGHT_MOVE_SPEEDS[speed_idx] + round(correction_factor)); 
    }
    avgEncoderVal = (leftEncoder.read() + rightEncoder.read())/2;
    Serial.println(avgEncoderVal);
  }
}

void stopMotors() {
  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);
}

void moveForwardThenStop(float angle, int speed_idx, unsigned long encoder_counts) {
  moveForwardForDistance(angle, speed_idx, encoder_counts);
  stopMotors();
}

void moveForwardForever(float angle, int speed_idx) {
  // Max value for unsigned long...
  while (1) {
    moveForwardForDistance(angle, speed_idx, 4294967295);  
  }
}

void moveForwardTile() {
  moveForwardThenStop(0, 1, FORWARD_ENCODER_DIST);
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
  analogWrite(LEFT_MOTOR_SPEED, RIGHT_ROTATION_SPEED);
  analogWrite(RIGHT_MOTOR_SPEED, RIGHT_ROTATION_SPEED);
  getIMUData();
  if (angle == 0) {
    while (!(cwHeading >= 0 && cwHeading < 1)) { getIMUData(); }
  }
  else {
    if (cwHeading < angle) {
      getIMUData();
      while (cwHeading < angle - 1.5) { getIMUData(); }
    } else if (cwHeading > angle) {
      while (cwHeading < 360 && cwHeading > 10) { getIMUData(); }
      while (cwHeading < angle) { getIMUData(); }
    }
  }
  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);
  delay(100);
  getIMUData();
}

// MOVE THESE LATER ===================================================================================================================================
void updateDistance() {
  float distance = getMergedDistance();
  updateRollingMin(distance);
}

void updateFlame() {

}

void updateRollingMin(float distance) {
  Serial.print("update rolling min with distance: ");
  Serial.println(distance);
  initialSweepDistances[curr_sweep_meas_idx][0] = cwHeading;
  initialSweepDistances[curr_sweep_meas_idx][1] = distance;
  curr_sweep_meas_idx += 1;
}

// ====================================================================================================================================================

void rotateRightWhileSweeping(int angle) {
  if (angle < 0 || angle >= 360) {
    Serial.println("Angle is invalid");
    return;
  }
  digitalWrite(LEFT_MOTOR_DIR, 0);
  digitalWrite(RIGHT_MOTOR_DIR, 1); 
  getIMUData();
  analogWrite(LEFT_MOTOR_SPEED, RIGHT_ROTATION_SPEED);
  analogWrite(RIGHT_MOTOR_SPEED, RIGHT_ROTATION_SPEED);
  getIMUData();
  if (angle == 0) {
    while (!(cwHeading >= 0 && cwHeading < 1)) {
        getIMUData();
        // distance = getMergedDistance();
        // updateRollingMin(distance);
        updateDistance();
    }
  }
  else {
    if (cwHeading < angle) {
      getIMUData();
      while(cwHeading < angle - 1.5) { //DEFINITELY NEED TO TUNE THIS
        getIMUData();
        // distance = getMergedDistance();
        // updateRollingMin(distance);
        updateDistance();
      }
    } else if (cwHeading > angle) {
      Serial.println("cwHeading > angle");
      while (cwHeading < 360 && cwHeading > 10) {
        getIMUData();
        // distance = getMergedDistance();
        // updateRollingMin(distance);
        updateDistance();
      }
      while (cwHeading < angle) {
        getIMUData();
        // distance = getMergedDistance();
        // updateRollingMin(distance);
        updateDistance();
      }
    }    
  }
  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);
  delay(100);
  getIMUData();
  Serial.println();
  Serial.println("Printing out distance array");
  // return avg;
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
  
  analogWrite(LEFT_MOTOR_SPEED, LEFT_ROTATION_SPEED);
  analogWrite(RIGHT_MOTOR_SPEED, LEFT_ROTATION_SPEED);
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

// CALIBRATION FUNCTIONS FOR MOTORS... USE VALUES THAT ARE AT LEAST A BIT HIGHER THAT WHAT IS GIVEN
void calibrateRotateRight() {
  Serial.println("calibrateRotateRight");
  int rotation_speed = 66;
  int ok_counts = 0;
  while(ok_counts < 40) {
    leftEncoder.write(0);
    rightEncoder.write(0);
    Serial.print("Rotation speed: ");
    Serial.println(rotation_speed);
    digitalWrite(LEFT_MOTOR_DIR, 0);
    digitalWrite(RIGHT_MOTOR_DIR, 1); 
    analogWrite(LEFT_MOTOR_SPEED, rotation_speed);
    analogWrite(RIGHT_MOTOR_SPEED, rotation_speed);  
    delay(500);
    if (abs(leftEncoder.read()) < 10 || abs(rightEncoder.read()) < 10) {
      rotation_speed += 3;
      ok_counts = 0;
    } else {
      ok_counts += 1;
    }
  }
  Serial.println(rotation_speed);
  stopMotors();
  delay(5000);
}

void calibrateRotateLeft() {
  Serial.println("calibrateRotateLeft");
  int rotation_speed = 66;
  int ok_counts = 0;
  while(ok_counts < 40) {
    leftEncoder.write(0);
    rightEncoder.write(0);
    Serial.print("Rotation speed: ");
    Serial.println(rotation_speed);
    digitalWrite(LEFT_MOTOR_DIR, 1);
    digitalWrite(RIGHT_MOTOR_DIR, 0); 
    analogWrite(LEFT_MOTOR_SPEED, rotation_speed);
    analogWrite(RIGHT_MOTOR_SPEED, rotation_speed);  
    delay(500);
    if (abs(leftEncoder.read()) < 10 || abs(rightEncoder.read()) < 10) {
      rotation_speed += 3;
      ok_counts = 0;
    } else {
      ok_counts += 1;
    }
  }
  Serial.println(rotation_speed);
  stopMotors();
  delay(5000);
}

void calibrateEncoders() {
  // Left: 151, Right: 144
  // Left: 117, Right: 114
  // Left: 109, Right: 107
  // Left: 100, Right: 96
  // Left: 96, Right: 94
  int left_speeds[] = {80, 90, 100, 110, 120, 130};
  int right_speeds[] = {80, 90, 100, 110, 120, 130};
  int left_final[6];
  int right_final[6];
  for (int i = 0; i < 6; i++) {
    int left_speed = left_speeds[i];
    int right_speed = right_speeds[i];
    leftEncoder.write(0);
    rightEncoder.write(0);
    digitalWrite(LEFT_MOTOR_DIR, 0);
    digitalWrite(RIGHT_MOTOR_DIR, 0);
    analogWrite(LEFT_MOTOR_SPEED, left_speed);
    analogWrite(RIGHT_MOTOR_SPEED, right_speed);
    delay(500);
    while (1) {
      leftEncoder.write(0);
      rightEncoder.write(0);
      delay(300);
      int left = leftEncoder.read();
      int right = rightEncoder.read();
      Serial.print("Left Encoder: ");
      Serial.print(left);
      Serial.print(" Right Encoder: ");
      Serial.print(right);
      Serial.print(" Left Speed: ");
      Serial.print(left_speed);
      Serial.print(" Right Speed: ");
      Serial.println(right_speed);
      if (left > right) {
        right_speed += 1;
      } else if (right > left) {
        left_speed += 1;
      } else {
        Serial.println("Good enough");
        left_final[i] = left_speed;
        right_final[i] = right_speed;
        break;
      }
      digitalWrite(LEFT_MOTOR_DIR, 0);
      digitalWrite(RIGHT_MOTOR_DIR, 0);
      analogWrite(LEFT_MOTOR_SPEED, left_speed);
      analogWrite(RIGHT_MOTOR_SPEED, right_speed);  
    }
  }
  Serial.println("Left Final Speeds:");
  for (int i = 0; i < 6; i++) {
    Serial.print(left_final[i]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.println("Right Final Speeds:");
  for (int i = 0; i < 6; i++) {
    Serial.print(right_final[i]);
    Serial.print(" ");
  }
  delay(10000);
}