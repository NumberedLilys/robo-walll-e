#include "Utility.cpp"

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