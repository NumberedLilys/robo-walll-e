#include "Gyro.h"
#include "Led.h"
#include "LineTracking.h"
#include "Motors.h"
#include "Servo.h"
#include "Ultrasonic.h"

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