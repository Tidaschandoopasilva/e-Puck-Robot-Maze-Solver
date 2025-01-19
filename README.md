# Color Detection and Wall-Following Robot in Webots

This project implements an e-puck robot simulation in Webots that detects specific wall colors (Red, Yellow, Pink, Brown, Green) and navigates a maze using wall-following logic. Once all the colors are detected, the robot stops automatically.

## Features

- **Color Detection:** The robot uses its camera to detect walls with predefined colors.
- **Wall-Following:** The robot uses proximity sensors to navigate the maze while avoiding obstacles.
- **Stopping Condition:** The robot halts after detecting all target colors.
- **Real-Time Feedback:** The detected color names are printed to the console.

## Project Structure

.
├── controllers
│   ├── color_detection_controller.c  # Main Webots controller code
├── worlds
│   ├── maze_arena.wbt                # Maze environment for the robot
├── README.md                         # Project documentation

## Prerequisites

- **Webots R2023b or later:** Install from [Cyberbotics Webots](https://cyberbotics.com).
- **C Compiler:** Ensure you have a C development environment.

## Setup Instructions

1. **Clone the Repository:**

2. **Open the Project in Webots:**
   - Launch Webots.
   - Open the `maze_arena.wbt` world file from the `worlds` directory.

3. **Set the Controller:**
   - Assign the `mycontroller1` to the e-puck robot in the simulation.

4. **Run the Simulation:**
   - Start the simulation and monitor the console output for detected colors.

## Usage

1. **Robot Behavior:**
   - The robot follows walls and uses its camera to detect colors.
   - When a target color is detected, it prints the color name to the console.
   - After detecting all target colors, the robot stops.

2. **Customizing the Maze:**
   - Modify the `maze_arena.wbt` file to adjust wall placements and colors.

## Code Overview

### Key Functions:

- **Color Detection:**
  The is_color_match function checks if the camera detects a target color by comparing RGB values against predefined thresholds.

- **Wall-Following Logic:**
  The robot uses proximity sensors to:
  - Avoid obstacles.
  - Follow walls on its left side.
  - Turn at corners or move forward when no walls are nearby.

- **Stopping Condition:**
  The robot stops when all colors are detected, using a current_color_index variable to track progress.

### Key Thresholds:

const int RED_THRESHOLD[3] = {255, 0, 0};
const int YELLOW_THRESHOLD[3] = {255, 255, 0};
const int PINK_THRESHOLD[3] = {255, 0, 255};
const int BROWN_THRESHOLD[3] = {165, 105, 30};
const int GREEN_THRESHOLD[3] = {0, 255, 0};

## Example Output

During the simulation, the robot will print the following to the console upon detecting colors:

Found Red
Found Yellow
Found Pink
Found Brown
Found Green
Robot stopped after detecting all colors.

## Customization

1. **Add New Colors:**
   - Extend the COLOR_COUNT, COLOR_NAMES, and threshold arrays.
   - Update the logic in the is_color_match function.

2. **Adjust Wall-Following:**
   - Modify proximity sensor thresholds to suit different maze designs.


## Acknowledgments

- **Webots:** For providing an excellent simulation platform.
- **Cyberbotics:** For the e-puck robot model.
- **GCTRONICS

## Contributing

Contributions are welcome! Feel free to fork the repository and submit a pull request with your enhancements or bug fixes.

Author 
Tidas Chandoopa Silva

