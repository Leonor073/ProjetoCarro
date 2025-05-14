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
unsigned int counter = 1;
unsigned int nPoints = 10; // Numero de pontos para a media
float xSum = 0;
float ySum = 0;
// Global client object
WiFiClient client;
bool serverConnected = false;

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 100; // Send data every 100ms

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
  
   connectToServer();
}

void connectToServer() {
  Serial.println("Connecting to server...");
  if (client.connect(serverIP, serverPort)) {
    Serial.println("Connected to ESP32 server!");
    serverConnected = true;
    
    // Send an initial greeting
    client.println("Hello from Nano 33 IoT!");
    
    // Wait for initial response
    unsigned long timeout = millis() + 2000;
    while (millis() < timeout) {
      if (client.available()) {
        String response = client.readStringUntil('\n');
        Serial.println("Server says: " + response);
        break;
      }
      delay(10);
    }
  } else {
    Serial.println("Failed to connect to server");
    serverConnected = false;
  }
}

void loop() {
     // Check if WiFi is still connected
     if (WiFi.status() != WL_CONNECTED) {
       Serial.println("WiFi connection lost. Reconnecting...");
       WiFi.begin(ssid, password);
       delay(1000);
       return;
     }

     // Check if server connection is still active
     if (!client.connected()) {
       Serial.println("Server connection lost. Reconnecting...");
       serverConnected = false;
       client.stop();
       connectToServer();
       return;
     }

    // Check for server messages
     if (client.available()) {
       String response = client.readStringUntil('\n');
       Serial.println("Server says: " + response);
     }

    // Send accelerometer data at regular intervals
    unsigned long currentTime = millis();
    if (currentTime - lastSendTime >= sendInterval) {
        lastSendTime = currentTime;
        
        // Read accelerometer data
        if (IMU.accelerationAvailable()) {
            IMU.readAcceleration(x, y, z);
            
            // Print accelerometer data to serial monitor
            Serial.print("Acceleration: x=");
            Serial.print(x);
            Serial.print(", y=");
            Serial.println(y);
            Serial.print(counter);
            Serial.println();

            if(counter == nPoints){
                x = xSum / nPoints;
                y = ySum / nPoints;
                Serial.print(x);
                Serial.print("   ");
                Serial.print(y);

                // Enviar os dados de x e y ao server
                // Only send data if there's a specific tilt direction
                bool dataSent = false;
                
                if (x > 0.5) {
                    x = 100 * x;
                    degreesX = map(x, 0, 97, 0, 90);
                    Serial.print("Tilting up ");
                    Serial.print(degreesX);
                    Serial.println(" degrees");
                    client.println("F");
                    dataSent = true;
                }
                
                if (x < -0.5) {
                    x = 100 * x;
                    degreesX = map(x, 0, -100, 0, 90);
                    Serial.print("Tilting down ");
                    Serial.print(degreesX);
                    Serial.println(" degrees");
                    client.println("T");
                    dataSent = true;
                }
                
                if (y > 0.5) {
                    y = 100 * y;
                    degreesY = map(y, 0, 97, 0, 90);
                    Serial.print("Tilting left ");
                    Serial.print(degreesY);
                    Serial.println(" degrees");
                    client.println("E");
                    dataSent = true;
                }
                
                if (y < -0.5) {
                    y = 100 * y;
                    degreesY = map(y, 0, -100, 0, 90);
                    Serial.print("Tilting right ");
                    Serial.print(degreesY);
                    Serial.println(" degrees");
                    client.println("D");
                    dataSent = true;
                }
                
                // Envio de uma mensagem para manter a coneccao aberta
                 if (!dataSent) {
                   client.println("KeepAlive\n");
                 }
                counter = 1;
                xSum = 0;
                ySum = 0;        
            }else{
                xSum += x; // xSum = xSum + x;
                ySum += y; // ySum = ySum + y;
                counter++;
            }
        }    
    }
    // Small delay to prevent CPU hogging
    delay(1000);
}