#pragma once
#include "Utility.h"
#include "Motors.h"

// ========== Foundational Line Tracking Functions ============================

// A function that takes the tracker value, theshold value, and type (gt (0) or lt (1)) and return true if it detects with that type
// If want to detect black if on black, use type 0; opposite if white
bool trackerDetectLine(int trackerValue, int threshold, bool type);

// A function that detects if all line trackers detect a line given a threshold and a type (gt (0) or lt (1))
// If want to detect if all on black, use type 0; opposite if white
bool allTrackersDetectLine(int theshold, int type);

// A function that detects if all line trackers detect a line given a threshold and a type (gt (0) or lt (1))
// If want to detect if any on black, use type 0; opposite if white
bool anyTrackersDetectLine(int theshold, int type);

// A function that detects if the given tracker is on a line or not
bool trackerOnLine(int trackerValue);

// A function that detects if the given tracker is off a line or not
bool trackerOffLine(int trackerValue);

// A function that detects if any trackers are on a line
bool anyTrackersOnLine();

// A function that detects if all trackers are off a line
bool allTrackersOffLine();

// ========== Line Following Functions ========================================

// Function to check which line to follow
void lineTrackingMode();

// Function that follows black lines only
void followingLineBlack();

// Function that follows white lines only
void followingLineWhite();

// Function that follows both types of lines
void followingLineBoth();

// ========== Thin Line Tracking ==============================================

// A function that goes forward and adjusts until it detects all sensors off and decided which way to go
void followLineThinType(int threshold, bool type = NULL);

// A function that goes forward and adjusts until it detects all sensors off and decided which way to go
void followLineThinBoth();

// ========== Thick Line Tracking =============================================

// A function that goes forward and adjusts until it detects all sensors off and decided which way to go
void followLineThickType(int threshold, bool type = NULL);

// A function that goes forward and adjusts until it detects all sensors off and decided which way to go
void followLineThickBoth();


// ========== Turn line tracking functions ====================================

void turnOnLine();

// ========== Other Line Tracking Functions ===================================

// A function that updates the global line tracker values to deduce the use of analogRead
void updateLineTrackers();

// Check for center line tracker for complete stop
bool CenterCheck();

// Double Check for center line tracker for complete stop
bool stopCenterCheck();