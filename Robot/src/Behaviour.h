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

// A function that runs the default state of the robot
void roaming();

// Function to explore all possible turns about 90 deg in a maze
void navigateWall();

// Function that turns right twice and then left
void rightTwoLeft();