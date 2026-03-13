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
#define LINE_THRESHOLD_BLACK 900
#define LINE_THRESHOLD_WHITE 200
#define MIN_DISTANCE 20
#define INERTIA_ERROR 1       // Higher if on less frictiony surface (Higher number undershots the turn)

// ====== PROGRAM VARIABLES ======
extern int16_t gyroZ;                // Raw gyro Z-axis reading
extern float gyroZOffset;        // Calibration offset
extern float currentAngle;       // Current angle in degrees
extern unsigned long lastTime;   // Last read time
extern CRGB leds[NUM_LEDS];          // Current LED Color values
extern CRGB stateColors[];
extern Servo scanServo;              // Servo
extern int obCounter;
extern int turnCounter;
extern int largeAngle;
extern int distance;
extern int state;

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