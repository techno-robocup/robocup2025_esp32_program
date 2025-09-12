# Claude Code Configuration
## Before talking about the project
When you make an output, please try to make the tokens less enough. You do not have to output any comments that can be understood without it.

Please concentrate on generating codes, or making information that are complicated and others.


## Project Overview

This is an ESP32-based robotics program for RoboCup 2025, designed to control a rescue robot with motor control, arm manipulation, ultrasonic sensing, and serial communication capabilities.

## Build System & Development Commands

This project uses PlatformIO for ESP32 development:

- **Build the project**: `pio run`
- **Upload to device**: `pio run --target upload`
- **Monitor serial output**: `pio run --target monitor`
- **Clean build**: `pio run --target clean`
- **Run tests**: `pio test`

The project targets ESP32 development board (`esp32dev`) using the Arduino framework.

## Code Architecture

### Core Components

The codebase is organized into modular libraries in `lib/` directory:

- **SerialIO** (`lib/serial_io/`): Message-based serial communication system
  - Handles structured message exchange with ID-based request/response protocol
  - Used for receiving motor commands and sensor data requests

- **MOTORIO** (`lib/motorio/`): PWM motor control with timing constraints
  - Controls 4 tyre motors and 1 wire motor with specific intervals
  - Implements precise timing control for motor operations

- **ARMIO** (`lib/armio/`): Servo-based arm control with feedback
  - Manages arm positioning and wire tension control
  - Uses ESP32Servo library for PWM servo control

- **UltrasonicIO** (`lib/ultrasonic_io/`): Distance sensing
  - Handles 3 ultrasonic sensors for obstacle detection
  - Provides distance measurements in centimeters

- **MutexGuard** (`lib/mutex_guard/`): Thread-safe resource management
  - RAII-style mutex wrapper for FreeRTOS synchronization

### Main Program Structure

`src/main.cpp` implements:

- **Multi-tasking**: Uses FreeRTOS tasks for concurrent motor control
- **Serial Protocol**: Command parsing for MOTOR, Rescue, GET button, GET ultrasonic, and Wire commands
- **Sensor Management**: Round-robin ultrasonic sensor reading
- **Emergency Stop**: Button-based emergency stop functionality

### Communication Protocol

The robot accepts these serial commands:
- `MOTOR <x> <y>`: Set motor values (legacy 2-motor format, expanded to 4 motors)
- `Rescue <aaaa><w>`: Set arm angle (4 digits) and wire state (1 digit)
- `GET button`: Read emergency stop button state
- `GET ultrasonic`: Get all 3 ultrasonic sensor readings
- `Wire <0|1>`: Control wire motor (0=retract, 1=extend)

## Code Style & Formatting

The project uses clang-format with Google style:
- 2-space indentation
- 100 character line limit
- Attach braces style
- Pointer alignment left

Format code with: `clang-format -i src/**/*.cpp lib/**/*.cpp lib/**/*.hpp`

## Hardware Configuration

Key pin assignments in `main.cpp`:
- Tyre motors: pins 13, 14, 15, 16
- Emergency button: pin 21
- Arm servo: pin 17, feedback: pin 34
- Wire control: pin 32
- Ultrasonic sensors: pins 18/19, 22/23, 26/27

## Commit Message Format

Follow conventional commit style as defined in `.cursor/rules/commit-rules.mdc`:
- First line: `prefix: Details` (short, clear description)
- Prefixes: `feat:`, `fix:`, `docs:`, `style:`, `refactor:`, `perf:`, `test:`, `chore:`
- Second line onwards: Explanation of why the change was needed (leave blank if unsure)

## Development Workflow

1. Code changes are automatically synced to remote device using `send.sh`
2. CI/CD runs PlatformIO build and clang-format checks via GitHub Actions
3. Use VS Code with PlatformIO IDE extension for development
4. Test on actual ESP32 hardware for verification