#define BLYNK_TEMPLATE_ID "TMPL6ai8R7bEp"
#define BLYNK_TEMPLATE_NAME "CONTROL MOTOR"
#define BLYNK_AUTH_TOKEN "Yg8D5wc__9PVevvrF00lqmsjYSx0xjGZ"
#include <WiFi.h>
#include <HTTPClient.h>
#include <AccelStepper.h>
#include <BlynkSimpleEsp32.h>

// WiFi credentials
const char* ssid = "CHONG";
const char* password = "11031023";

// Google Sheets Script URL
const char* scriptUrl = "https://script.google.com/macros/s/AKfycbzGdT_CeJmtBzJ3OkT7oDzosU6aZxRryShMkttr0EjILd-Yx4du0zDg_p2WLByvc5C-/exec";

// Blynk Auth Token
char auth[] = "Yg8D5wc__9PVevvrF00lqmsjYSx0xjGZ";

// Stepper Motor Pins
#define X_STEP_PIN 25
#define X_DIR_PIN 26
#define Y_STEP_PIN 32
#define Y_DIR_PIN 33
#define Z_STEP_PIN 22
#define Z_DIR_PIN 23
#define ENABLE_PIN 21

AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);
AccelStepper stepperZ(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);

// Position matrix for 5 compartments
long positions[5][3] = {
  {500, 700, 1000},  // Position 1
  {3000, 2000, 0},  // Position 2
  {1000, 4000, 0},  // Position 3 
  {3000, 4000, 0},  // Position 4
  {2000, 3000, 1000} // Position 5
};

// Home position (0, 0, 0)
long homePosition[3] = {0, 0, 0};

// Barcode array mapped to compartments
String barcodes[5] = {
  "11031023",  // Barcode for compartment 1
  "12345678",  // Barcode for compartment 2
  "87654321",  // Barcode for compartment 3
  "88881234",  // Barcode for compartment 4
  "12348888"   // Barcode for compartment 5
};

String lastBarcode = "";

#define BARCODE_RX_PIN 16
#define BARCODE_TX_PIN 17

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

  // Initialize stepper motor properties
  stepperX.setMaxSpeed(10);
  stepperX.setAcceleration(0.1);
  stepperY.setMaxSpeed(500);
  stepperY.setAcceleration(500);
  stepperZ.setMaxSpeed(500);
  stepperZ.setAcceleration(500);

  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW);  // Enable stepper drivers
}

BLYNK_WRITE(V1) { moveToPosition(0);}
BLYNK_WRITE(V2) { moveToPosition(1);}
BLYNK_WRITE(V3) { moveToPosition(2); }
BLYNK_WRITE(V4) { moveToPosition(3); }
BLYNK_WRITE(V5) { moveToPosition(4); }

void loop() {
  Blynk.run();

  if (Serial1.available()) {
    lastBarcode = Serial1.readStringUntil('\n');
    lastBarcode.trim();
    Serial.println("Scanned Barcode: " + lastBarcode);
    handleScannedBarcode(lastBarcode);
    sendToGoogleSheets(lastBarcode);  // Send scanned barcode to Google Sheets
  }
}

void moveToPosition(int compartmentIndex) {
  // Move to the specified compartment
  stepperX.moveTo(positions[compartmentIndex][0]);
  stepperY.moveTo(positions[compartmentIndex][1]);
  stepperZ.moveTo(positions[compartmentIndex][2]);

  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0 || stepperZ.distanceToGo() != 0) {
   
    stepperY.run();
    stepperZ.run();
  }

  // Return to home position
  returnToHome();
}

void returnToHome() {
  stepperX.moveTo(homePosition[0]);
  stepperY.moveTo(homePosition[1]);
  stepperZ.moveTo(homePosition[2]);

  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0 || stepperZ.distanceToGo() != 0) {
    stepperX.run();
    stepperY.run();
    stepperZ.run();
  }
}

void handleScannedBarcode(String barcode) {
  for (int i = 0; i < 5; i++) {
    if (barcode == barcodes[i]) {
      Serial.println("Matching barcode found. Moving to compartment: " + String(i + 1));
      moveToPosition(i);
      return;
    }
  }
  Serial.println("No matching compartment for barcode: " + barcode);
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
