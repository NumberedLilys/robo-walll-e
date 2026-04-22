#include "Ultrasonic.h"

// ===== ULTRASONIC SENSOR FUNCTIONS =====

// Returns distance in centimeters, or 0 if invalid
int getDistance() {
  int validReading = 0;
  int attempts = 0;
  
  while (validReading == 0 && attempts < 3) {
    if (attempts > 0) delay(10);  // Only delay on retries (CHANGED FROM 60 TO 10)
    
    digitalWrite(US_OUT, LOW);
    delayMicroseconds(2);
    digitalWrite(US_OUT, HIGH);
    delayMicroseconds(10);
    digitalWrite(US_OUT, LOW);
    
    long duration = pulseIn(US_IN, HIGH, 30000);
    int distance = duration * 0.034 / 2;
    
    if (duration > 0 && distance <= 500) { // 200
      validReading = distance;
    }
    
    attempts++;
  }
  
  return validReading;
}

bool invalidWallCheck(){
  bool flag = 0;
  int numval = 0;
  for (int i = 0 ; i < 3 ; i++){
    if (numval == 2){
      flag = 1;
      break;
    }
    else if (getDistance() == 0){
      numval++;
    }
    else {
      numval = 0;
      break;
    }
  }

  return flag;
}

// A function that checks if a wall is within minimum distance for a wall
bool distanceCheck(int multi){
  distance = getDistance();
  if (0 < distance && distance <= (MIN_DISTANCE*multi)){
    return true;
  } else {
    return false;
  }
}