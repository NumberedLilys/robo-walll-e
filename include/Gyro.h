#include "Utility.cpp"

// ====== GYRO FUNCTIONS ======

// Initialize Gyro Sensor
bool setupGyro();

// Calibrate gyro (robot must be stationary!)
void calibrateGyro();

// Read gyro Z-axis
int16_t readGyroZ();

// MUST be called frequently (e.g., every loop iteration)
// Angle accuracy degrades if this is not called often
void updateGyroAngle();

// Reset angle to zero
void resetAngle();

// Get current angle
float getAngle();