/*
 * CSCI 1063U - Elegoo Smart Car V4.0
 *
 * Note: Some code in this file is not used since this was 
 * created with the idea that it was a maze that we have not seen before.
 * 
 * I have also slightly changed the starter code function "getDistance()" 
 * to have a 10 ms delay instead of 60 ms.
 * This creates more invalid readings (0 cm distance) but since the majority of this code is 
 * reading lines the decreased delay increases accuracy in what really matters (Line Reading)
 * 
 * Credit to Aryam Sharma is due for the function "moveMotors()". 
 * My version was a direct inspiration from his function in the Robo Swap day
 * 
 * I also switched the stater code MTR_L and MTR_R becease it is opposite to how I would preseve left and right (From behind)
 */

<<<<<<< HEAD
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
#define SPEED_TURN 60
#define LINE_THRESHOLD_BLACK 500
#define LINE_THRESHOLD_WHITE 40
#define MIN_DISTANCE 10
#define INERTIA_ERROR 2

// ====== PROGRAM VARIABLES ======
int16_t gyroZ;                // Raw gyro Z-axis reading
float gyroZOffset = 0;        // Calibration offset
float currentAngle = 0;       // Current angle in degrees
unsigned long lastTime = 0;   // Last read time
CRGB leds[NUM_LEDS];          // Current LED Color values
Servo scanServo;              // Servo
int obCounter = 0;
int distance = 0;
int state = 0;

=======
#include "Behaviour.h"
>>>>>>> 7c9e5677144c5edf58a959451a4de0ae339bd9b8

void setup() {

  // setup LED
  FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, NUM_LEDS);
  FastLED.setBrightness(50); // 0-255
  
  // Motor pins
  pinMode(PWR_R, OUTPUT);
  pinMode(PWR_L, OUTPUT);
  pinMode(MTR_L, OUTPUT);
  pinMode(MTR_R, OUTPUT);
  pinMode(MTR_ENABLE, OUTPUT);
  
  // Ultrasonic sensor pins
  pinMode(US_OUT, OUTPUT);
  pinMode(US_IN, INPUT);

  // Line tracking sensor pins
  pinMode(LINE_L, INPUT);
  pinMode(LINE_C, INPUT);
  pinMode(LINE_R, INPUT);
  
  // Button pin
  pinMode(BUTTON, INPUT_PULLUP);

  // Enable motor driver
  digitalWrite(MTR_ENABLE, HIGH);

  // Initialize serial communication
  Serial.begin(9600);

  // Initialize Servo motor
  scanServo.attach(SERVO);
  centerServo();   // Center position

  // Initialize Gyro - hard stop if failed
  if (!setupGyro()) {
    ledOn(CRGB::Red);
    while (true);  // Hard stop
  }

  calibrateGyro();
  state = 0;
}

// Main loop for robots code
void loop() {
  updateGyroAngle();
  switch (state){
    
    // Test case
    case -1:
      stationary();
      print("[LIGHT LEVEL] ");
      print(analogRead(LINE_C));
      print(" / ");
      print("[DISTANCE] ");
      println(getDistance());
      break;

    // Stationary
    case 0:
      stationary();
      state = 1;
      break;

    // Forward
    case 1:
      roaming();
      break;
    
    // Navigate all 90 deg turns (NOT USED)
    case 2:
      navigateWall();
      break;

    // navigate line
    case 3:
      navigateLine();
      break;
  }
}