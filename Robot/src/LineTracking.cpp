#include "LineTracking.h"

// ========== Foundational Line Tracking Functions ============================

// A function that takes the tracker value, theshold value, and type (gt or lt) and return true if it detects with that type
// If want to detect black if on black, use type 0; opposite if white
bool trackerDetectLine(int trackerValue, int threshold, bool type){

  // Type 1, for detecting if on white or off black
  if (type){
    if (0 < trackerValue && trackerValue < threshold){
      return true;
    }
  }

  // Type 0, for detecting if on black or off white
  else{
    if (threshold < trackerValue){
      return true;
    }
  }
  return false;
}

// A function that detects if all line trackers detect a line given a threshold and a type
// If want to detect if all on black, use type 0; opposite if white
bool allTrackersDetectLine(int threshold, int type){
  if (trackerDetectLine(lineLValue, threshold, type) 
   && trackerDetectLine(lineCValue, threshold, type)
   && trackerDetectLine(lineRValue, threshold, type)){
    return true;
  }
  return false;
}

// A function that detects if all line trackers detect a line given a threshold and a type
// If want to detect if any on black, use type 0; opposite if white
bool anyTrackersDetectLine(int theshold, int type){
  if (trackerDetectLine(lineLValue, theshold, type) 
   || trackerDetectLine(lineCValue, theshold, type)
   || trackerDetectLine(lineRValue, theshold, type)){
    return true;
  }
  return false;
}

// A function that detects if the given tracker is on a line or not
bool trackerOnLine(int trackerValue){
  if (trackerDetectLine(trackerValue, LINE_THRESHOLD_BLACK, 0) || trackerDetectLine(trackerValue, LINE_THRESHOLD_WHITE, 1)){
    return true;
  }
  return false;
}

// A function that detects if the given tracker is off a line or not
bool trackerOffLine(int trackerValue){
  if (trackerDetectLine(trackerValue, LINE_THRESHOLD_BLACK, 1) && trackerDetectLine(trackerValue, LINE_THRESHOLD_WHITE, 0)){
    return true;
  }
  return false;
}

// A function that detects if any trackers are on a line
bool anyTrackersOnLine(){
  if (anyTrackersDetectLine(LINE_THRESHOLD_BLACK, 0) || anyTrackersDetectLine(LINE_THRESHOLD_WHITE, 1)){
    return true;
  }
  return false;
}

// A function that detects if all trackers are off a line
bool allTrackersOffLine(){
  if (allTrackersDetectLine(LINE_THRESHOLD_BLACK, 1) && allTrackersDetectLine(LINE_THRESHOLD_WHITE, 0)){
    return true;
  }
  return false;
}

// ========== Line Following Functions ========================================

// Function that follows black lines only
void followingLineBlack(){
  if (trackerDetectLine(lineCValue, LINE_THRESHOLD_BLACK, 0)){
    followLineThinType(LINE_THRESHOLD_BLACK, 0);
  } else{
    followLineThickType(LINE_THRESHOLD_BLACK, 0);
  }
}

// Function that follows white lines only
void followingLineWhite(){
  // if (trackerDetectLine(lineCValue, LINE_THRESHOLD_WHITE, 1) &&
  //     trackerDetectLine(lineLValue, LINE_THRESHOLD_WHITE, 0) &&
  //     trackerDetectLine(lineRValue, LINE_THRESHOLD_WHITE, 0)){
  //   followLineThinType(LINE_THRESHOLD_WHITE, 1);
  // } else{
  //   followLineThickType(LINE_THRESHOLD_WHITE, 1);
  // }
  followLineThickType(LINE_THRESHOLD_WHITE, 1);
}

// ========== Thin Line Tracking ==============================================

