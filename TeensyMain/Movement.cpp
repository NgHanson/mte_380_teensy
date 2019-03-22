#include "Movement.h"
#include "Globals.h"
#include "Constants.h"
#include "IMU.h"
#include "DetectionHelper.h"
#define MOVE_SPEED 130
#define LEFT_MOVE_SPEED 135
#define RIGHT_MOVE_SPEED 130
#define RIGHT_ROTATION_SPEED 130//80//75
#define LEFT_ROTATION_SPEED 110//66
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
  Serial.print("mooveForwardForDistance - angle: ");
  Serial.print(angle);
  Serial.print(" speed: ");
  Serial.print(speed_idx);
  Serial.print(" encoder count: ");
  Serial.println(encoder_counts);
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
        float correction_factor = 5*(360 - cwHeading);
        Serial.print("grater tahn 355 - left: ");
        Serial.print(LEFT_MOVE_SPEEDS[speed_idx] + round(correction_factor));
        Serial.print(" right ");
        Serial.println(RIGHT_MOVE_SPEEDS[speed_idx] - round(correction_factor));
        analogWrite(LEFT_MOTOR_SPEED, LEFT_MOVE_SPEEDS[speed_idx] + round(correction_factor));
        analogWrite(RIGHT_MOTOR_SPEED, RIGHT_MOVE_SPEEDS[speed_idx] - round(correction_factor)); 
      } else if (cwHeading < 5) {
        float correction_factor = 5*cwHeading;
        Serial.print("here - left: ");
        Serial.print(LEFT_MOVE_SPEEDS[speed_idx] - round(correction_factor));
        Serial.print(" right ");
        Serial.println(RIGHT_MOVE_SPEEDS[speed_idx] + round(correction_factor));
        analogWrite(LEFT_MOTOR_SPEED, LEFT_MOVE_SPEEDS[speed_idx] - round(correction_factor));
        analogWrite(RIGHT_MOTOR_SPEED, RIGHT_MOVE_SPEEDS[speed_idx] + round(correction_factor)); 
      }
    } else {
      float correction_factor = 5*(angle - cwHeading);
      analogWrite(LEFT_MOTOR_SPEED, LEFT_MOVE_SPEEDS[speed_idx] + round(correction_factor));
      analogWrite(RIGHT_MOTOR_SPEED, RIGHT_MOVE_SPEEDS[speed_idx] + round(correction_factor)); 
    }
    avgEncoderVal = (leftEncoder.read() + rightEncoder.read())/2;
    Serial.println(avgEncoderVal);
  }
}

void moveBackwardForDistance(float angle, int speed_idx, unsigned long encoder_counts) {
  leftEncoder.write(0);
  rightEncoder.write(0);
  digitalWrite(LEFT_MOTOR_DIR, 1);
  digitalWrite(RIGHT_MOTOR_DIR, 1);
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
        float correction_factor = 5*(360 - cwHeading);
        Serial.print("grater tahn 355 - left: ");
        Serial.print(LEFT_MOVE_SPEEDS[speed_idx] - round(correction_factor));
        Serial.print(" right ");
        Serial.println(RIGHT_MOVE_SPEEDS[speed_idx] + round(correction_factor));
        analogWrite(LEFT_MOTOR_SPEED, LEFT_MOVE_SPEEDS[speed_idx] - round(correction_factor));
        analogWrite(RIGHT_MOTOR_SPEED, RIGHT_MOVE_SPEEDS[speed_idx] + round(correction_factor)); 
      } else if (cwHeading < 5) {
        float correction_factor = 5*cwHeading;
        Serial.print("here - left: ");
        Serial.print(LEFT_MOVE_SPEEDS[speed_idx] + round(correction_factor));
        Serial.print(" right ");
        Serial.println(RIGHT_MOVE_SPEEDS[speed_idx] - round(correction_factor));
        analogWrite(LEFT_MOTOR_SPEED, LEFT_MOVE_SPEEDS[speed_idx] + round(correction_factor));
        analogWrite(RIGHT_MOTOR_SPEED, RIGHT_MOVE_SPEEDS[speed_idx] - round(correction_factor)); 
      }
    } else {
      float correction_factor = 5*(angle - cwHeading);
      analogWrite(LEFT_MOTOR_SPEED, LEFT_MOVE_SPEEDS[speed_idx] - round(correction_factor));
      analogWrite(RIGHT_MOTOR_SPEED, RIGHT_MOVE_SPEEDS[speed_idx] - round(correction_factor)); 
    }
    avgEncoderVal = abs(leftEncoder.read() + rightEncoder.read())/2;
    Serial.println(avgEncoderVal);
  }
}

