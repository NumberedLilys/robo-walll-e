#include "Utility.h"

// ====== PROGRAM VARIABLES ======
int16_t gyroZ;                // Raw gyro Z-axis reading
float gyroZOffset = 0;        // Calibration offset
float currentAngle = 0;       // Current angle in degrees
unsigned long lastTime = 0;   // Last read time
CRGB leds[NUM_LEDS];          // Current LED Color values
Servo scanServo;              // Servo
int obCounter = 0;
int distance = 0;
int state = 0;
