#include "Behaviour.h"

// Function that makes the robot wait until a button is pressed before moving into the nest stage of operation
void stationary(){
  moveMotors(0, 0);

  // Wait for button press
  while (digitalRead(BUTTON) == HIGH){}

  // After button press, start main loop of robotic operation after half a second delay
  println("BUTTON PRESSED - Calibrating Gyro..");
  calibrateGyro();
  println("Calibration Complete!");
  delay(200);

}

// A function that runs the default state of the robot
void roaming(){
  moveMotors(SPEED_NORMAL, SPEED_NORMAL);
  while (true){

    // ======== Line Tracking ========

    // Checks for lines and adjusts if necessary

    if (trackingLineBlack || trackingLineWhite){
      if (stopCenterCheck()){
        state = 0;
        break;
      }
      else if (anyTrackersOnLine()){
        state = 4;
        break;
      }
    }

    // ======== Gyro Tracking ========

    // Adjusts for drift (0.5 deg)

    if (trackingGyroAdjust){
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
    }

    // ========= Ultrasonic ===========

    if (trackingWall){

      // If near a wall, stop
      flag = distanceCheck();
      if (flag && stopWall){
        state = 0;
        break;
      }
      else if (flag && !stopWall){
        moveMotors(0, 0);
        state = 2;
        break;
      }
    }
  }
}

// Function to explore all possible turns about 90 deg in a maze
void navigateWall(){
  
  // While loop to check all possible outcomes
  while (true){

    // Start of navigation, turn right 90 deg
    if (obCounter == 0){
      obCounter++;
      moveMotors(0, 0);
      setServoAngleSmooth(0);
      delay(500);
      if (getDistance() > (MIN_DISTANCE*3)){
        obCounter = 0;
        centerServo();
        rotate(90);
        break;
      }
    }

    // If when turning right the robot cannot go forward, turn 180 deg left
    else if (getDistance() < MIN_DISTANCE && obCounter == 1){
      obCounter++;
      setServoAngleSmooth(180);
      delay(500);
      if (getDistance() > (MIN_DISTANCE*3)){
        obCounter = 0;
        centerServo();
        rotate(-90);
        break;
      }
    }

    // If the robot still cannot go forward, go back the way it came in (90 deg left)
    else if (getDistance() < MIN_DISTANCE && obCounter == 2){
      obCounter = 0;
      centerServo();
      rotate(-180);
      break;
    } 

    // If it can go forward, reset the obCounter variable
    else{
      obCounter = 0;
      break;
    }
  }

  // go back to roaming mode
  centerServo();
  state = 1;
}

// A function that uses the turn right twice then left method of navigating a maze
void rightTwoLeft(){
  while (true){

    // turn right 90 deg
    if (turnCounter < 2){
      turnCounter++;
      delay(500);
      rotate(90);
      break;
    
    // turn Left 90 deg
    } else if(turnCounter >= 2){
      turnCounter = 0;
      delay(500);
      rotate(-90);

      // instead, turn 180 if cannot go forward after turning left
      if (distanceCheck()){
        rotate(180);
        turnCounter = 2;
      }
      break;
    }
  }

  // go back to roaming mode
  state = 1;
}

// A function that Checks for lines and adjusts if necessary
void lineAdjustAway(){

  // Stop when a line is detected
  moveMotors(0, 0);

  // Center Line sensor
  if (trackerOnLine(lineCValue)){ // if center is on the line, call right two left and go backward for a little bit
    rightTwoLeft();
    moveMotors(-50, -50);
    delay(300);
    moveMotors(0, 0);
  }

  // Left Line Sensor
  else if (trackerOnLine(lineLValue)){ // if left is on the line, adjust right
    rotate(2);
  }

  // Right Line sensor
  else if (trackerOnLine(lineRValue)){ // if right is on the line, adjust left
    rotate(-2);
  }

  // If not on a line, go to roaming
  else{
    state = 1;
  }

  // Update sensors at the end for the next loop
  updateLineTrackers();

}

// A function that will determine which direction to go when at a line crossroad
void crossroads(){

}