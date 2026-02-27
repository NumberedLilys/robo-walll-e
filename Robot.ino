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

#include "Behaviour.h"

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