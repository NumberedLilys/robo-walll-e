#pragma once
#include "Gyro.h"
#include "Led.h"
#include "LineTracking.h"
#include "Motors.h"
#include "Servo.h"
#include "Ultrasonic.h"

// Function that makes the robot wait until a button is pressed before moving into the nest stage of operation
void stationary();

// A function that runs the default state of the robot
void roaming();

// Function to explore all possible turns about 90 deg in a maze
void navigateWall();

// Function to check which line to follow
void lineTrackingMode();

// A dance function for the robot once he finishes the maze
void doJig();

// A function that makes sure the robot does not go backwards in the maze
void regressionPrevention();