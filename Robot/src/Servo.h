#pragma once
#include "Utility.h"

// ===== SERVO FUNCTIONS =====

// Set servo angle (0–180 degrees)
void setServoAngle(int angle);

// Center the servo
void centerServo();

// Set servo with smoothness
void setServoAngleSmooth(int angle);