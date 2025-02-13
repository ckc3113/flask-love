#include <AccelStepper.h>

// Define A4988 driver pins (X axis)
#define X_STEP_PIN 25      
#define X_DIR_PIN 26       
#define X_ENABLE_PIN 27    

// Define limit switch pin (X axis, NO type)
#define X_LIMIT_SWITCH_PIN 15  

// Create stepper object (X axis)
AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);

// Number of steps to move forward/backward
int stepSize = 1200;  

void setup() {
  // X axis setup
  pinMode(X_ENABLE_PIN, OUTPUT);
  digitalWrite(X_ENABLE_PIN, HIGH);  // Initially disable X axis motor
  stepperX.setMaxSpeed(1000);        
  stepperX.setAcceleration(300);    

  // Limit switch setup (NO type)
  pinMode(X_LIMIT_SWITCH_PIN, INPUT_PULLUP);  // Use INPUT_PULLUP for NO switch

  // Start serial monitor
  Serial.begin(115200);
}

void loop() {
  // Enable motor and move forward 20 steps
  Serial.println("Moving X axis motor forward...");
  digitalWrite(X_ENABLE_PIN, LOW);  // Enable motor
  stepperX.move(1200);  // Move forward 20 steps

  // Keep the motor running and monitor the limit switch
  while (stepperX.distanceToGo() != 0) {
    stepperX.run();  // Let motor run

    // Check if limit switch (NO) is triggered
    if (digitalRead(X_LIMIT_SWITCH_PIN) == LOW) {
      Serial.println("Limit switch triggered during forward movement, stopping motor...");
      stepperX.stop();               // Stop motor
      stepperX.setCurrentPosition(0); // Set current position to zero
      break;  // Exit loop
    }
  }

  // Move backward 20 steps after a delay
  delay(1000);  // Wait for 1 second
  Serial.println("Moving X axis motor backward...");
  stepperX.move(-1200);  // Move backward 20 steps

  // Keep the motor running and monitor the limit switch
  while (stepperX.distanceToGo() != 0) {
    stepperX.run();  // Let motor run

    // Check if limit switch (NO) is triggered
    if (digitalRead(X_LIMIT_SWITCH_PIN) == LOW) {
      Serial.println("Limit switch triggered during backward movement, stopping motor...");
      stepperX.stop();               // Stop motor
      stepperX.setCurrentPosition(0); // Set current position to zero
      break;  // Exit loop
    }
  }

  // Disable motor after completing movement
  Serial.println("Disabling X axis motor...");
  digitalWrite(X_ENABLE_PIN, HIGH);  // Disable motor

  delay(1000);  // Wait before repeating the loop
}