void stopMotors() {
  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);
  delay(300);
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

float addToleranceForEdge(float distance) {
  if (distance < 0.1) {
    return distance + 0.08;
  } else {
    return distance;
  }
}

void moveForwardInSand() {
  float tolerance = 0.06;
  Serial.println("move fwd in sand");
  if ((cwHeading > 350 || cwHeading < 10) && yPos > 0) {
    Serial.println("heading is 0");
    int current_furthest = 5;
    for (int i = yPos; i < 5; i++) {
      if (levelMap[i][xPos] == 'h' || levelMap[i][xPos] == 'c') {
        current_furthest = i;
        break;
      }
    }
    Serial.println("Needs to go: ");
    Serial.println(yPos);
    Serial.println(current_furthest);
    float dist = (current_furthest - yPos)*TILE_DIST_M + tolerance;
    dist = addToleranceForEdge(dist);
    Serial.println(dist);
    while (getMergedDistance() > dist) {
      moveForwardForDistance(0, 2, 1);
    }
    stopMotors();
  } else if ((cwHeading > 80 && cwHeading < 100) && xPos > 0) {
    int current_furthest = 5;
    for (int i = xPos; i < 5; i++) {
      if (levelMap[yPos][i] == 'h' || levelMap[yPos][i] == 'c') {
        current_furthest = i;
        break;
      }
    }
    float dist = (current_furthest - xPos)*TILE_DIST_M + tolerance;
    dist = addToleranceForEdge(dist);
    Serial.print("Needs to go: ");
    Serial.println(dist);
    while (getMergedDistance() > dist) {
      moveForwardForDistance(90, 2, 1);
    }
    stopMotors();
  } else if ((cwHeading > 170 && cwHeading < 190) && yPos < 5) {
    int current_furthest = 0;
    for (int i = yPos; i >= 0; i--) {
      if (levelMap[i][xPos] == 'h' || levelMap[i][xPos] == 'c') {
        current_furthest = i;
        break;
      }
    }
    float dist = (yPos - current_furthest)*TILE_DIST_M + tolerance;
    dist = addToleranceForEdge(dist);
    Serial.print("Needs to go: ");
    Serial.println(dist);
    while (getMergedDistance() > dist) {
      moveForwardForDistance(180, 2, 1);
    }
    stopMotors();
  } else if ((cwHeading > 260 && cwHeading < 280) && xPos < 5) {
    Serial.println("270");
    int current_furthest = 0;
    for (int i = xPos; i >= 0; i--) {
      if (levelMap[yPos][i] == 'h' || levelMap[yPos][i] == 'c') {
        current_furthest = i;
        break;
      }
    }
    Serial.print("xPos: ");
    Serial.println(xPos);

    float dist = (max(0, (xPos - current_furthest)))*TILE_DIST_M + tolerance;
    dist = addToleranceForEdge(dist);
    Serial.print("Needs to go: ");
    Serial.println(dist);
    while (getMergedDistance() > dist) {
      moveForwardForDistance(270, 2, 1);
    }
    stopMotors();
  } else {
    Serial.print("cwHeading = ");
    Serial.print(cwHeading);
    Serial.print(" no clue where were looking...");
    delay(10000);
  }
}
void signalComplete(){
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}

