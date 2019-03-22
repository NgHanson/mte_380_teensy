#define RIGHT_ENC_1 A3
#define RIGHT_ENC_2 A2
#define LEFT_ENC_2 A5
#define LEFT_ENC_1 A4

#define RIGHT_MOTOR_DIR A6
#define RIGHT_MOTOR_SPEED A7
#define LEFT_MOTOR_DIR A8
#define LEFT_MOTOR_SPEED A9

#define colorS1 A12
#define colorS0 A13
#define colorS3 A14
#define colorS2 A15
#define colorOut A16

#define fanPin A17
#define FLAME_PIN A0
#define IR_SENSOR A1

#define LED_PIN LED_BUILTIN
// No longer using cheap ultrasonic...
// #define trigPin_HCSR04_1 A19
// #define echoPin_HCSR04_1 A20
// #define trigPin_HCSR04_2 A0
// #define echoPin_HCSR04_2 A1

// No longer using ultrasonic...
#define pingPin A20

// No longer using hall effect...
// #define hallPin A3

#define TILE_DIST_M 0.3048 //m

// Math function
#define SENSOR_NUM_SAMPLES 5
#define TILT_TOLERANCE 15
#define DEG_PER_RAD 57.2957795

// TODO: Confirm this max size ... may lead to missing vals if not large enough
#define MAX_PATH_FINDING_SIZE 40

#define LED_R 24
#define LED_G 12
#define LED_B 11