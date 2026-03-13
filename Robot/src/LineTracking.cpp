#include "LineTracking.h"
#include "Motors.h"

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

void lineAdjust(){
  // ======== Line Tracking ========

  // Checks for lines and adjusts if necessary

  // Left Line sensor
  if (!offLine(1)){ // if left is on the line, adjust right
    moveMotors(SPEED_NORMAL, SPEED_TURN);
    delay(200);
    moveMotors(SPEED_NORMAL, SPEED_NORMAL);
    delay(200);

    moveMotors(SPEED_TURN, SPEED_NORMAL); // back to normal direction
    delay(100);
    moveMotors(SPEED_NORMAL, SPEED_NORMAL);
    // state = 1; // if in roaming, not necessary to change state
  }

  // Center Line Sensor
  else if (!offLine(2)){ // if center is on the line, break and go to line tracking mode? Or turn around?
    // state of line following or something (state = #)
    moveMotors(0, 0);
    delay(500);
  }

  // Right Line sensor
  else if (!offLine(3)){ // if right is on the line, adjust left
    moveMotors(SPEED_TURN, SPEED_NORMAL);
    delay(200);
    moveMotors(SPEED_NORMAL, SPEED_NORMAL);

    moveMotors(SPEED_NORMAL, SPEED_TURN); // back to normal direction
    delay(100);
    moveMotors(SPEED_NORMAL, SPEED_NORMAL);
    // state = 1; // if in roaming, not necessary to change state
  }
}