// A function that goes forward and adjusts until it detects all sensors off and decided which way to go
void followLineThinType(int threshold, bool type){

  moveMotors(SPEED_FOLLOW, SPEED_FOLLOW);

  if (allTrackersDetectLine(threshold, !type)){
    state = 1;
    resetAngle();
  }

  else if (trackerDetectLine(lineLValue, threshold, type)){
    rotate(-1);
  }

  else if (trackerDetectLine(lineRValue, threshold, type)){
    rotate(1);
  }
}

// ========== Thick Line Tracking =============================================

// A function that goes forward and adjusts until it detects all sensors off and decided which way to go
void followLineThickType(int threshold, bool type){

  moveMotors(SPEED_FOLLOW, SPEED_FOLLOW);

  while (true){
    // Go the line following speed
    updateLineTrackers();

    // If all line trackers off of lines, go back to roaming
    if (allTrackersDetectLine(threshold, !type)){
      state = 1;
      break;
    }

    // // If only the center tracker is on the line, switch to thin line tracking
    // else if (trackerDetectLine(lineCValue, threshold, type)
    //       && trackerDetectLine(lineRValue, threshold, !type)
    //       && trackerDetectLine(lineLValue, threshold, !type)){
    //         followLineThinType(threshold, type);
    // }

    // If left tracker off line, adjust to the right
    else if (trackerDetectLine(lineLValue, threshold, !type)){
      moveMotors(SPEED_TURN, -SPEED_TURN);
    }

    // If right tracker off line, adjust to the left
    else if (trackerDetectLine(lineRValue, threshold, !type)){
      moveMotors(-SPEED_TURN, SPEED_TURN);
    }

    else{
      moveMotors(SPEED_FOLLOW, SPEED_FOLLOW);
    }
  }
}

// ========== Turn line tracking functions ====================================

// A function that turns on a specific line
void turnOnLine(){
  rotate(180);
  state = 1;
}

// ============ Adjust line tracking functions ================================

// A function that Checks for lines and adjusts if necessary
void lineAdjustType(int threshold, bool type){

  // Stop when a line is detected
  moveMotors(0, 0);

  // Update the line trackers
  updateLineTrackers();

  // Center Line sensor
  if (trackerDetectLine(lineCValue, threshold, type)){ // if center is on the line, call right two left and go backward for a little bit
    rightTwoLeft();
    moveMotors(-50, -50);
    delay(300);
    moveMotors(0, 0);
  }

  // Left Line Sensor
  else if (trackerDetectLine(lineLValue, threshold, type)){ // if left is on the line, adjust right
    rotate(1);
  }

  // Right Line sensor
  else if (trackerDetectLine(lineRValue, threshold, type)){ // if right is on the line, adjust left
    rotate(-1);
  }

  // If not on a line, go to roaming
  state = 1;
}

// ========== Other Line Tracking Functions ===================================

// A function that updates the global line tracker values to deduce the use of analogRead
void updateLineTrackers(){
  lineRValue = analogRead(LINE_R);
  lineCValue = analogRead(LINE_C);
  lineLValue = analogRead(LINE_L);
}

// Check for center line tracker for complete stop
bool CenterCheck(){
  updateLineTrackers();
  if (trackerDetectLine(lineCValue, LINE_THRESHOLD_BLACK, 0) && stopLineBlack){
    return true;
  }

  else if (trackerDetectLine(lineCValue, LINE_THRESHOLD_WHITE, 1) && stopLineWhite){
    return true;
  }

  return false;
}

// Double Check for center line tracker for complete stop
bool stopCenterCheck(){
  if (CenterCheck() && CenterCheck()){
    return true;
  }
  else{
    return false;
  }
}

bool invalidLineCheck(){
  bool flag = 0;
  int numval = 0;
  for (int i = 0 ; i < 4 ; i++){
    updateLineTrackers();
    if (numval == 3){
      flag = 1;
      break;
    }
    else if (lineRValue == 0 && lineCValue == 0 && lineLValue == 0){
      numval++;
    }
    else {
      numval = 0;
      break;
    }
  }

  return flag;
}