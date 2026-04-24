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
  randomSeed(micros());
  delay(200);
}

// A function that runs the default state of the robot
void roaming(){

  // Timer function for the time array
  startTimer();
  moveMotors(SPEED_NORMAL, SPEED_NORMAL);
  while (true){

    // ======== Gyro Tracking ========

    // Adjusts for drift (0.5 deg)

    if (trackingGyroAdjust){
      updateGyroAngle();
      
      // Adjust left
      if (getAngle() > 0.5){
        // print("[LEFT] ");
        moveMotors(SPEED_DRIFT, SPEED_NORMAL);
      }
      
      // Adjust right
      else if (getAngle() < -0.5){
        // print("[RIGT] ");
        moveMotors(SPEED_NORMAL, SPEED_DRIFT);
      }

      // Go forward
      else{
        // print("[FRWD] ");
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
        endTimer();
        moveMotors(0, 0);
        state = 2;
        break;
      }
    }

    // ======== Line Tracking ========

    // Checks for lines and adjusts if necessary
    
    if (trackingLineBlack || trackingLineWhite){
      updateLineTrackers();

      // Black line check for jig
      if (allTrackersDetectLine(LINE_THRESHOLD_BLACK, 0)){
        if (blackLineCounter < 2){
          onBlackLine = true;
        }
        else if (blackLineCounter >= 2){
          doJig();
          state = 0;
          break;
        }
      }
      else if (allTrackersDetectLine(LINE_THRESHOLD_BLACK, 1)){
        if (onBlackLine){
          blackLineCounter++;
          onBlackLine = false;
        }
      }

      // White line check
      if (allTrackersDetectLine(LINE_THRESHOLD_WHITE, 1)){
        clearArr(timeArray);
        clearArr(doubleClearenceArray);
        clearTurnArr(turnArray);
        state = 3;
        break;
      }
    }
  }
}

// Function to explore all possible turns about 90 deg in a maze
void navigateWall(){

  bool rightClearence = 0;
  bool leftClearence = 0;
  
  // ========== Checking ==========
  setServoAngleSmooth(0);
  delay(500);
  if (!distanceCheck(4)){
    rightClearence = 1;
  }
  setServoAngleSmooth(180);
  delay(500);
  if (!distanceCheck(4)){
    leftClearence = 1;
  }
  centerServo();
  delay(200);

  // =========== Logic ============
  if (rightClearence && !leftClearence){ // Only right is clear
    pushValue(doubleClearenceArray, false);
    pushTurn(turnArray, "RIGT");
    rotate(90);
  } else if (!rightClearence && leftClearence){ // Only left is clear
    pushValue(doubleClearenceArray, false);
    pushTurn(turnArray, "LEFT");
    rotate(-90);
  } else if (!rightClearence && !leftClearence){ // Both are not clear
    pushValue(doubleClearenceArray, false);
    pushTurn(turnArray, "SPIN");
    rotate(180);
  } else if (rightClearence && leftClearence){ // Both are clear
    pushValue(doubleClearenceArray, true);
    regressionPrevention();
  }

  // go back to roaming mode
  state = 1;
}

// Function that determines what to do for line tracking
void lineTrackingMode(){

  updateLineTrackers();
  moveMotors(0, 0);

  // If not on a line, go back to roaming
  if (allTrackersOffLine()){
    state = 1;
  }

  // If on a line to stop, go to stationary
  else if (stopCenterCheck()){
    state = 0;
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

  // Go back to roaming
  else{
    state = 1;
  }
}

// A dance function for the robot once he finishes the maze
void doJig(){
  rotate(30);
  ledOn(CRGB::HotPink);
  moveMotors(100, 25);
  delay(500);
  moveMotors(150, 15);
  delay(1500);
  moveMotors(100, 25);
  delay(750);

  ledOn(CRGB::Cyan);
  moveMotors(100, 100);
  delay(250);

  ledOn(CRGB::HotPink);
  moveMotors(25, 100);
  delay(500);
  moveMotors(15, 150);
  delay(1500);
  moveMotors(25, 100);
  delay(750);

  moveMotors(100, 100);
  delay(250);

  for (int i = 0; i < 10; i++){
    ledOn(CRGB::Red);
    moveMotors(100, 0);
    delay(100);
    ledOn(CRGB::Orange);
    moveMotors(0, 100);
    delay(100);
    ledOn(CRGB::Green);
    moveMotors(-100, 0);
    delay(100);
    ledOn(CRGB::Blue);
    moveMotors(0, -100);
    delay(100);
  }
  state = 0;
}

// A function that makes sure the robot does not go backwards in the maze
void regressionPrevention(){

  // If the last three turns were either right or left, turn the same direction to prevent going backward in progression
  if (strcmp(turnArray[0], turnArray[1]) == 0 && strcmp(turnArray[1], turnArray[2]) == 0 && (strcmp(turnArray[0], "RIGT") == 0 || strcmp(turnArray[0], "LEFT") == 0)){
    
    // If left
    if (strcmp(turnArray[0], "LEFT") == 0){
      ledOn(CRGB::Orange);
      clearArr(timeArray);
      clearArr(doubleClearenceArray);
      clearTurnArr(turnArray);
      rotate(-90);

    // If right
    } else{
      ledOn(CRGB::Orange);
      clearArr(timeArray);
      clearArr(doubleClearenceArray);
      clearTurnArr(turnArray);
      rotate(90);
    }

  // If both ways are clear and the last turns were not the same, randomize which way to go
  } else{
    int randomNum = random(2);
      if (randomNum == 0){

        // Push to turn array
        pushTurn(turnArray, "RIGT");
        rotate(90);
      } else{

        // push to turn array
        pushTurn(turnArray, "LEFT");
        rotate(-90);
      }
    }
}