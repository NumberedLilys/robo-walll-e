# Robot Project (Walll-e)
## Description
Walll-e is an autonomous robot designed to perform multiple tasks using integrated sensors and control/movement systems. It is capable of line tracking black and white lines, maintaining directional stability with gyroscopic feedback, detecting obstacles or walls using ultrasonic sensors, operating a servo motor to account for drift to ensure accurate turns/movement, and utilizing an LED indicator for visual feedback.

The robot is built to function independently by combining hardware components (gyroscope, servo, motors, line tracking) with programmed functionality to execute precise movements and responses to outside obstacles.
<img width="4080" height="2296" alt="20260424_182310" src="https://github.com/user-attachments/assets/29aca31d-b1ca-4e82-8370-6bbe69e6e82b" />

<details>
    <summary><span style="font-size: 1.3em; font-weight: bold;">Table of Contents</span></summary>

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
- Arduino/VScode
- FastLED installed onto Arduino

## Usage
- Assemble the robot with all required hardware components.
- Ensure the robot battery is charged.
- Open the project file in the Arduino IDE.
- Install any required libraries.
- Connect the robot / controller to the computer and upload.
- Place the robot on a suitable surface with line markings and sufficient space for obstacle detection.

### What it does
Walll-e is capable of performing the following autonomous functions:
- Tracks both white and black lines using infrared line sensors
- Detects obstacles or walls using an ultrasonic sensor
- Stops when an object or wall is detected within a defined distance
- Executes a precise 90-degree right turn when it locates a line
- Uses gyroscopic feedback to maintain directional stability (adjusts for exact turns)
- Applies drift control to ensure straight-line movement
- Provides LED feedback for status indication (Moving vs turning)
The integration of these systems allows the robot to navigate environments with little to no user input, with only a button click to start the program.

## Structure
The structure of the code is as follows:
- Start in stationary mode (state = 0) and wait for a button press
- Once the button is pressed, goes to roaming mode.
- Roaming them looks for sensor input (Linetracking, Ultrasonic, Gyro) to determine what to do next
    - If it detects lines, it goes to linetracking
    - If it detects walls close to it, it goes to naviageWall function
    - If it drifts off course it adjusts to stay straight
- Naviage wall determines where to go based on its surroundings, it can make the robot turn right or left whether it determines if it can or not using the servo and ultrasonic sensor to look around.

## Troubleshooting
If the gyroscope doesn't calibrate properly, the robot's LED goes red and requires a restart before use.

## Limitations
It currently has autonomous functionality but has minor limitations with its hardware.
- The line tracking sensors have trouble distinguishing dark flooring and black tape
- There is a limited number of hardware functions in the robot itself (no audio, one LED)
- The Line following works but is slightly janky (is not as smooth as other functions).

## Contributing

*   [Lily H](https://github.com/NumberedLilys) - Author

*   [Frankie A](https://github.com/FrankieAntoine) - Co-Author
