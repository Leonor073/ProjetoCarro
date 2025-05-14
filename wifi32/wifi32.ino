#include <WiFi.h>

// AP credentials
const char* ssid = "ESP32_Server";
const char* password = "12345678";

// Initialize the server
WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // Start the ESP32 as an access point
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.println("IP Address: " + WiFi.softAPIP().toString());

  // Start the server
  server.begin();
  Serial.println("Server is ready to accept clients...");
}

void loop() {
  // Check for client connections
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected!");

    while (client.connected()) {
      if (client.available()) {
        // Read the request
        String request = client.readStringUntil('\n');
        Serial.println("Received: " + request);

        // Respond to the client
        client.println("Hello from ESP32 server!");
      }
    }

    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
  }
}