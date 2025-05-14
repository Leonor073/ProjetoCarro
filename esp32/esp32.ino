#include <Arduino.h>
#include <WiFi.h>
// Using updated LEDC API for ESP32

// Function prototypes
void moverFrente();
void moverTras();
void moverEsquerda();
void moverDireita();
void ajustarVelocidade(int novaVelocidade);

// AP credentials
const char* ssid = "ESP32_Server";
const char* password = "12345678";

// Initialize the server
WiFiServer server(80);
WiFiClient client;
bool clientConnected = false;

// Pinos do Motor A (Motor Esquerdo)
const int motorA_IN1 = 27;
const int motorA_IN2 = 26;
const int motorA_ENA = 14; // PWM para controle de velocidade

// Pinos do Motor B (Motor Direito)
const int motorB_IN3 = 25;
const int motorB_IN4 = 33;
const int motorB_ENB = 32; // PWM para controle de velocidade

// Configurações PWM
const int freq = 5000;
const int resolution = 8; // 8 bits de resolução (0-255)

// Velocidade padrão dos motores (0-255)
int velocidade = 200;

void setup() {
  Serial.begin(115200);
  delay (1000);
  Serial.println("Access Point Started");


  // Start the ESP32 as an access point
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.println("IP Address: " + WiFi.softAPIP().toString());
  
  // Start the server
  server.begin();
  Serial.println("Server is ready to accept clients...");

  // Configurar pinos como saída
  pinMode(motorA_IN1, OUTPUT);
  pinMode(motorA_IN2, OUTPUT);
  pinMode(motorB_IN3, OUTPUT);
  pinMode(motorB_IN4, OUTPUT);
  
  // Configure PWM on ENA and ENB pins using new ledcAttach API
  // ledcAttach combines the old ledcSetup and ledcAttachPin functions
  // Parameters: pin, frequency, resolution
  ledcAttach(motorA_ENA, freq, resolution);
  ledcAttach(motorB_ENB, freq, resolution);
}

void loop() {
  // If no client is connected, check for new connections
  if (!clientConnected) {
    client = server.available();
    
    if (client) {
      Serial.println("Client connected!");
      clientConnected = true;
      // Send initial greeting
      client.println("ESP32 server connected and ready for data");
    }
  }
  
  // If client is connected, process any incoming data
  if (clientConnected && client.connected()) {
    if (client.available()) {
      // Read data one byte at a time
      String command = client.readStringUntil('\n');
      command.trim();  // Remove whitespace
      Serial.println("Received: " + command);
      
      // Process commands
      if (command == "F") {
        Serial.println("Processing: Tilting up");
        client.println("Received tilt up command");
        moverFrente();
      } 
      else if (command == "T") {
        Serial.println("Processing: Tilting down");
        client.println("Received tilt down command");
        moverTras();
      }
      else if (command == "E") {
        Serial.println("Processing: Tilting left");
        client.println("Received tilt left command");
        moverEsquerda();
      }
      else if (command == "D") {
        Serial.println("Processing: Tilting right");
        client.println("Received tilt right command");
        moverDireita();
      }
      else if (command.startsWith("Hello")) {
        Serial.println("Received greeting");
        client.println("ESP32 acknowledges your greeting");
      }
      else if(command.startsWith("KeepAlive")){
        Serial.println("Received keep alive packege");
        client.println("ESP32 acknowledges connection");
      }
      else{
        Serial.println("Unknown command: " + command);
        client.println("Unknown command received");
      }
    }
  } else if (clientConnected) {
    // Client disconnected
    Serial.println("Client disconnected.");
    clientConnected = false;
    client.stop();
  }
  
  // Short delay
  delay(100);
}

void moverFrente() {
    // Motor Esquerdo - Frente
    digitalWrite(motorA_IN1, HIGH);
    digitalWrite(motorA_IN2, LOW);
    // Using ledcWrite with pin directly, not channel
    ledcWrite(motorA_ENA, velocidade);

    // Motor Direito - Frente
    digitalWrite(motorB_IN3, HIGH);
    digitalWrite(motorB_IN4, LOW);
    ledcWrite(motorB_ENB, velocidade);

    Serial.println("Frente");
} 

void moverTras() {
    // Motor Esquerdo - Trás
    digitalWrite(motorA_IN1, LOW);
    digitalWrite(motorA_IN2, HIGH);
    ledcWrite(motorA_ENA, velocidade);

    // Motor Direito - Trás
    digitalWrite(motorB_IN3, LOW);
    digitalWrite(motorB_IN4, HIGH);
    ledcWrite(motorB_ENB, velocidade);

    Serial.println("Trás");
}

void moverEsquerda() {
    // Motor Esquerdo - Trás (ou parado)
    digitalWrite(motorA_IN1, LOW);
    digitalWrite(motorA_IN2, HIGH);
    ledcWrite(motorA_ENA, velocidade);

    // Motor Direito - Frente
    digitalWrite(motorB_IN3, HIGH);
    digitalWrite(motorB_IN4, LOW);
    ledcWrite(motorB_ENB, velocidade);

    Serial.println("Esquerda");
}

void moverDireita() {
    // Motor Esquerdo - Frente
    digitalWrite(motorA_IN1, HIGH);
    digitalWrite(motorA_IN2, LOW);
    ledcWrite(motorA_ENA, velocidade);

    // Motor Direito - Trás (ou parado)
    digitalWrite(motorB_IN3, LOW);
    digitalWrite(motorB_IN4, HIGH);
    ledcWrite(motorB_ENB, velocidade);

    Serial.println("Direita");
}

void ajustarVelocidade(int novaVelocidade) {
    if (novaVelocidade >= 0 && novaVelocidade <= 255) {
        velocidade = novaVelocidade;
        Serial.print("Velocidade ajustada para: ");
        Serial.println(velocidade);
    }
}