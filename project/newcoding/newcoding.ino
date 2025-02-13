#define BLYNK_TEMPLATE_ID "TMPL6ai8R7bEp"
#define BLYNK_TEMPLATE_NAME "CONTROL MOTOR"
#define BLYNK_AUTH_TOKEN "Yg8D5wc__9PVevvrF00lqmsjYSx0xjGZ"
#include <WiFi.h>
#include <HTTPClient.h>
#include <AccelStepper.h>
#include <BlynkSimpleEsp32.h>

// WiFi credentials
const char* ssid = "fastconsb 2.4G@unifi";
const char* password = "91781978";

// Google Sheets Script URL
const char* scriptUrl = "https://script.google.com/macros/s/AKfycbznoZC1fb5cUoq7RRuvz9Y_E9as5XmP72LfV-AnORuyR4syA0eUfG0FCj1Rp8G_k8Eq/exec";

// Blynk Auth Token
char auth[] = "Yg8D5wc__9PVevvrF00lqmsjYSx0xjGZ";

// Stepper Motor Pins
#define X_STEP_PIN 18
#define X_DIR_PIN 19
#define Y_STEP_PIN 21
#define Y_DIR_PIN 22
#define Z_STEP_PIN 23
#define Z_DIR_PIN 25
#define ENABLE_PIN 26

AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);
AccelStepper stepperZ(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);

long positions[5][3] = {
  {1000, 2000, 0},
  {3000, 2000, 0},
  {1000, 4000, 0},
  {3000, 4000, 0},
  {2000, 3000, 1000}
};

String lastBarcode = "";

#define BARCODE_RX_PIN 03
#define BARCODE_TX_PIN 01

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, BARCODE_RX_PIN, BARCODE_TX_PIN);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  Blynk.begin(auth, ssid, password);

  stepperX.setMaxSpeed(1000);
  stepperX.setAcceleration(500);
  stepperY.setMaxSpeed(1000);
  stepperY.setAcceleration(500);
  stepperZ.setMaxSpeed(1000);
  stepperZ.setAcceleration(500);

  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW);
}

BLYNK_WRITE(V1) { moveToPosition(0); }
BLYNK_WRITE(V2) { moveToPosition(1); }
BLYNK_WRITE(V3) { moveToPosition(2); }
BLYNK_WRITE(V4) { moveToPosition(3); }
BLYNK_WRITE(V5) { moveToPosition(4); }

void loop() {
  Blynk.run();

  if (Serial1.available()) {
    lastBarcode = Serial1.readStringUntil('\n');
    Serial.println("Scanned Barcode: " + lastBarcode);
    sendToGoogleSheets(lastBarcode);
  }
}

void moveToPosition(int compartmentIndex) {
  stepperX.moveTo(positions[compartmentIndex][0]);
  stepperY.moveTo(positions[compartmentIndex][1]);
  stepperZ.moveTo(positions[compartmentIndex][2]);

  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0 || stepperZ.distanceToGo() != 0) {
    stepperX.run();
    stepperY.run();
    stepperZ.run();
  }
}

void sendToGoogleSheets(String barcode) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(scriptUrl);
    http.addHeader("Content-Type", "application/json");

    // Trim and escape special characters
    barcode.trim();
    barcode.replace("\\", "\\\\");  // Escape backslashes
    barcode.replace("\"", "\\\"");  // Escape double quotes

    String jsonData = "{\"barcode\":\"" + barcode + "\"}";
    Serial.println("Sending JSON: " + jsonData);

    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      Serial.printf("HTTP Response code: %d\n", httpResponseCode);
      String response = http.getString();
      //Serial.println("Google Script Response: " + response);
    } else {
      Serial.printf("Error code: %d\n", httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}
