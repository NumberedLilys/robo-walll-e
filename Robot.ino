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

// ======= MY FUNCTIONS ======

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

// Bool function to check if only one specified line tracker is off the line
bool offLine(int lineTracker){

  // Left tracker
  if (lineTracker == 1){
    if ((LINE_THRESHOLD_WHITE < analogRead(LINE_L)) && (analogRead(LINE_L) < LINE_THRESHOLD_BLACK)){
      return true;
    } else {
      return false;
      }
  // Right tracker
  } else if (lineTracker == 3){
    if (LINE_THRESHOLD_WHITE < analogRead(LINE_R) && analogRead(LINE_R) < LINE_THRESHOLD_BLACK){
      return true;
    } else {
      return false;
      }
  // Centre tracker
  } else if (lineTracker == 2){
    if ((LINE_THRESHOLD_WHITE < analogRead(LINE_C)) && (analogRead(LINE_C) < LINE_THRESHOLD_BLACK)){
      return true;
    } else {
      return false;
      }
  }
  else {
    return false;
    }
}

// Bool function to check if all are off the line (NOT USED)
bool allOffLine(){
  if (offLine(1) && offLine(3) && offLine(2)){
    return true;
  } else {
    return false;
    }
}

// Function that moves the motors given speed. Negative means go backwards
// Credit to Aryam Sharma for this function, this was a direct inspiration from his function in the Robo Swap
void moveMotors(int lSpeed, int rSpeed){

  // === Left wheel direction ===

  // If greater than or equal to 0, go forward
  if (lSpeed >= 0) {
    digitalWrite(MTR_L, HIGH);
  }

  // Else go backward
  else {
    digitalWrite(MTR_L, LOW);
  }

  // === Right wheel direction ===

  // If greater than or equal to 0, go forward
  if (rSpeed >= 0) {
    digitalWrite(MTR_R, HIGH);
  }

  // Else go backward
  else {
    digitalWrite(MTR_R, LOW);
  }

  // Speed adjustment
  analogWrite(PWR_L, constrain(abs(lSpeed), 0, 255));
  analogWrite(PWR_R, constrain(abs(rSpeed), 0, 255));
}

// Function that makes the robot wait until a button is pressed before moving into the nest stage of operation
void stationary(){

  // Wait for button press
  while (digitalRead(BUTTON) == HIGH) {

  }

  // After button press, start main loop of robotic operation after half a second delay
  Serial.println("BUTTON PRESSED - Begining WALLE Operation...");
  calibrateGyro();
  delay(500);

}

/* 
* A function that goes forward with a maximum of 0.5 degree drift.
* If it finds a line infront of it, it turns 90 degrees right
* If it it slightly on a line it adjusts either 3 deg left or right
* If it is within 10 cm from a wall, it will stop
*/
void roaming(){
  moveMotors(SPEED_NORMAL, SPEED_NORMAL);
  
  // While loop to exit from a given point if an if statement is true
  while (true){
    // === Line Tracking ===

    // Center Line Sensor
    if (!offLine(2)){
      state = 3;
      break;
    }

    // Left Line sensor
    else if (!offLine(1)){
      rotate(3);
      break;
    }

    // Right Line sensor
    else if (!offLine(3)){
      rotate(-3);
      break;
    }

    // === Gyro Tracking ===

    updateGyroAngle();
    
    // Adjust left
    if (getAngle() > 0.5){
      print("[LEFT] ");
      moveMotors(SPEED_DRIFT, SPEED_NORMAL);
    }
    
    // Adjust right
    else if (getAngle() < -0.5){
      print("[RIGT] ");
      moveMotors(SPEED_NORMAL, SPEED_DRIFT);
    }

    // Go forward
    else{
      print("[FRWD] ");
      moveMotors(SPEED_NORMAL, SPEED_NORMAL);
    }

    // Variable to keep track of the current given distance, 
    // if not for this it can produce two different distances with different values (Invalid = 0)
    distance = getDistance();

    // If near a wall, stop. Added 0 < .. for invalid readings
    if (0 < distance && distance < MIN_DISTANCE){
      moveMotors(0, 0);
      state = 0;
      break;
    }

    // print to screen of all info needed
    print("DIS: ");
    print(distance);
    print(" ");
    print("ANG: ");
    println(getAngle());
  }
}

// Function to turn a set amount of degrees
void rotate(int targetAngle){

  // Reset the angle for accuracy
  resetAngle();

  // While loop to rotate until it has reached the given angle
  while (true){
    updateGyroAngle();

    // If the target is left, turn left until reaching the target angle
    if (targetAngle < 0){
      moveMotors(-SPEED_TURN, SPEED_TURN);
      if (getAngle() <= targetAngle + INERTIA_ERROR){
        moveMotors(0, 0);
        break;
      }
    }

    // If the target if right, turn right until reaching the target angle
    else{
      moveMotors(SPEED_TURN, -SPEED_TURN);
      if (getAngle() >= targetAngle - INERTIA_ERROR){
        moveMotors(0, 0);
        break;
      }
    }
  }

  // Reset angle back to zero for accuracy
  resetAngle();
}

