#pragma once
#include "Utility.h"

// Bool function to check if only one specified line tracker is off the line
bool offLine(int lineTracker);

// Bool function to check if all are off the line 
bool allOffLine();

// Bool function to check if any are on the line
bool anyOnLine();

// UNUSED FUNCTION
void followLine();

// A function that updates the global line tracker values to deduce the use of analogRead
void updateLineTrackers();

// Check for center line tracker for complete stop
bool CenterCheck();

// Double Check for center line tracker for complete stop
bool stopCenterCheck();