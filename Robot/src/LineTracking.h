#pragma once
#include "Utility.h"

// Bool function to check if only one specified line tracker is off the line
bool offLine(int lineTracker);

// Bool function to check if all are off the line 
bool allOffLine();

// Bool function to check if any are off the line 
bool anyOffLine();

// Function to adjust the robot's direction if it detects a line
void lineAdjustAway();

void followLine();