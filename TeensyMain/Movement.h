#include <Arduino.h>
void moveForward(int);
void rotateRight(int);
void rotateLeft(int);
void moveForwardTile(void);
void stopMotors(void);
void moveForwardForever(float, int);
void moveForwardForDistance(float, int, unsigned long);
char convertHeadingToCharDirection(float heading);
void calibrateRotateRight(void);
void calibrateRotateLeft(void);
bool shouldTurnLeft(float startAngle, float finalAngle);
void rotateRightWhileSweeping(int);