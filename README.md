# Robot Project (Wall-e)
## Description
(Name) is an autonomous robot designed to perform multiple tasks using integrated sensors and control/movement systems. It is capable of line tracking black and white lines, maintaining directional stability with gyroscopic feedback, detecting obstacles or walls using ultrasonic sensors, operating a servo motor to account for drift to ensure accurate turns/movement, and utilizing an LED indicator for visual feedback.

The robot is built to function independently by combining hardware components (gyroscope, servo, motors, line tracking) with programmed functionality to execute precise movements and responses to outside obstacles.
(Photo of robot?)

<details>
    <summary>Table of Contents</summary>
    - Name
        - Description

    - Requirements
        - Hardware
        - Software / Dependencies 

    - Usage
        - What it does

    - Structure

    - Troubleshooting (Common errors)

    - Limitations
    
    - Contributing

</details>

## Requirements
### Hardware
There is an expectation that the user has a working robot with all of the functionalities required.
- (All hardware requirements)
- Servo motor
- Battery
- Gyroscope
- Ultrasonic Sensor
- LED light

### Software / Dependencies

## Usage
- Assemble the robot with all required hardware components.
- Ensure the robot battery is charged.
- Open the project file in the Arduino IDE.
- Install any required libraries.
- Connect the robot / controller to the computer and upload.
- Place the robot on a suitable surface with line markings and sufficient space for obstacle detection.

### What it does
(Name) is capable of performing the following autonomous functions:
- Tracks both white and black lines using infrared line sensors
- Detects obstacles or walls using an ultrasonic sensor
- Stops when an object or wall is detected within a defined distance
- Executes a precise 90-degree right turn when it locates a line
- Uses gyroscopic feedback to maintain directional stability (adjusts for exact turns)
- Applies drift control to ensure straight-line movement
- Provides LED feedback for status indication (Moving vs turning)
The integration of these systems allows the robot to navigate environments with little to no user input, with only a button click to start the program.

## Structure
## Troubleshooting
## Limitations
It currently has many errors in formatting due to a project structure change.
- The functionality/autonomy is limited to a small number of functions (Only turns right when it finds a line, stopping when hitting a wall).
- There is a limited number of hardware functions in the robot itself (no audio, one LED).

## Contributing
