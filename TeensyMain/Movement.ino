#define LEFT_MOTOR_SPEED A6
#define RIGHT_MOTOR_SPEED A7
#define LEFT_MOTOR_DIR A8
#define RIGHT_MOTOR_DIR A9
#define LEFT_ENC_1 A4
#define LEFT_ENC_2 A5
#define RIGHT_ENC_1 A21
#define RIGHT_ENC_2 A22

//A6 - 9 for reg motors, A4,A5,A21,A22 for the motor encoders
void moveForward(int dist) {
  analogWrite(LEFT_MOTOR_SPEED, 255);
  analogWrite(RIGHT_MOTOR_SPEED, 255);
  if (dist < 0) {
    digitalWrite(LEFT_MOTOR_DIR, 0);
    digitalWrite(RIGHT_MOTOR_DIR, 0);  
  } else if (dist > 0) {
    digitalWrite(LEFT_MOTOR_DIR, 1);
    digitalWrite(RIGHT_MOTOR_DIR, 1);
  }
  delay(1000);
  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);
  delay(1000);
  //do nothing for dist of 0
}

void rotateCW (int angle) {
  analogWrite(LEFT_MOTOR_SPEED, 255);
  analogWrite(RIGHT_MOTOR_SPEED, 255);
  digitalWrite(LEFT_MOTOR_DIR, 1);
  digitalWrite(RIGHT_MOTOR_DIR, 0); 
  delay(1000);
  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);
}

void rotateCCW(int angle) {
  analogWrite(LEFT_MOTOR_SPEED, 255);
  analogWrite(RIGHT_MOTOR_SPEED, 255);
  digitalWrite(LEFT_MOTOR_DIR, 0);
  digitalWrite(RIGHT_MOTOR_DIR, 1);
  delay(1000); 
  analogWrite(LEFT_MOTOR_SPEED, 0);
  analogWrite(RIGHT_MOTOR_SPEED, 0);  
}
