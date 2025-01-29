#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ESP32Servo.h>

// Wi-Fi credentials
const char* ssid = "--";
const char* password = "**";

// Create a web server and WebSocket server instance
WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// Servo motor setup
Servo myServo;
const int servoPin = 13;  // Connect servo signal to GPIO 13
const int initialAngle = 0;  // Servo's initial angle
const int targetAngle = 180;  // Servo's target angle
int currentAngle = 0;         // Store the current servo angle

// Define the unlock password
const String unlockPassword = "1234";  // Password to unlock (move servo to 180°)

// HTML UI
const char MAIN_page[] PROGMEM = R"=====( 
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Smart Lock Demo</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background: linear-gradient(to bottom, #f0faff, #ffffff);
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
    }

    .container {
      display: flex;
      justify-content: space-between;
      align-items: center;
      width: 90%;
      max-width: 1200px;
      background-color: #fff;
      border-radius: 10px;
      box-shadow: 0px 4px 10px rgba(0, 0, 0, 0.1);
      padding: 20px;
    }

    /* Left Column: Project Description */
    .description {
      flex: 1;
      background-color: #004080;
      color: white;
      padding: 20px;
      border-radius: 10px;
      text-align: center;
      font-size: 1.2rem;
    }

    .description h2 {
      font-size: 1.8rem;
      margin-bottom: 10px;
    }

    .description p {
      line-height: 1.6;
    }

    /* Middle Column: Servo Display */
    .servo-demo {
      flex: 1;
      text-align: center;
      margin: 0 20px;
    }

    .servo-demo h1 {
      font-size: 1.8rem;
      color: #004080;
    }

    .servo {
      width: 120px;
      height: 240px;
      background: #444;
      border-radius: 10px;
      margin: 20px auto;
      position: relative;
    }

    .servo .needle {
      width: 12px;
      height: 120px;
      background: #f7b500;
      position: absolute;
      bottom: 60px;
      left: 54px;
      transform-origin: bottom center;
      transform: rotate(0deg);
      transition: transform 0.5s ease;
    }

    .status {
      font-size: 1.2rem;
      color: #004080;
      margin-top: 15px;
    }

    /* Right Column: Control Panel */
    .control-panel {
      flex: 1;
      text-align: center;
    }

    .control-panel h2 {
      font-size: 1.8rem;
      color: #004080;
      margin-bottom: 10px;
    }

    .keypad {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 10px;
      margin: 20px auto;
      max-width: 200px;
    }

    .keypad button {
      padding: 15px;
      font-size: 18px;
      font-weight: bold;
      background-color: #004080;
      color: white;
      border: none;
      border-radius: 5px;
      cursor: pointer;
    }

    .keypad button:hover {
      background-color: #002f5f;
    }

    .actions button {
      padding: 10px 20px;
      font-size: 16px;
      background-color: #f7b500;
      color: #004080;
      border: none;
      border-radius: 5px;
      margin: 10px 5px;
      cursor: pointer;
    }

    .actions button:hover {
      background-color: #c69500;
    }
  </style>
</head>
<body>
  <div class="container">
    <!-- Left Column: Project Description -->
    <div class="description">
      <h2>PROJECT PURPOSE</h2>
      <p>
        This project uses an ESP32 microcontroller to create a smart locking system. It controls a servo motor, which physically locks or unlocks based on a user-provided password through a web-based interface. The system allows remote operation, real-time feedback, and demonstrates the application of IoT in secure access control and home automation.
      </p>
    </div>

    <!-- Middle Column: Servo Display -->
    <div class="servo-demo">
      <h1>Servo Motor Demo</h1>
      <div class="servo">
        <div class="needle" id="needle"></div>
      </div>
      <div class="status" id="status">Servo Default: 0°</div>
    </div>

    <!-- Right Column: Control Panel -->
    <div class="control-panel">
      <h2>Control Panel</h2>
      <div class="keypad">
        <button onclick="pressKey('1')">1</button>
        <button onclick="pressKey('2')">2</button>
        <button onclick="pressKey('3')">3</button>
        <button onclick="pressKey('4')">4</button>
        <button onclick="pressKey('5')">5</button>
        <button onclick="pressKey('6')">6</button>
        <button onclick="pressKey('7')">7</button>
        <button onclick="pressKey('8')">8</button>
        <button onclick="pressKey('9')">9</button>
        <button onclick="pressKey('0')">0</button>
      </div>
      <div class="actions">
        <button onclick="submitPassword()">Submit</button>
        <button onclick="clearPassword()">Clear</button>
      </div>
    </div>
  </div>

  <script>
    let password = "";

    function pressKey(key) {
      password += key;
      document.getElementById("status").textContent = "Password: " + password;
    }

    function submitPassword() {
      fetch(`/validate?password=${password}`)
        .then(response => response.text())
        .then(data => {
          document.getElementById("status").textContent = data;
          password = "";  // Reset password after submission
        });
    }

    function clearPassword() {
      password = "";
      document.getElementById("status").textContent = "Password cleared.";
    }

    // WebSocket for real-time servo angle updates
    const ws = new WebSocket(`ws://${window.location.hostname}:81/`);

    ws.onmessage = (event) => {
      const angle = parseInt(event.data);
      const needle = document.getElementById("needle");
      needle.style.transform = `rotate(${angle}deg)`;
      document.getElementById("status").textContent = `Servo Angle: ${angle}°`;
    };
  </script>
</body>
</html>

)=====";

// Function to validate the password and move the servo
void handleValidate() {
  if (server.hasArg("password")) {
    String inputPassword = server.arg("password");
    Serial.println("Received password: " + inputPassword);

    if (inputPassword == unlockPassword) {
      currentAngle = targetAngle;  // Move servo to 180 degrees
      myServo.write(targetAngle);
      notifyClients(currentAngle);
      server.send(200, "text/plain", "Unlocked! Servo moved to 180 degrees.");
      Serial.println("Servo moved to 180 degrees.");

      // Delay for 5 seconds, then reset the servo to locked position
      delay(5000);
      currentAngle = initialAngle;
      myServo.write(initialAngle);
      notifyClients(currentAngle);
      Serial.println("Servo returned to 0 degrees.");
    } else {
      server.send(200, "text/plain", "Incorrect password. Try again.");
      Serial.println("Incorrect password.");
    }
  } else {
    server.send(400, "text/plain", "Missing password parameter.");
    Serial.println("Missing password parameter.");
  }
}

// Notify WebSocket clients with the current servo angle
void notifyClients(int angle) {
  String message = String(angle);
  webSocket.broadcastTXT(message);
}

void setup() {
  // Start Serial Monitor
  Serial.begin(115200);

  // Attach the servo
  myServo.attach(servoPin);
  myServo.write(initialAngle);  // Initialize servo to 0 degrees
  currentAngle = initialAngle;

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Serve the main page
  server.on("/", []() {
    server.send_P(200, "text/html", MAIN_page);
  });

  // Handle password validation
  server.on("/validate", handleValidate);

  // Start the server and WebSocket
  server.begin();
  webSocket.begin();
  Serial.println("Web server and WebSocket started.");
}

void loop() {
  server.handleClient();  // Handle web server requests
  webSocket.loop();       // Handle WebSocket communication
}
