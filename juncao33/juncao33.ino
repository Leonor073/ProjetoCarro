#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>

// Global variables for accelerometer
float x, y, z;
int degreesX = 0;
int degreesY = 0;

// WiFi credentials
const char* ssid = "ESP32_Server";
const char* password = "12345678";
const char* serverIP = "192.168.4.1";
const uint16_t serverPort = 80;

// Global client object
WiFiClient client;
bool clientConnected = false;

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial connection
  
  // Initialize accelerometer
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  
  // Connect to ESP32 AP
  Serial.println("Connecting to ESP32 Access Point...");
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to ESP32!");
  Serial.println("Nano 33 IoT IP: " + WiFi.localIP().toString());
  
  // Connect to server once
  connectToServer();
}

void connectToServer() {
  // Attempt to connect to the server
  Serial.println("Connecting to server...");
  if (client.connect(serverIP, serverPort)) {
    Serial.println("Connected to ESP32 server!");
    clientConnected = true;
  } else {
    Serial.println("Failed to connect to server");
    clientConnected = false;
  }
}

void loop() {
  // Check if connection is still active, reconnect if needed
  if (!client.connected()) {
    Serial.println("Connection lost. Reconnecting...");
    clientConnected = false;
    connectToServer();
  }
  
  // Only proceed if we have a valid connection
  if (clientConnected) {
    // Read accelerometer data
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x, y, z);
      
      // Print accelerometer data to serial monitor
      Serial.print("Acceleration: x=");
      Serial.print(x);
      Serial.print(", y=");
      Serial.print(y);
      Serial.print(", z=");
      Serial.println(z);
      
      // Send accelerometer data based on readings
      if (x > 0.5) {
        x = 100 * x;
        degreesX = map(x, 0, 97, 0, 90);
        Serial.print("Tilting up ");
        Serial.print(degreesX);
        Serial.println(" degrees");
        client.println("F");
      }
      
      if (x < -0.5) {
        x = 100 * x;
        degreesX = map(x, 0, -100, 0, 90);
        Serial.print("Tilting down ");
        Serial.print(degreesX);
        Serial.println(" degrees");
        client.println("T");
      }
      
      if (y > 0.5) {
        y = 100 * y;
        degreesY = map(y, 0, 97, 0, 90);
        Serial.print("Tilting left ");
        Serial.print(degreesY);
        Serial.println(" degrees");
        client.println("E");
      }
      
      if (y < -0.5) {
        y = 100 * y;
        degreesY = map(y, 0, -100, 0, 90);
        Serial.print("Tilting right ");
        Serial.print(degreesY);
        Serial.println(" degrees");
        client.println("D");
      }
      
      // Check for server response
      unsigned long timeout = millis() + 100; // Short timeout
      while (client.connected() && millis() < timeout) {
        if (client.available()) {
          String response = client.readStringUntil('\n');
          Serial.println("Response from server: " + response);
          break;
        }
      }
    }
  }
  
  // Short delay to prevent flooding the server
  delay(100);
}