int getIdealHeading() {
  if ((cwHeading > 350 || cwHeading < 10)) {
    return 0;
  } else if ((cwHeading > 80 && cwHeading < 100)) {
    return 90;
  } else if ((cwHeading > 170 && cwHeading < 190)) {
    return 180;
  } else if ((cwHeading > 260 && cwHeading < 280)) {
    return 270;
  }  
}
void dipIntoMagnet() {
  moveForwardForDistance(getIdealHeading(), 4, FORWARD_ENCODER_DIST * 0.75);
  Serial.print("moving backwars fuc");
  delay(100);
  if (didDetectMagnet()) {
    Serial.println("DID DETECT MAGNET");
      magnetDetected = true;
      signalComplete();
      delay(1000);
  }
  moveBackwardForDistance(getIdealHeading(), 5, FORWARD_ENCODER_DIST);
  stopMotors();
  delay(1000);
  moveForwardInSand();
}
void moveForwardTile() {
  if (levelMap[yPos][xPos] == 'm') {
    Serial.println("IN SAND - THIS SHOULD NEVER BE HAPPENING");
    moveForwardInSand();
  } else {
    moveForwardThenStop(0, 4, FORWARD_ENCODER_DIST);
  }
  

  //Update global x,y
  char currDir = convertHeadingToCharDirection(cwHeading);
  if (currDir == 'u') {
    yPos++;
  } else if (currDir == 'l') {
    xPos--;
  } else if (currDir == 'r') {
    xPos++;
  } else if (currDir == 'd') {
    yPos--;
  }
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

void rotateRightWhileSweeping(int angle) {
  Serial.println("rotateRightWhileSweeping ");
  Serial.println(cwHeading);
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
  Serial.println("here");
  if (angle == 0) {
    while (!(cwHeading >= 0 && cwHeading < 1)) {
        getIMUData();
        // distance = getMergedDistance();
        // updateRollingMin(distance);
        updateHeadingVals();
    }
  }
  else {
    if (cwHeading < angle) {
      Serial.print("cwHeading < angle");
      Serial.println(cwHeading);
      getIMUData();
      while(cwHeading < angle - 1.5) { //DEFINITELY NEED TO TUNE THIS
        getIMUData();
        // distance = getMergedDistance();
        // updateRollingMin(distance);
        updateHeadingVals();
      }
      Serial.print("Done Rotating here");
      Serial.println(cwHeading);
    } else if (cwHeading > angle) {
      Serial.println("cwHeading > angle");
      while (cwHeading < 360 && cwHeading > 10) {
        getIMUData();
        // distance = getMergedDistance();
        // updateRollingMin(distance);
        updateHeadingVals();
      }
      while (cwHeading < angle) {
        getIMUData();
        // distance = getMergedDistance();
        // updateRollingMin(distance);
        updateHeadingVals();
      }
    }    
  }
  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);
  delay(100);
  Serial.println("Done");
  getIMUData();
  // Serial.println();
  // Serial.println("Printing out distance array");
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

char convertHeadingToCharDirection(float heading) {
  //PROBABLY SHOULDNT ROUND TO NEARIST 90
  if ((345.0 <= heading && heading < 360.0) || (heading >= 0.0 && heading <= 15.0)) {
    return 'u';
  } else if (heading >= 75.0 && heading <= 105.0) {
    return 'r';
  } else if (heading >= 165.0 && heading <= 195.0) {
    return 'd';
  } else if (heading >= 255.0 && heading <= 285.0) {
    return 'l';
  } else { // CURRENTLY HEADING IS COMPLETELY OFF FROM a 90 deg
    Serial.print("WRONG");
    return '-'; //SHOULD PROB THROW ERR OR PERFORM RE ALIGN 
  }
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

bool shouldTurnLeft(float startAngle, float finalAngle) {
  float left = startAngle - finalAngle;
  if (left < 0.0) {
    left += 360.0;
  }

  float right = finalAngle - startAngle;
  if (right < 0.0) {
    right += 360.0;
  }

  return left < right;
}
