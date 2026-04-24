#include "Motors.h"

// Function that moves the motors given speed. Negative means go backwards
// Credit to Aryam Sharma for this function, this was a direct inspiration from his function in the Robo Swap
void moveMotors(int lSpeed, int rSpeed){

  // === Left wheel direction ===

  // If greater than or equal to 0, go forward
  if (lSpeed >= 0) {
    digitalWrite(MTR_L, HIGH);
  }

  // Else go backward
  else {
    digitalWrite(MTR_L, LOW);
  }

  // === Right wheel direction ===

  // If greater than or equal to 0, go forward
  if (rSpeed >= 0) {
    digitalWrite(MTR_R, HIGH);
  }

  // Else go backward
  else {
    digitalWrite(MTR_R, LOW);
  }

  // Speed adjustment
  analogWrite(PWR_L, constrain(abs(lSpeed), 0, 255));
  analogWrite(PWR_R, constrain(abs(rSpeed), 0, 255));
}

// Function to turn a set amount of degrees
void rotate(int targetAngle){

  // If angle is 180 or -180, rotate 90 or -90 twice
  if (targetAngle == 180 || targetAngle == -180){
    largeAngle = true;
    rotate(targetAngle/2);
    rotate(targetAngle/2);
    resetAngle();
  }else{

    // If the angle is a small angle, this prevents the inertia error from breaking the turn.
    if (targetAngle < 0){
      if (targetAngle > -90){
        targetAngle = targetAngle - INERTIA_ERROR;
      }
    } else{
      if (targetAngle < 90){
        targetAngle = targetAngle + INERTIA_ERROR;
      }
    }
    

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