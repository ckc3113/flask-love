#include <AccelStepper.h>

// Define A4988 driver pins (X, Y, and Z axes)
#define X_STEP_PIN 25      
#define X_DIR_PIN 26       
#define X_ENABLE_PIN 27    

#define Y_STEP_PIN 32      
#define Y_DIR_PIN 33       
#define Y_ENABLE_PIN 27    

#define Z_STEP_PIN 22      
#define Z_DIR_PIN 23       
#define Z_ENABLE_PIN 27    

// Define limit switch pins (X, Y, and Z axes, NO type)
#define X_LIMIT_SWITCH_PIN 15  
#define Y_LIMIT_SWITCH_PIN 14  
#define Z_LIMIT_SWITCH_PIN 13  

// Create stepper objects (X, Y, and Z axes)
AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);
AccelStepper stepperZ(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);

// Number of steps to move forward/backward
int stepSize = 1200;  // Same for all axes

// Flags to track if the motors should stop
bool stopX = false;
bool stopY = false;
bool stopZ = false;

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

  // Z axis setup
  pinMode(Z_ENABLE_PIN, OUTPUT);
  digitalWrite(Z_ENABLE_PIN, HIGH);  // Initially disable Z axis motor
  stepperZ.setMaxSpeed(1000);        
  stepperZ.setAcceleration(300);    

  // Limit switch setup (NO type)
  pinMode(X_LIMIT_SWITCH_PIN, INPUT_PULLUP);  // Use INPUT_PULLUP for NO switch (X axis)
  pinMode(Y_LIMIT_SWITCH_PIN, INPUT_PULLUP);  // Use INPUT_PULLUP for NO switch (Y axis)
  pinMode(Z_LIMIT_SWITCH_PIN, INPUT_PULLUP);  // Use INPUT_PULLUP for NO switch (Z axis)

  // Start serial monitor
  Serial.begin(115200);
}

void moveAxesTogether(int steps) {
  // Enable all X, Y, and Z motors
  Serial.println("Moving X, Y, and Z axes motors...");
  digitalWrite(X_ENABLE_PIN, LOW);  // Enable X motor
  digitalWrite(Y_ENABLE_PIN, LOW);  // Enable Y motor
  digitalWrite(Z_ENABLE_PIN, LOW);  // Enable Z motor

  // Set target positions for each axis
  stepperX.move(steps);
  stepperY.move(steps);
  stepperZ.move(steps);

  // Reset stop flags
  stopX = stopY = stopZ = false;

  // Keep the motors running and monitor limit switches
  while (!stopX || !stopY || !stopZ) {
    if (!stopX) {
      stepperX.run();  // Let X motor run
      if (digitalRead(X_LIMIT_SWITCH_PIN) == LOW) {
        Serial.println("Limit switch triggered on X axis, stopping X motor...");
        stepperX.stop();               // Stop X motor
        stepperX.setCurrentPosition(0); // Set X current position to zero
        stopX = true;                   // Mark X motor as stopped
      }
    }

    if (!stopY) {
      stepperY.run();  // Let Y motor run
      if (digitalRead(Y_LIMIT_SWITCH_PIN) == LOW) {
        Serial.println("Limit switch triggered on Y axis, stopping Y motor...");
        stepperY.stop();               // Stop Y motor
        stepperY.setCurrentPosition(0); // Set Y current position to zero
        stopY = true;                   // Mark Y motor as stopped
      }
    }

    if (!stopZ) {
      stepperZ.run();  // Let Z motor run
      if (digitalRead(Z_LIMIT_SWITCH_PIN) == LOW) {
        Serial.println("Limit switch triggered on Z axis, stopping Z motor...");
        stepperZ.stop();               // Stop Z motor
        stepperZ.setCurrentPosition(0); // Set Z current position to zero
        stopZ = true;                   // Mark Z motor as stopped
      }
    }
  }

  // Disable all motors after completing movement
  Serial.println("Disabling X, Y, and Z axis motors...");
  digitalWrite(X_ENABLE_PIN, HIGH);  // Disable X motor
  digitalWrite(Y_ENABLE_PIN, HIGH);  // Disable Y motor
  digitalWrite(Z_ENABLE_PIN, HIGH);  // Disable Z motor
}

void loop() {
  // Move X, Y, and Z axes forward together
  moveAxesTogether(1200);

  delay(1000);  // Wait for 1 second

  // Move X, Y, and Z axes backward together
  moveAxesTogether(-1200);

  delay(1000);  // Wait for 1 second
}
