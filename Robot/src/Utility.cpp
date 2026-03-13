#include "Utility.h"
#include "Ultrasonic.h"

// ====== PROGRAM VARIABLES ======
int16_t gyroZ;                // Raw gyro Z-axis reading
float gyroZOffset = 0;        // Calibration offset
float currentAngle = 0;       // Current angle in degrees
unsigned long lastTime = 0;   // Last read time
CRGB leds[NUM_LEDS];          // Current LED Color values
Servo scanServo;              // Servo
int obCounter = 0;
int turnCounter = 0;
int distance = 0;
int state = 0;

// Function to print all info for testing purposes
void printInfo(){
    println("PROGRAM INFO:");
    print("[LIGHT LEVELS] ");
    print(analogRead(LINE_L));
    print(" | ");
    print(analogRead(LINE_C));
    print(" | ");
    println(analogRead(LINE_R));
    print("[DISTANCE] ");
    println(getDistance());
    delay(150);
}