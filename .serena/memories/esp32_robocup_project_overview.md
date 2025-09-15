# ESP32 RoboCup 2025 Robotics Project

## Project Description
ESP32-based rescue robot program for RoboCup 2025 competition with motor control, arm manipulation, ultrasonic sensing, and serial communication.

## Build System
- **Platform**: PlatformIO for ESP32 development
- **Target**: ESP32 development board (`esp32dev`)
- **Framework**: Arduino
- **Key Commands**:
  - Build: `pio run`
  - Upload: `pio run --target upload`
  - Monitor: `pio run --target monitor`
  - Test: `pio test`

## Architecture Overview

### Core Libraries (lib/ directory)
1. **SerialIO** (`lib/serial_io/`): Message-based serial communication with ID-based request/response protocol
2. **MOTORIO** (`lib/motorio/`): PWM motor control for 4 tyre motors + 1 wire motor with timing constraints
3. **ARMIO** (`lib/armio/`): Servo-based arm control with feedback using ESP32Servo library
4. **UltrasonicIO** (`lib/ultrasonic_io/`): Distance sensing for 3 ultrasonic sensors
5. **MutexGuard** (`lib/mutex_guard/`): RAII-style FreeRTOS mutex wrapper

### Main Program (src/main.cpp)
- FreeRTOS multi-tasking for concurrent motor control
- Serial command parsing and response handling
- Round-robin ultrasonic sensor reading
- Emergency stop button functionality

## Communication Protocol
- `MOTOR <x> <y>`: Set motor values (legacy 2-motor, expanded to 4)
- `Rescue <aaaa><w>`: Set arm angle (4 digits) + wire state (1 digit)
- `GET button`: Read emergency stop button state
- `GET ultrasonic`: Get all 3 ultrasonic sensor readings
- `Wire <0|1>`: Control wire motor (0=retract, 1=extend)

## Hardware Pin Configuration
- Tyre motors: pins 13, 14, 15, 16
- Emergency button: pin 21
- Arm servo: pin 17, feedback: pin 34
- Wire control: pin 32
- Ultrasonic sensors: pins 18/19, 22/23, 26/27

## Code Style
- clang-format with Google style
- 2-space indentation, 100 char line limit
- Format command: `clang-format -i src/**/*.cpp lib/**/*.cpp lib/**/*.hpp`

## Development Workflow
- Code sync via `send.sh` script
- CI/CD with GitHub Actions (PlatformIO build + clang-format)
- VS Code + PlatformIO IDE extension
- Hardware testing on ESP32 required

## Commit Message Format
Conventional commits: `prefix: Details`
Prefixes: feat, fix, docs, style, refactor, perf, test, chore