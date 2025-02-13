// Define pins for A4988 and relay
#define STEP_PIN 25      // Pin for step signal
#define DIR_PIN 26       // Pin for direction signal
#define ENABLE_PIN 27    // Pin to enable/disable stepper (optional)
#define RELAY_PIN 12     // Pin to control relay

// Stepper motor parameters
int stepsPerRevolution = 200;  // Change based on your stepper motor

void setup() {
  // Set pins as output
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  
  // Start with stepper disabled
  digitalWrite(ENABLE_PIN, HIGH);
  
  // Turn off relay (cut off motor power supply)
  digitalWrite(RELAY_PIN, LOW);
  
  // Start serial monitor
  Serial.begin(115200);
}

void loop() {
  // Simulate relay control for motor power
  Serial.println("Turning on relay (motor power on)...");
  digitalWrite(RELAY_PIN, HIGH);  // Power on stepper motor via relay
  delay(1000);  // Wait for 1 second
  
  // Enable the stepper driver
  digitalWrite(ENABLE_PIN, LOW);  // Enable the motor
  
  // Rotate stepper motor clockwise
  digitalWrite(DIR_PIN, HIGH);  // Set direction
  for (int i = 0; i < stepsPerRevolution; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(5000);  // Control speed
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(5000);
  }

  delay(500);  // Wait for 1 second

  // Rotate stepper motor counterclockwise
  Serial.println("Rotating motor counterclockwise...");
  digitalWrite(DIR_PIN, LOW);  // Change direction
  for (int i = 0; i < stepsPerRevolution; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(5000);  // Control speed
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(5000);
  }

  // Disable the stepper driver and turn off the relay
  Serial.println("Disabling motor and turning off relay...");
  digitalWrite(ENABLE_PIN, HIGH);  // Disable the motor
  digitalWrite(RELAY_PIN, LOW);    // Power off motor via relay
  
  delay(1000);  // Wait for 5 seconds before repeating the loop
}