// Function to explore all possible turns about 90 deg in a maze
void navigateWall(){
  
  // While loop to check all possible outcomes
  while (true){

    // Start of navigation, turn right 90 deg
    if (obCounter == 0){
      obCounter++;
      rotate(90);
    }

    // If when turning right the robot cannot go forward, turn 180 deg left
    else if (getDistance() < MIN_DISTANCE && obCounter == 1){
      obCounter++;
      rotate(-180);
    }

    // If the robot still cannot go forward, go back the way it came in (90 deg left)
    else if (getDistance() < MIN_DISTANCE && obCounter == 2){
      obCounter = 0;
      rotate(-90);
      break;
    } 

    // If it can go forward, reset the obCounter variable
    else{
      obCounter = 0;
      break;
    }
  }

  // go back to roaming mode
  state = 1;
}

// A function that makes the robot go back and turn 90 deg after off the line
void navigateLine(){
  moveMotors(-SPEED_TURN, -SPEED_TURN);
  delay(100);
  moveMotors(0, 0);
  delay(100);
  rotate(90);
  delay(100);
  state = 1;
}

// Function to follow a line (NOT USED)
void followLine(int lightThreshold){
  state = -1;
}

// ====== LED FUNCTIONS ======

void ledOn(CRGB color) {
  leds[0] = color;
  FastLED.show();
}

void ledOff() {
  leds[0] = CRGB::Black;
  FastLED.show();
}

// ===== SERVO FUNCTIONS =====

// Set servo angle (0–180 degrees)
void setServoAngle(int angle) {
  static int lastAngle = -1;
  angle = constrain(angle, 0, 180);

  if (angle != lastAngle) {
    scanServo.write(angle);
    delay(15);  // Allow servo to settle
    lastAngle = angle;
  }
}


// Center the servo
void centerServo() {
  setServoAngle(90);
}


// ====== GYRO FUNCTIONS ======

// Initialize Gyro Sensor
bool setupGyro() {
  Wire.begin();
  Wire.beginTransmission(GYRO);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // Wake up MPU6050
  byte error = Wire.endTransmission();
  
  if (error != 0) {
    return false;
  }
  
  // Configure gyro sensitivity (±250 deg/s)
  Wire.beginTransmission(GYRO);
  Wire.write(0x1B);  // GYRO_CONFIG register
  Wire.write(0x00);  // ±250 deg/s
  Wire.endTransmission();
  
  lastTime = millis();
  return true;
}

// Calibrate gyro (robot must be stationary!)
void calibrateGyro() {
  delay(500);
  
  long sum = 0;
  int samples = 100;
  
  for (int i = 0; i < samples; i++) {
    Wire.beginTransmission(GYRO);
    Wire.write(0x47);  // GYRO_ZOUT_H register
    Wire.endTransmission(false);
    Wire.requestFrom(GYRO, 2, true);
    
    int16_t gz = Wire.read() << 8 | Wire.read();
    sum += gz;
    delay(10);
  }
  
  gyroZOffset = sum / samples;
  currentAngle = 0;
}

// Read gyro Z-axis
int16_t readGyroZ() {
  Wire.beginTransmission(GYRO);
  Wire.write(0x47);  // GYRO_ZOUT_H register
  Wire.endTransmission(false);
  Wire.requestFrom(GYRO, 2, true);
  
  int16_t gz = Wire.read() << 8 | Wire.read();
  return gz;
}

// MUST be called frequently (e.g., every loop iteration)
// Angle accuracy degrades if this is not called often
void updateGyroAngle() {
  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;  // Time in seconds
  lastTime = now;
  
  // Read gyro
  gyroZ = readGyroZ();
  
  // Convert to degrees per second (sensitivity = 131 for ±250 deg/s)
  // INVERTED THE SIGN HERE to fix direction!
  float gyroRate = -((gyroZ - gyroZOffset) / 131.0);
  
  // Integrate to get angle
  currentAngle += gyroRate * dt;
  
  // Keep angle in range -180 to +180
  if (currentAngle > 180) currentAngle -= 360;
  if (currentAngle < -180) currentAngle += 360;
}

// Reset angle to zero
void resetAngle() {
  currentAngle = 0;
}

// Get current angle
float getAngle() {
  return currentAngle;
}

// ===== ULTRASONIC SENSOR FUNCTIONS =====

// Returns distance in centimeters, or 0 if invalid
int getDistance() {
  int validReading = 0;
  int attempts = 0;
  
  while (validReading == 0 && attempts < 3) {
    if (attempts > 0) delay(10);  // Only delay on retries (CHANGED FROM 60 TO 10)
    
    digitalWrite(US_OUT, LOW);
    delayMicroseconds(2);
    digitalWrite(US_OUT, HIGH);
    delayMicroseconds(10);
    digitalWrite(US_OUT, LOW);
    
    long duration = pulseIn(US_IN, HIGH, 30000);
    int distance = duration * 0.034 / 2;
    
    if (duration > 0 && distance <= 200) {
      validReading = distance;
    }
    
    attempts++;
  }
  
  return validReading;
}
