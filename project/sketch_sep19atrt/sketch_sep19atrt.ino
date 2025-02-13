// Define pin connections & motor's steps per revolution
const int dirPin = 16; // Change to ESP32 GPIO pin
const int stepPin = 17; // Change to ESP32 GPIO pin
const int stepsPerRevolution = 500;

void setup()
{
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}

void loop()
{
  // Set motor direction clockwise
  digitalWrite(dirPin, HIGH);

  // Spin motor slowly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000); // Adjust timing if needed
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
  delay(1000); // Wait a second
}
