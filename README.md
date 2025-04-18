# Smart Lock System using ESP32

This project demonstrates the creation of a **smart lock system** using the **ESP32** microcontroller. The system allows remote unlocking and locking of a door through a web interface and a servo motor.

## Overview

- **ESP32** controls a **servo motor** that locks or unlocks a door.
- The user provides a password via a **web interface** to unlock the door.
- The servo motor's position (lock/unlock) is displayed in real-time via WebSocket communication.
- The system uses **Wi-Fi** to connect to a network and serves a web page for user interaction.

### Key Features

- **Web-based interface** to control the lock.
- **Real-time updates** of the servo position.
- **Password protection** for unlocking.
- **Servo angle visualization** on the web page.

## Components Used

- **ESP32 Development Board**
- **Servo Motor** (e.g., SG90 or any small hobby servo)
- **Power Supply** for the ESP32 and servo motor
- **Jumper Wires** and **Breadboard** for connections
- **Wi-Fi Network** for the ESP32 to connect to the internet

## Circuit Diagram

- Connect the servo motor's **signal wire** to **GPIO 13** of the ESP32.
- The **power** and **ground** pins of the servo should be connected to the appropriate power supply.

## Software Requirements

- **Arduino IDE** (for programming the ESP32)
- **ESP32 Arduino core** (Install via the Arduino IDE Board Manager)
- **Web browser** for accessing the smart lock interface.

### Libraries Required

- `WiFi.h` - for Wi-Fi connectivity
- `WebServer.h` - for hosting the web server
- `WebSocketsServer.h` - for handling WebSocket communication
- `ESP32Servo.h` - for controlling the servo motor

### Installation

1. **Install the Arduino IDE** from [here](https://www.arduino.cc/en/software).
2. **Install ESP32 board support** in Arduino IDE:
   - Go to **File** -> **Preferences** and add the following URL in the **Additional Boards Manager URLs** field:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Go to **Tools** -> **Board** -> **Boards Manager**, search for `esp32`, and install it.
3. **Install required libraries**:
   - In Arduino IDE, go to **Sketch** -> **Include Library** -> **Manage Libraries**, search and install:
     - `ESP32Servo`
     - `WebServer`
     - `WebSocketsServer`
     - `WiFi`
4. **Upload the code** to your ESP32:
   - Select the correct **Board** and **Port** in the **Tools** menu.
   - Upload the code from the Arduino IDE.

## Usage

### Initial Setup

1. Power up the ESP32.
2. Once the ESP32 connects to the Wi-Fi, open the Serial Monitor. The ESP32 will display its IP address.
3. Open a web browser and enter the IP address of the ESP32. You should see the **Smart Lock** web interface.

### Interacting with the System

1. Enter the password (`1234`) into the keypad on the web interface.
2. Click **Submit** to unlock the door.
3. The servo will move to **180 degrees**, indicating the door is unlocked.
4. After 5 seconds, the servo will return to the initial **0 degrees** position, locking the door.
5. If the password is incorrect, an error message will appear.

## Web Interface

The web page consists of:

- **Left column**: Description of the project.
- **Middle column**: Real-time display of the servo motor angle.
- **Right column**: Keypad for entering the password and control buttons.

### WebSocket Communication

The system uses WebSocket communication to update the servo position in real-time on the webpage.

### Control Panel

The control panel on the right allows users to:

- **Press buttons** (0-9) to form a password.
- **Submit** the password for validation.
- **Clear** the entered password.

## Code Explanation

### Main Components

- **Wi-Fi Setup**: Connects the ESP32 to a local Wi-Fi network.
- **WebServer**: Hosts the web page and handles user requests.
- **WebSocketServer**: Handles real-time updates of the servo position.
- **Servo Control**: Moves the servo motor based on the password input.

### Password Handling

The password `1234` is required to move the servo to **180 degrees**. If the password is correct, the servo will be moved to the unlock position. Otherwise, an error message will be shown.

### WebSocket Communication

The servo's angle is broadcast to all connected WebSocket clients whenever the servo position changes. This allows the webpage to update in real-time.

## Troubleshooting

1. **Servo not moving**: Ensure the servo is properly powered and connected to GPIO 13.
2. **Cannot connect to Wi-Fi**: Double-check your Wi-Fi credentials and ensure the ESP32 is within range of the Wi-Fi router.
3. **Incorrect password**: The password must match exactly. Ensure no extra spaces or characters.

## Conclusion

This project demonstrates how to create an IoT-based smart lock system using the ESP32, showcasing the power of microcontrollers and web technologies in home automation. With basic modifications, this can be expanded into more complex applications.

## License

This project is licensed under the MIT License. See LICENSE for more details.
