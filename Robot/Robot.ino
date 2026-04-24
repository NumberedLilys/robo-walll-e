/*
 * CSCI 1063U - Elegoo Smart Car V4.0
 *
 * I have slightly changed the starter code function "getDistance()" 
 * to have a 10 ms delay instead of 60 ms.
 * This creates more invalid readings (0 cm distance) but since the majority of this code is 
 * reading lines the decreased delay increases accuracy in what really matters (Line Reading)
 * 
 * Credit to Aryam Sharma is due for the function "moveMotors()". 
 * My version was a direct inspiration from his function in the Robo Swap day
 * 
 * I also switched the stater code MTR_L and MTR_R becease it is opposite to how I would preseve left and right (From behind)
 * 
 * This is the final version of walll-e during the duration of the course CSCI1063U
 */

#include "src//Behaviour.h"

void setup() {

  // setup LED
  FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, NUM_LEDS);
  FastLED.setBrightness(50); // 0-255
  ledOff();
  
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

  // Starting state, can be manually changed to -1 for test state
  state = 0;
}

// Main loop for robots code
void loop() {
  updateGyroAngle();
  switch (state){
    
    // Test case
    case -1:
      stationary();
      while (true){
        printInfo();
        delay(200);
      }
      break;

    // Stationary
    case 0:
      setLed(state);
      stationary();
      state = 1;
      break;

    // Main roaming state that determines functionality
    case 1:
      setLed(state);
      roaming();
      break;
    
    // Turning method NaviageWall
    case 2:
      setLed(state); 
      navigateWall();
      break;
    
    // Line tracking for following a line
    case 3: 
      setLed(state);
      lineTrackingMode();
      break;

  }
}