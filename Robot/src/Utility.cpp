#include "Utility.h"

// ====== PROGRAM VARIABLES ======
int16_t gyroZ;                // Raw gyro Z-axis reading
float gyroZOffset = 0;        // Calibration offset
float currentAngle = 0;       // Current angle in degrees
unsigned long lastTime = 0;   // Last read time
CRGB leds[NUM_LEDS];          // Current LED Color values
CRGB stateColors[] = {CRGB::Green, CRGB::Blue, CRGB::Purple, CRGB::White, CRGB::Pink, CRGB::Violet}; // State colors in order
Servo scanServo;              // Servo
bool flag = false;
int blackLineCounter = 0;
int obCounter = 0;
int turnCounter = 0;
int largeAngle = false;
int distance = 0;
int state = 0;

// Line tracking variables instead of analog read
int lineRValue = 0;
int lineCValue = 0;
int lineLValue = 0;

// ============ Adjustable functionality ======================================

// Specific functionality variable true if on
bool trackingLineBlack = 0;
bool trackingLineWhite = 0;
bool trackingWall = 1;
bool trackingGyroAdjust = 1;

// ============ Wall tracking functionality ===================================

// Stop for specific conditions
bool stopWall = 0;

// ============ Line tracking functionality ===================================

// Stop for specific conditions
bool stopLineWhite = 0;
bool stopLineBlack = 0;

// Being able to follow different types of lines
bool followLineWhite = 1;
bool followLineBlack = 0;

// Being able to turn when detect a line
bool turnLineWhite = 0;
bool turnLineBlack = 0;

// Being able to adjust when detect a line
bool adjustLineWhite = 0;
bool adjustLineBlack = 0;


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
    println(distance);
}