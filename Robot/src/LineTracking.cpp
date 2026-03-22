#include "LineTracking.h"

// Bool function to check if only one specified line tracker is off the line
bool offLine(int lineTracker){

  // Left tracker
  if (lineTracker == 1){
    if ((LINE_THRESHOLD_WHITE < lineLValue) && (lineLValue < LINE_THRESHOLD_BLACK)){
      return true;
    } else {
      return false;
      }

  // Right tracker
  } else if (lineTracker == 3){
    if ((LINE_THRESHOLD_WHITE < lineRValue) && (lineRValue < LINE_THRESHOLD_BLACK)){
      return true;
    } else {
      return false;
      }

  // Centre tracker
  } else if (lineTracker == 2){
    if ((LINE_THRESHOLD_WHITE < lineCValue) && (lineCValue < LINE_THRESHOLD_BLACK)){
      return true;
    } else {
      return false;
      }
  }
  else {
    return false;
    }
}

// Bool function to check if all are off the line
bool allOffLine(){
  if (offLine(1) && offLine(3) && offLine(2)){
    return true;
  } else {
    return false;
    }
}

// Bool function to check if any are off the line
bool anyOnLine(){
  if (!offLine(1) || !offLine(3) || !offLine(2)){
    return true;
  } else {
    return false;
    }
}

// A function that updates the global line tracker values to deduce the use of analogRead
void updateLineTrackers(){
  lineRValue = analogRead(LINE_R);
  lineCValue = analogRead(LINE_C);
  lineLValue = analogRead(LINE_L);
}
