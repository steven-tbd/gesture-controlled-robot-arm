# Robotic Arm Control System

A versatile system for controlling a 4-DOF robotic arm using TouchDesigner, OpenFrameworks, and a Leap Motion sensor.

This project demonstrates a flexible control system with multiple input modes:
- **Real-time Gesture Control:** Using a Leap Motion sensor for direct manipulation.
- **Pre-Programmed Automation:** Using TouchDesigner's Animation COMP for repeatable keyframe sequences.
- **Manual Slider Control:** A UI for direct control over each servo.

A key feature of this system is the use of TouchDesigner for all high-level data processing (filtering, smoothing, mapping) before sending clean, simple commands via OSC to a dedicated OpenFrameworks C++ application that handles the low-level servo communication.

### Dependencies

This repository contains only the original project files. To run this project, you will need to install the following dependencies:

*   **openFrameworks:** [Download here](https://openframeworks.cc/download/) (This project was built with version `of_v20241228_vs_64_release`)
*   **Dynamixel SDK:** [Download here](https://github.com/ROBOTIS-GIT/DynamixelSDK)

### Compatibility

*   **TouchDesigner:** 2023.12370
*   **Visual Studio:** 2022 Community (with C++ development tools)
    
### Project Links & Demos

*   **Full Project Write-up:** [stevenmbenton.com/robotic-arm-control-system](https://stevenmbenton.com/robotic-arm-control-system/)
*   **YouTube Demo:** [Gesture-Controlled 'Pick and Place'](https://youtu.be/poKgKCw8hp4)
*   **YouTube Demo:** [Pre-programmed Automated Movement](https://youtu.be/FgYoJYX6Q9w)