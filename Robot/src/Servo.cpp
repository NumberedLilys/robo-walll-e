#include "Servo.h"

// ===== SERVO FUNCTIONS =====

// Set servo angle (0–180 degrees)
void setServoAngle(int angle) {
  static int lastAngle = -1;
  angle = constrain(angle, 0, 180);

  if (angle != lastAngle) {
    scanServo.write(angle);
    delay(15);  // Allow servo to settle
    lastAngle = angle;
  }
}

// Center the servo
void centerServo() {
  setServoAngle(90);
}

// Set servo with smoothness
void setServoAngleSmooth(int angle){

  // Turning right smoothly
  if (angle > 90){
    for (int i = 90; i <= angle; i = i + 2){
      setServoAngle(i);
    }
  }

  // Turning left smoothly
  if (angle < 90){
    for (int i = 90; i >= angle; i = i - 2){
      setServoAngle(i);
    }
  }

  // Set to center
  if (angle == 90){
    centerServo();
  }
}