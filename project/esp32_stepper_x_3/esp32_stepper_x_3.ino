#include <SpeedyStepper.h>
int limit_x = 19;
int limit_y = 18;
int limit_z = 5;
int button_start = 21;

const int MOTOR_X_STEP_PIN = 26;
const int MOTOR_Y_STEP_PIN = 25;
const int MOTOR_Z_STEP_PIN = 33;

const int MOTOR_X_DIR_PIN = 32;
const int MOTOR_Y_DIR_PIN = 14;
const int MOTOR_Z_DIR_PIN = 27;

SpeedyStepper stepperX;
SpeedyStepper stepperY;
SpeedyStepper stepperZ;

int motorAcceleration = 100;
int motorSpeed = 100;
int xCounter = 0;
int yCounter = 0;
int zCounter = 0;
int buttonState = 0;
int lastButtonState = 0;
int initialize = 1;


void setup() {
  Serial.begin(9600);
  pinMode(limit_x, INPUT_PULLUP);
  pinMode(limit_y, INPUT_PULLUP);
  pinMode(limit_z, INPUT_PULLUP);
  pinMode(button_start, INPUT_PULLUP);
  stepperX.connectToPins(MOTOR_X_STEP_PIN, MOTOR_X_DIR_PIN);
  stepperY.connectToPins(MOTOR_Y_STEP_PIN, MOTOR_Y_DIR_PIN);  
  stepperZ.connectToPins(MOTOR_Z_STEP_PIN, MOTOR_Z_DIR_PIN); 
}

void loop() {
  while(initialize){
    homeAll();  
  }
  buttonState = (digitalRead(button_start) == LOW);
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      slot_1();
    }
  }
  lastButtonState = buttonState; 
}

void slot_1(){
  for (int i = 0; i <= 10; i++) {
        runIncreaseZStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 10; i++) {
        runIncreaseYStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 10; i++) {
      runDecreaseZStepper();
      }  
      delay(2000);   
  for (int i = 0; i <= 5; i++) {
      runIncreaseXStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 10; i++) {
      runIncreaseYStepper();
      }  
      delay(2000);  
  for (int i = 0; i <= 10; i++) {
      runIncreaseZStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 10; i++) {
      runDecreaseYStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 10; i++) {
      runDecreaseZStepper();
      }  
      delay(2000); 
      homeAll();  
}

void homeAll(){
  do{
      stepperX.setSpeedInStepsPerSecond(100);
      stepperX.setAccelerationInStepsPerSecondPerSecond(100);
      stepperX.moveRelativeInSteps(-50);
    } while (digitalRead(limit_x) == HIGH);
  do{
      stepperY.setSpeedInStepsPerSecond(100);
      stepperY.setAccelerationInStepsPerSecondPerSecond(100);
      stepperY.moveRelativeInSteps(-50);
    } while (digitalRead(limit_y) == HIGH);
  do{
      stepperZ.setSpeedInStepsPerSecond(100);
      stepperZ.setAccelerationInStepsPerSecondPerSecond(100);
      stepperZ.moveRelativeInSteps(-50); 
    } while (digitalRead(limit_z) == HIGH);
  initialize = 0; 
}
void runIncreaseXStepper() 
{
  stepperX.setSpeedInStepsPerSecond(motorSpeed);
  stepperX.setAccelerationInStepsPerSecondPerSecond(motorAcceleration);
  stepperX.moveRelativeInSteps(50);
}

void runDecreaseXStepper() 
{
  stepperX.setSpeedInStepsPerSecond(motorSpeed);
  stepperX.setAccelerationInStepsPerSecondPerSecond(motorAcceleration);
  stepperX.moveRelativeInSteps(-50);
}

void runIncreaseYStepper() 
{
  stepperY.setSpeedInStepsPerSecond(motorSpeed);
  stepperY.setAccelerationInStepsPerSecondPerSecond(motorAcceleration);
  stepperY.moveRelativeInSteps(50);
}

void runDecreaseYStepper() 
{
  stepperY.setSpeedInStepsPerSecond(motorSpeed);
  stepperY.setAccelerationInStepsPerSecondPerSecond(motorAcceleration);
  stepperY.moveRelativeInSteps(-50);
}

void runIncreaseZStepper() 
{
  stepperZ.setSpeedInStepsPerSecond(motorSpeed);
  stepperZ.setAccelerationInStepsPerSecondPerSecond(motorAcceleration);
  stepperZ.moveRelativeInSteps(50);
}

void runDecreaseZStepper() 
{
  stepperZ.setSpeedInStepsPerSecond(motorSpeed);
  stepperZ.setAccelerationInStepsPerSecondPerSecond(motorAcceleration);
  stepperZ.moveRelativeInSteps(-50);
}
