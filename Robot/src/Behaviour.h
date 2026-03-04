#pragma once
#include "Gyro.h"
#include "Led.h"
#include "LineTracking.h"
#include "Motors.h"
#include "Servo.h"
#include "Ultrasonic.h"

// Function to turn a set amount of degrees
void rotate(int targetAngle);

// Function that makes the robot wait until a button is pressed before moving into the nest stage of operation
void stationary();

/* 
* A function that goes forward with a maximum of 0.5 degree drift.
* If it finds a line infront of it, it turns 90 degrees right
* If it it slightly on a line it adjusts either 3 deg left or right
* If it is within 10 cm from a wall, it will stop
*/

void roaming();

// Function to explore all possible turns about 90 deg in a maze
void navigateWall();

// A function that makes the robot go back and turn 90 deg after off the line
void navigateLine();

// Function to follow a line (NOT USED)
void followLine(int lightThreshold);