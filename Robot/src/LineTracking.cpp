#include "LineTracking.h"

// ========== Foundational Line Tracking Functions ============================

// A function that takes the tracker value, theshold value, and type (gt or lt) and return true if it detects with that type
// If want to detect black if on black, use type 0; opposite if white
bool trackerDetectLine(int trackerValue, int threshold, bool type){

  // Type 1, for detecting if on white or off black
  if (type){
    if (trackerValue < threshold){
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
  if (anyTrackersDetectLine(LINE_THRESHOLD_BLACK, 1) && anyTrackersDetectLine(LINE_THRESHOLD_WHITE, 0)){
    return true;
  }
  return false;
}

// ========== Line Following Functions ========================================

// Function that determines what to do for line tracking
void lineTrackingMode(){

  // *Needs to add the other if statesments for line adjust and stopping
  // *Need to make different varients to line adjust so it works with just white or black lines
  if (turnLineBlack && trackerDetectLine(lineCValue, LINE_THRESHOLD_BLACK, 0)){
    turnOnLine();
  }
  
  else if ((followLineBlack || followLineWhite)){
    if ((followLineWhite && trackingLineWhite) && (!followLineBlack || !trackingLineBlack) && !stopLineWhite){
      followingLineWhite();
    }

    else if ((followLineBlack && trackingLineBlack) && (!followLineWhite || !trackingLineWhite) && !stopLineBlack){
      followingLineBlack();
    }

    else if ((trackingLineBlack && trackingLineWhite) && (followLineBlack && followLineWhite) && (!stopLineBlack && !stopLineWhite)){
      followingLineBoth();
    }

    else{
      state = 0;
    }
  // } else if (){
    
  }
}

// Function that follows black lines only
void followingLineBlack(){
  if (trackerDetectLine(lineCValue, LINE_THRESHOLD_BLACK, 0)){
    followLineThinType(LINE_THRESHOLD_BLACK);
  } else{
    followLineThickType(LINE_THRESHOLD_BLACK, 1);
  }
}

// Function that follows white lines only
void followingLineWhite(){
  if (trackerDetectLine(lineCValue, LINE_THRESHOLD_WHITE, 1) &&
      trackerDetectLine(lineLValue, LINE_THRESHOLD_WHITE, 0) &&
      trackerDetectLine(lineRValue, LINE_THRESHOLD_WHITE, 0)){
    followLineThinType(LINE_THRESHOLD_WHITE);
  } else{
    followLineThickType(LINE_THRESHOLD_WHITE, 1);
  }
}

// Function that follows both types of lines
void followingLineBoth(){
  if (trackerOnLine(lineCValue)){
    followLineThinBoth();
  } else{
    followLineThickBoth();
  }
}

// ========== Thin Line Tracking ==============================================

// A function that goes forward and adjusts until it detects all sensors off and decided which way to go
void followLineThinType(int threshold, bool type){

  // Line of code to determine the type of tracking (1 for white means true if on line)
  if ((type == NULL) && (threshold == LINE_THRESHOLD_WHITE)){
    type = 1;
  }else{
    type = 0;
  }

  moveMotors(SPEED_FOLLOW, SPEED_FOLLOW);

  if (allTrackersDetectLine(threshold, !type)){
    state = 1;
  }

  else if (allTrackersDetectLine(threshold, type)){
    state = 5;
  }

  else if (trackerDetectLine(lineLValue, threshold, type)){
    rotate(-1);
  }

  else if (trackerDetectLine(lineRValue, threshold, type)){
    rotate(1);
  }
}

// A function that goes forward and adjusts until it detects all sensors off and decided which way to go
void followLineThinBoth(){
  moveMotors(SPEED_FOLLOW, SPEED_FOLLOW);

  if (allTrackersOffLine()){
    state = 1;
  }

  else if (!allTrackersOffLine()){
    state = 5;
  }

  else if (trackerOnLine(lineLValue)){
    rotate(-1);
  }

  else if (trackerOnLine(lineRValue)){
    rotate(1);
  }
}

// ========== Thick Line Tracking =============================================

// A function that goes forward and adjusts until it detects all sensors off and decided which way to go
void followLineThickType(int threshold, bool type){

  // // Line of code to determine the type of tracking (1 for white means true if on line)
  // if ((type == NULL) && (threshold == LINE_THRESHOLD_WHITE)){
  //   type = 1;
  // }else{
  //   type = 0;
  // }

  while (true){
    // Go the line following speed
    moveMotors(SPEED_FOLLOW, SPEED_FOLLOW);

    // If all line trackers off of lines, go back to roaming
    if (allTrackersDetectLine(threshold, !type)){
      state = 0;
      break;
    }

    // If left tracker off line, adjust to the right
    else if (trackerDetectLine(lineLValue, threshold, !type)){
      moveMotors(0, 0);
      rotate(1);
    }

    // If right tracker off line, adjust to the left
    else if (trackerDetectLine(lineRValue, threshold, !type)){
      moveMotors(0, 0);
      rotate(-1);
    }

    // // If only the center tracker is on the line, switch to thin line tracking
    // else if (trackerDetectLine(lineCValue, threshold, type)
    //       && trackerDetectLine(lineRValue, threshold, !type)
    //       && trackerDetectLine(lineLValue, threshold, !type)){
    //         followLineThinType(threshold, type);
    //       }
  }
}

// A function that goes forward and adjusts until it detects all sensors off and decided which way to go
void followLineThickBoth(){

  // Go the line following speed
  moveMotors(SPEED_FOLLOW, SPEED_FOLLOW);

  // If all line trackers off of lines, go back to roaming
  if (allTrackersOffLine()){
    state = 1;
  }

  // If left tracker off line, adjust to the right
  else if (trackerOffLine(lineLValue)){
    rotate(1);
  }

  // If right tracker off line, adjust to the left
  else if (trackerOffLine(lineRValue)){
    rotate(-1);
  }

  // If only the center tracker is on the line, switch to thin line tracking
  else if (trackerOnLine(lineCValue)
        && trackerOffLine(lineRValue)
        && trackerOffLine(lineLValue)){
          followLineThinBoth();
        }
}

// ========== Turn line tracking functions ====================================

void turnOnLine(){
  moveMotors(-SPEED_TURN, SPEED_TURN);
  if (trackerDetectLine(lineCValue, LINE_THRESHOLD_WHITE, 1)){
    moveMotors(0, 0);
    state = 4;
  }
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