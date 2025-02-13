#define BLYNK_TEMPLATE_ID "TMPL6ai8R7bEp"
#define BLYNK_TEMPLATE_NAME "CONTROL MOTOR"
#define BLYNK_AUTH_TOKEN "Xf8nrw5l01QiS_9vDlCT-PhZPKkyOjOd"
#include <AccelStepper.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "CHONG";
const char* password = "11031023";

// Google Sheets script URL
const char* googleScriptURL = "https://docs.google.com/spreadsheets/d/1cwALWHoWuez3aZ283V7dg3UC9XXFQ7LrtQdHp9ADXJY/edit?gid=0#gid=0";

// Blynk Auth Token
char auth[] = "Xf8nrw5l01QiS_9vDlCT-PhZPKkyOjOd";

// Stepper Motor Pins (Assume GPIOs for each motor)
#define X_STEP_PIN 18
#define X_DIR_PIN 19
#define Y_STEP_PIN 21
#define Y_DIR_PIN 22
#define Z_STEP_PIN 23
#define Z_DIR_PIN 25
#define ENABLE_PIN 26 // Optional enable pin for all motors

// Create instances of AccelStepper for X, Y, Z motors
AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);
AccelStepper stepperZ(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);

// Setup positions for each compartment
long positions[5][3] = {
  {1000, 2000, 0}, // Position 1
  {3000, 2000, 0}, // Position 2
  {1000, 4000, 0}, // Position 3
  {3000, 4000, 0}, // Position 4
  {2000, 3000, 1000} // Position 5
};

// Barcode storage
String lastBarcode = "";

// Barcode scanner setup (depending on your hardware)
// For example, using Serial1 for communication
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

  // Setup stepper motors
  stepperX.setMaxSpeed(1000);
  stepperX.setAcceleration(500);
  stepperY.setMaxSpeed(1000);
  stepperY.setAcceleration(500);
  stepperZ.setMaxSpeed(1000);
  stepperZ.setAcceleration(500);

  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW); // Enable stepper drivers
}

// Blynk virtual pins to select compartments
BLYNK_WRITE(V1) { moveToPosition(0); } // Compartment 1
BLYNK_WRITE(V2) { moveToPosition(1); } // Compartment 2
BLYNK_WRITE(V3) { moveToPosition(2); } // Compartment 3
BLYNK_WRITE(V4) { moveToPosition(3); } // Compartment 4
BLYNK_WRITE(V5) { moveToPosition(4); } // Compartment 5

void loop() {
  Blynk.run();

  // Check for barcode data
  if (Serial1.available()) {
    lastBarcode = Serial1.readStringUntil('\n');
    sendBarcodeToGoogleSheets(lastBarcode);
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

void sendBarcodeToGoogleSheets(String barcode) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(googleScriptURL) + "?barcode=" + barcode;
    http.begin(url);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Barcode sent: " + response);
    } else {
      Serial.println("Error sending barcode");
    }
    http.end();
  }
}
