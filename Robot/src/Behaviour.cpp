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
      updateLineTrackers();
      if (anyTrackersOnLine()){
        state = 3;
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

// Function that determines what to do for line tracking
void lineTrackingMode(){

  updateLineTrackers();
  moveMotors(0, 0);

  // *Needs to add the other if statesments for line adjust and stopping
  // *Need to make different varients to line adjust so it works with just white or black lines

  // If not on a line, go back to roaming
  if (allTrackersOffLine()){
    state = 1;
  }

  // Check to see if on black line
  else if (anyTrackersDetectLine(LINE_THRESHOLD_BLACK, 0)){
    finished();
  }

  // If on a line to stop, go to stationary
  else if (stopCenterCheck()){
    state = 0;
  }

  // ========== Turn on line modes ============================================

  // If center is on black line and turnLineBlack is true, turnOnLine
  else if (turnLineBlack && trackerDetectLine(lineCValue, LINE_THRESHOLD_BLACK, 0) && trackingLineBlack){
    turnOnLine();
  }

  // If center is on white line and turnLineWhite is true, turnOnLine
  else if (turnLineWhite && trackerDetectLine(lineCValue, LINE_THRESHOLD_WHITE, 1) && trackingLineWhite){
    turnOnLine();
  }

  // ========== Adjust line modes =============================================

  // If on a white line and adjust on white line, adjust
  else if (anyTrackersDetectLine(LINE_THRESHOLD_WHITE, 1) && adjustLineWhite && trackingLineWhite){
    lineAdjustType(LINE_THRESHOLD_WHITE, 1);
  }

  // If on a black line and adjust on black line, adjust
  else if (anyTrackersDetectLine(LINE_THRESHOLD_BLACK, 0) && adjustLineBlack && trackingLineBlack){
    lineAdjustType(LINE_THRESHOLD_BLACK, 0);
  }
  
  // ========== Follow line modes =============================================

  // If any on white and we are only following white, start following white
  else if (anyTrackersDetectLine(LINE_THRESHOLD_WHITE, 1) && (followLineWhite && trackingLineWhite) && !stopLineWhite){
    followingLineWhite();
  }

  // If any on Black and we are only following black, start following black
  else if (anyTrackersDetectLine(LINE_THRESHOLD_BLACK, 0) && (followLineBlack && trackingLineBlack) && !stopLineBlack){
    followingLineBlack();
  }
}

// A fucntion that determines if it has crossed the finish line or not
void finished(){
  if ((blackLineCounter < 1)){
    blackLineCounter++;
    rotate(180);
    updateLineTrackers();
    while (anyTrackersDetectLine(LINE_THRESHOLD_BLACK, 0)){
      updateLineTrackers();
      moveMotors(SPEED_NORMAL, SPEED_NORMAL);
    }
    state = 1;
  }

  else if (blackLineCounter >= 1){
    doJig();
    state = 0;
  }
}

void doJig(){
  moveMotors(150, -150);
  delay(5000);
  moveMotors(0, 0);
}