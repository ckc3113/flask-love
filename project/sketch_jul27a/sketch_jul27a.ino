#include <Arduino.h>

// Define the pin connected to the IR sensor
const int irSensorPin = 14;
// Define the pin connected to the base of the transistor
const int motorControlPin = 18;

void setup() {
  // Initialize the serial communication
  Serial.begin(115200);

  // Set the IR sensor pin as input
  pinMode(irSensorPin, INPUT);

  // Set the motor control pin as output
  pinMode(motorControlPin, OUTPUT);

  // Initialize the motor state to off
  digitalWrite(motorControlPin, LOW);

  // Optional: Print a message to indicate the setup is complete
  Serial.println("IR Sensor and Motor Control Initialized");
}

void loop() {
  // Read the state of the IR sensor
  int sensorValue = digitalRead(irSensorPin);

  // Print the sensor value to the Serial Monitor
  Serial.print("IR Sensor Value: ");
  Serial.println(sensorValue);

  if (sensorValue == LOW) {
    // If the IR sensor detects an object, run the motor
    digitalWrite(motorControlPin, HIGH);
    Serial.println("Motor running...");
  } else {
    // If no object is detected, stop the motor
    digitalWrite(motorControlPin, LOW);
    Serial.println("Motor stopped");
  }

  // Optional: Add a delay to make the output readable
  delay(500);
}

