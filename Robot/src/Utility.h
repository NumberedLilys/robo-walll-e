#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <FastLED.h>
#include <Servo.h>

// ====== PIN CONSTANT VALUES ======

#define NUM_LEDS 2            // Number of LEDs on your board
#define PIN_RBGLED 4          // LED Pin
#define PWR_R 5               // Right Motor Power
#define PWR_L 6               // Left Motor Power
#define MTR_R 7               // Right Motor Control
#define MTR_L 8               // Left Motor Control
#define SERVO 10              // Servo Motor
#define MTR_ENABLE 3          // Motor Enable Pin
#define US_OUT 13             // Ultrasonic Sensor Input
#define US_IN 12              // Ultrasonic Sensor Output
#define LINE_L A2             // Left Line Tracker
#define LINE_C A1             // Center Line Tracker
#define LINE_R A0             // Right Line Tracker
#define BUTTON 2              // Push Button
#define GYRO 0x68             // Gyro Sensor Address

// ====== PROGRAM CONSTANTS ======
#define SPEED_NORMAL 100
#define SPEED_DRIFT 85
#define SPEED_TURN 75
#define SPEED_FOLLOW 50
#define LINE_THRESHOLD_BLACK 825 // 875
#define LINE_THRESHOLD_WHITE 300
#define MIN_DISTANCE 5 // 8
#define INERTIA_ERROR 3.5 // 3.5       // Higher if on less frictiony surface (Higher number undershots the turn)

// ====== PROGRAM VARIABLES ======
extern int16_t gyroZ;                // Raw gyro Z-axis reading
extern float gyroZOffset;            // Calibration offset
extern float currentAngle;           // Current angle in degrees
extern unsigned long lastTime;       // Last read time
extern CRGB leds[NUM_LEDS];          // Current LED Color values
extern CRGB stateColors[];           // Array of colors
extern Servo scanServo;              // Servo
extern bool flag;
extern int blackLineCounter;
extern int obCounter;              
extern int turnCounter;
extern int largeAngle;
extern int distance;
extern int state;
extern bool onBlackLine;

// Timer variables
extern unsigned long timerStartValue;
extern unsigned long timerEndValue;

// Data arrays
extern unsigned long timeArray[10];
extern char turnArray[10][5];
extern bool doubleClearenceArray[10];

// Line tracking variables instead of analog read
extern int lineRValue;
extern int lineCValue;
extern int lineLValue;

// ============ Adjustable functionality ======================================

// Specific functionality variable true if on
extern bool trackingLineBlack;
extern bool trackingLineWhite;
extern bool trackingWall;
extern bool trackingGyroAdjust;

// ============ Wall tracking functionality ===================================

// Stop for specific conditions
extern bool stopWall;

// ============ Line tracking functionality ===================================

// Stop for specific conditions
extern bool stopLineWhite;
extern bool stopLineBlack;

// Being able to follow different types of lines
extern bool followLineWhite;
extern bool followLineBlack;

// Being able to turn when detect a line
extern bool turnLineWhite;
extern bool turnLineBlack;

// Being able to adjust when detect a line
extern bool adjustLineWhite;
extern bool adjustLineBlack;

// ====== PROGRAM FUNCTIONS ======

// Function to print all info for testing purposes
void printInfo();

// Normal print function to save a little bit of typing each time
template<typename T>
void print(T message){
  Serial.print(message);
}

// println print function to save a little bit of typing each time
template<typename T>
void println(T message){
  Serial.println(message);
}

// A function to add the the array like a vector, newest value at index 0. Works for bool and numbers
template<typename T, size_t N>
void pushValue(T (&arr)[N], const T& value){

  // For loop that turns all of the end values the the one before it
  for (size_t i = N - 1; i > 0; i--){
    arr[i] = arr[i - 1];
  }

  // Turns the front value to the newest value
  arr[0] = value;
}

// A function to add the the array like a vector, newest value at index 0. Works only for turnarray with char[5]
template<size_t N, size_t M>
void pushTurn(char (&arr)[N][M], const char* value) {

    // For loop that turns all of the end values the the one before it
    for (size_t i = N - 1; i > 0; i--) {
        strcpy(arr[i], arr[i - 1]);
    }

    // Turns the front value to the newest value
    strncpy(arr[0], value, M - 1);

    // Ensures null terminator at the end
    arr[0][M - 1] = '\0';
}

// A function that clears the given array
template<typename T, size_t N>
void clearArr(T (&arr)[N]){

  // For loop that turns all of the values to zero
  for (size_t i = N - 1; i > 0; i--){
    arr[i] = 0;
  }
  arr[0] = 0;
}

// A function that clears the turn array
template<typename T, size_t N>
void clearTurnArr(T (&arr)[N]){

    // For loop that turns all of the end values the the one before it
    for (size_t i = N - 1; i > 0; i--) {
        arr[i][0] = '\0';
    }

    arr[0][0] = '\0';
}

// A function that starts the time to be added to the array of times
void startTimer();

// A function that ends the time and adds it to the array of times
void endTimer();