#include <AccelStepper.h>

// Define A4988 driver pins (X and Y axes)
#define X_STEP_PIN 25      
#define X_DIR_PIN 26       
#define X_ENABLE_PIN 27    

#define Y_STEP_PIN 32      
#define Y_DIR_PIN 33       
#define Y_ENABLE_PIN 27    

// Define limit switch pins (X and Y axes, NO type)
#define X_LIMIT_SWITCH_PIN 15  
#define Y_LIMIT_SWITCH_PIN 14  

// Create stepper objects (X and Y axes)
AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);

// Number of steps to move forward/backward
int stepSize = 1200;  // Same for both X and Y axes

void setup() {
  // X axis setup
  pinMode(X_ENABLE_PIN, OUTPUT);
  digitalWrite(X_ENABLE_PIN, HIGH);  // Initially disable X axis motor
  stepperX.setMaxSpeed(800);        
  stepperX.setAcceleration(300);    

  // Y axis setup
  pinMode(Y_ENABLE_PIN, OUTPUT);
  digitalWrite(Y_ENABLE_PIN, HIGH);  // Initially disable Y axis motor
  stepperY.setMaxSpeed(1200);        
  stepperY.setAcceleration(300);    

  // Limit switch setup (NO type)
  pinMode(X_LIMIT_SWITCH_PIN, INPUT_PULLUP);  // Use INPUT_PULLUP for NO switch (X axis)
  pinMode(Y_LIMIT_SWITCH_PIN, INPUT_PULLUP);  // Use INPUT_PULLUP for NO switch (Y axis)

  // Start serial monitor
  Serial.begin(115200);
}

void moveAxesTogether(int steps) {
  // Enable both X and Y motors
  Serial.println("Moving X and Y axes motors forward...");
  digitalWrite(X_ENABLE_PIN, LOW);  // Enable X motor
  digitalWrite(Y_ENABLE_PIN, LOW);  // Enable Y motor

  stepperX.move(steps);  // Move X forward
  stepperY.move(steps);  // Move Y forward

  // Keep the motors running and monitor both limit switches
  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0) {
    stepperX.run();  // Let X motor run
    stepperY.run();  // Let Y motor run

    // Check if limit switch (NO) is triggered for X axis
    if (digitalRead(X_LIMIT_SWITCH_PIN) == LOW) {
      Serial.println("Limit switch triggered on X axis, stopping X motor...");
      stepperX.stop();               // Stop X motor
      stepperX.setCurrentPosition(0); // Set X current position to zero
    }

    // Check if limit switch (NO) is triggered for Y axis
    if (digitalRead(Y_LIMIT_SWITCH_PIN) == LOW) {
      Serial.println("Limit switch triggered on Y axis, stopping Y motor...");
      stepperY.stop();               // Stop Y motor
      stepperY.setCurrentPosition(0); // Set Y current position to zero
    }
  }

  // Disable both motors after completing movement
  Serial.println("Disabling X and Y axis motors...");
  digitalWrite(X_ENABLE_PIN, HIGH);  // Disable X motor
  digitalWrite(Y_ENABLE_PIN, HIGH);  // Disable Y motor
}

void loop() {
  // Move X and Y axes forward together
  moveAxesTogether(1200);

  delay(1000);  // Wait for 1 second

  // Move X and Y axes backward together
  moveAxesTogether(-1200);

  delay(1000);  // Wait for 1 second
}
