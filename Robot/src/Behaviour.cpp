#include "Behaviour.h"

// Function to turn a set amount of degrees
void rotate(int targetAngle){
  setLed(state);

  // If angle is 180 or -180, rotate 90 or -90 twice
  if (targetAngle == 180 || targetAngle == -180){
    largeAngle = true;
    rotate(targetAngle/2);
    rotate(targetAngle/2);
    resetAngle();
  }else{

    // Reset the angle at the beginning for accuracy
    resetAngle();

    while (true){
      updateGyroAngle();

      // If the target is left, turn left until reaching the target angle
      if (targetAngle < 0){
        moveMotors(-SPEED_TURN, SPEED_TURN);
        if (getAngle() <= targetAngle + INERTIA_ERROR){
          moveMotors(0, 0);
          break;
        }
      }
      
      // If the target is right, turn right until reaching the target angle
      else{
        moveMotors(SPEED_TURN, -SPEED_TURN);
        if (getAngle() >= targetAngle - INERTIA_ERROR){
          moveMotors(0, 0);
          break;
        }
      }
    }

    // Reset angle back to zero for accuracy
    if (!largeAngle){
      resetAngle();
    }
  }
  largeAngle = false;
}

// Function that makes the robot wait until a button is pressed before moving into the nest stage of operation
void stationary(){
  setLed(state);

  // Wait for button press
  while (digitalRead(BUTTON) == HIGH){}

  // After button press, start main loop of robotic operation after half a second delay
  println("BUTTON PRESSED - Calibrating Gyro..");
  calibrateGyro();
  println("Calibration Complete!");
  delay(200);

}

// A function that runs the default state of the robot
void roaming(){
  setLed(state);
  moveMotors(SPEED_NORMAL, SPEED_NORMAL);
  while (true){
    // ======== Line Tracking ========

    // Checks for lines and adjusts if necessary

    lineAdjust();

    // Center Line Sensor (Not Used)
    // if (!offLine(2)){
    //   break;
    // }

    // // Left Line sensor (Not Used)
    // else if (!offLine(1)){
    //   break;
    // }

    // // Right Line sensor (Not Used)
    // else if (!offLine(3)){
    //   break;
    // }

    // ======== Gyro Tracking ========

    // Adjusts for drift (0.5 deg)

    updateGyroAngle();
    
    // Adjust left
    if (getAngle() > 0.5){
      print("[LEFT] ");
      moveMotors(SPEED_DRIFT, SPEED_NORMAL);
    }
    
    // Adjust right
    else if (getAngle() < -0.5){
      print("[RIGT] ");
      moveMotors(SPEED_NORMAL, SPEED_DRIFT);
    }

    // Go forward
    else{
      print("[FRWD] ");
      moveMotors(SPEED_NORMAL, SPEED_NORMAL);
    }

    // ========= Ultrasonic ===========

    // If near a wall, stop

    distance = getDistance();

    if (0 < distance && distance < MIN_DISTANCE){
      moveMotors(0, 0);
      state = 2;
      break;
    }
  }
}

// Function to explore all possible turns about 90 deg in a maze
void navigateWall(){
  
  // While loop to check all possible outcomes
  while (true){

    // Start of navigation, turn right 90 deg
    if (obCounter == 0){
      obCounter++;
      moveMotors(0, 0);
      setServoAngleSmooth(0);
      delay(500);
      if (getDistance() > MIN_DISTANCE){
        obCounter = 0;
        centerServo();
        rotate(90);
        break;
      }
    }

    // If when turning right the robot cannot go forward, turn 180 deg left
    else if (getDistance() < MIN_DISTANCE && obCounter == 1){
      obCounter++;
      setServoAngleSmooth(180);
      delay(500);
      if (getDistance() > MIN_DISTANCE){
        obCounter = 0;
        centerServo();
        rotate(-90);
        break;
      }
    }

    // If the robot still cannot go forward, go back the way it came in (90 deg left)
    else if (getDistance() < MIN_DISTANCE && obCounter == 2){
      obCounter = 0;
      centerServo();
      rotate(-180);
      break;
    } 

    // If it can go forward, reset the obCounter variable
    else{
      obCounter = 0;
      break;
    }
  }

  // go back to roaming mode
  centerServo();
  state = 1;
}

// A function that uses the turn right twice then left method of navigating a maze
void rightTwoLeft(){
  while (true){

    // turn right 90 deg
    if (turnCounter < 2){
      turnCounter++;
      delay(500);
      rotate(90);
      break;
    
    // turn Left 90 deg
    } else if(turnCounter >= 2){
      turnCounter = 0;
      delay(500);
      rotate(-90);

      // instead, turn 180 if cannot go forward after turning left
      if (getDistance() < MIN_DISTANCE){
        rotate(180);
        turnCounter = 2;
      }
      break;
    }
  }

  // go back to roaming mode
  state = 1;
}