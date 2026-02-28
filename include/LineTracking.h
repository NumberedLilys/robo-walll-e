#include "Utility.cpp"

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