#define BLYNK_TEMPLATE_ID "TMPL6JAFXaLFj"
#define BLYNK_TEMPLATE_NAME "smart barcode shoe box"
#define BLYNK_AUTH_TOKEN "i7VMKp0gDyuI8-7Z5wU9s_l3dUz9YLyr"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <SpeedyStepper.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(16, 17); // RX, TX
String inputString = "";      
String DataScanner = "";     
bool stringComplete = false; 
int barcode_slot_1 = 0;
int barcode_slot_2 = 0; 
int scanner = 4;
int limit_x = 19;
int limit_y = 18;
int limit_z = 5;
int infrared_sensor = 35;
int button_start = 21;
int ledPin = 13;
char ssid[] = "CHONG";
char pass[] = "11031023";
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
int button_slot_1 = 0;
int button_slot_2 = 0;
int buttonState = 0;
int lastButtonState = 0;

BLYNK_WRITE(V4)// Button scanner
{
  int button = param.asInt();
  if(button == 1){
    digitalWrite(scanner, HIGH);
    delay(300);
  }else if(button == 0){
    digitalWrite(scanner, LOW); 
    delay(300); 
  }
} 

BLYNK_WRITE(V1)// Button slot 2
{
  int button = param.asInt();
  if(button == 1){
    button_slot_2 = 1;  
  }else if(button == 0){
    button_slot_2 = 0;   
  }
} 

BLYNK_WRITE(V0)// Button slot 1
{
  int button = param.asInt();
  if(button == 1){
    button_slot_1 = 1;  
  }else if(button == 0){
    button_slot_1 = 0;   
  }
} 

void setup() {
  Serial.begin(9600);  
  mySerial.begin(9600); 
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  Blynk.virtualWrite(V0, LOW);  //Button 1(slot 1)
  Blynk.virtualWrite(V1, LOW);  //Button 2(slot 2)
  Blynk.virtualWrite(V2, LOW);  //LED 1(slot 1)
  Blynk.virtualWrite(V3, LOW);  //LED 2(slot 2)
  Blynk.virtualWrite(V4, LOW);  //Button Scanner
  pinMode(scanner, OUTPUT);
  pinMode(limit_x, INPUT_PULLUP);
  pinMode(limit_y, INPUT_PULLUP);
  pinMode(limit_z, INPUT_PULLUP);
  pinMode(infrared_sensor, INPUT_PULLUP);
  pinMode(button_start, INPUT_PULLUP);
  digitalWrite(scanner, LOW); 
  stepperX.connectToPins(MOTOR_X_STEP_PIN, MOTOR_X_DIR_PIN);
  stepperY.connectToPins(MOTOR_Y_STEP_PIN, MOTOR_Y_DIR_PIN);  
  stepperZ.connectToPins(MOTOR_Z_STEP_PIN, MOTOR_Z_DIR_PIN); 
}

void loop() {
buttonState = (digitalRead(button_start) == LOW);
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      return_to_home();
    }
  }
  lastButtonState = buttonState; 
  
//   Infrared sensor detection and LED control
  buttonState = (digitalRead(infrared_sensor) == LOW);
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      digitalWrite(scanner, HIGH);   // Turn on the scanner
      digitalWrite(ledPin, HIGH);    // Turn on the LED when object detected
      delay(30);
      digitalWrite(scanner, LOW);    // Turn off the scanner
      delay(30);
    } else {
      digitalWrite(ledPin, LOW);     // Turn off the LED when no object detected
    }
  }
  
  lastButtonState = buttonState; 
  Blynk.run();
  barcode();
  move_in();
  move_out();
  led_blynk();
}

void move_out(){
  if(barcode_slot_1 == 2 && button_slot_1 == 1){
    position_slot_1_out();            //goto position slot 1(collect the shoe)
    return_to_home();             //send the shoe to home position, from slot 1
    barcode_slot_1 = 0;
    button_slot_1 = 0;
  }else if(barcode_slot_2 == 2 && button_slot_2 == 1){
    position_slot_2_out();            //goto position slot 2(collect the shoe)
    return_to_home();             //send the shoe to home position, from slot 2
    barcode_slot_2 = 0; 
    button_slot_1 = 0;
  }  
}

void move_in(){
  if(barcode_slot_1 == 1){
    position_slot_1_in();            //collect the shoe from home position, send the shoe to slot 1
    return_to_home();         //return to home position from slot 1
    barcode_slot_1 = 2;
    button_slot_1 = 0;
  }else if(barcode_slot_2 == 1){
    position_slot_2_in();            //collect the shoe from home position, send the shoe to slot 2
    return_to_home();         //return to home position from slot 2
    barcode_slot_2 = 2; 
    button_slot_2 = 0;
  }  
}

void return_to_home(){
  do{
      stepperZ.setSpeedInStepsPerSecond(100);
      stepperZ.setAccelerationInStepsPerSecondPerSecond(100);
      stepperZ.moveRelativeInSteps(-50); 
    } while (digitalRead(limit_z) == HIGH);
  do{
      stepperY.setSpeedInStepsPerSecond(100);
      stepperY.setAccelerationInStepsPerSecondPerSecond(100);
      stepperY.moveRelativeInSteps(-50);
    } while (digitalRead(limit_y) == HIGH);
  do{
        stepperX.setSpeedInStepsPerSecond(100);
        stepperX.setAccelerationInStepsPerSecondPerSecond(100);
        stepperX.moveRelativeInSteps(-50);
      } while (digitalRead(limit_x) == HIGH);
}

void position_slot_1_in(){
  for (int i = 0; i <= 1; i++) {
      runIncreaseZStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
      runIncreaseYStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
      runDecreaseZStepper();
      }  
      delay(2000);   
  for (int i = 0; i <= 1; i++) {
      runIncreaseXStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 5; i++) {
        runIncreaseYStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
      runIncreaseZStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
      runDecreaseYStepper();
      }  
      delay(2000); 
}
void position_slot_1_out(){
  for (int i = 0; i <= 1; i++) {
      runIncreaseXStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 5; i++) {
        runIncreaseYStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
        runIncreaseZStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
        runIncreaseYStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
      runDecreaseZStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 5; i++) {
        runDecreaseYStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
      runDecreaseXStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
        runIncreaseZStepper();
      }  
      delay(2000);  
  for (int i = 0; i <= 1; i++) {
        runDecreaseYStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
      runDecreaseZStepper();
      }  
      delay(2000);
}

void position_slot_2_in(){
for (int i = 0; i <= 1; i++) {
      runIncreaseZStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
      runIncreaseYStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
      runDecreaseZStepper();
      }  
      delay(2000);   
  for (int i = 0; i <= 1; i++) {
      runIncreaseXStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 10; i++) {
        runIncreaseYStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
      runIncreaseZStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
      runDecreaseYStepper();
      }  
      delay(2000); 
}

void position_slot_2_out(){
 for (int i = 0; i <= 1; i++) {
      runIncreaseXStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 5; i++) {
        runIncreaseYStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
        runIncreaseZStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
        runIncreaseYStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
      runDecreaseZStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 10; i++) {
        runDecreaseYStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
      runDecreaseXStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
        runIncreaseZStepper();
      }  
      delay(2000);  
  for (int i = 0; i <= 1; i++) {
        runDecreaseYStepper();
      }  
      delay(2000);
  for (int i = 0; i <= 1; i++) {
      runDecreaseZStepper();
      }  
      delay(2000);
}

void barcode(){
if (mySerial.available()) // Check if there is Incoming Data in the Serial Buffer.
  {
    while (mySerial.available()) // Keep reading Byte by Byte from the Buffer till the Buffer is empty
    {
      char inChar = mySerial.read(); // Read 1 Byte of data and store it in a character variable
      inputString += inChar;
      if (inChar == '\n') {
        stringComplete = true;
        DataScanner = String(inputString);
      }
    }
  }
  if (stringComplete) {
    
       if (DataScanner == "88881234\r\n"){
          barcode_slot_1 = 1;
          Serial.println("sLOT 1:");
          Serial.println(DataScanner);
       }else if(DataScanner == "11031023\r\n"){ 
          barcode_slot_2 = 1;
          Serial.println("sLOT 2:");
          Serial.println(DataScanner);
       }else{
          Serial.println("No Data / INVALID"); 
       }
    inputString = "";
    DataScanner = "";
    stringComplete = false;
  }
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
}
void runIncreaseXStepper() 
{
  stepperX.setSpeedInStepsPerSecond(motorSpeed);
  stepperX.setAccelerationInStepsPerSecondPerSecond(motorAcceleration);
  stepperX.moveRelativeInSteps(250);
}

void runDecreaseXStepper() 
{
  stepperX.setSpeedInStepsPerSecond(motorSpeed);
  stepperX.setAccelerationInStepsPerSecondPerSecond(motorAcceleration);
  stepperX.moveRelativeInSteps(-250);
}

void runIncreaseYStepper() 
{
  stepperY.setSpeedInStepsPerSecond(motorSpeed);
  stepperY.setAccelerationInStepsPerSecondPerSecond(motorAcceleration);
  stepperY.moveRelativeInSteps(600);
}

void runDecreaseYStepper() 
{
  stepperY.setSpeedInStepsPerSecond(motorSpeed);
  stepperY.setAccelerationInStepsPerSecondPerSecond(motorAcceleration);
  stepperY.moveRelativeInSteps(-600);
}

void runIncreaseZStepper() 
{
  stepperZ.setSpeedInStepsPerSecond(motorSpeed);
  stepperZ.setAccelerationInStepsPerSecondPerSecond(motorAcceleration);
  stepperZ.moveRelativeInSteps(1300);
}

void runDecreaseZStepper() 
{
  stepperZ.setSpeedInStepsPerSecond(motorSpeed);
  stepperZ.setAccelerationInStepsPerSecondPerSecond(motorAcceleration);
  stepperZ.moveRelativeInSteps(-1300);
}

void led_blynk(){
  if(barcode_slot_1 == 2){
    Blynk.virtualWrite(V2, HIGH);  //LED 1(slot 1)  
  }else if(barcode_slot_1 == 0){
    Blynk.virtualWrite(V2, LOW);  //LED 1(slot 1)  
  }
  if(barcode_slot_2 == 2){
    Blynk.virtualWrite(V3, HIGH);  //LED 2(slot 2)  
  }else if(barcode_slot_2 == 0){
    Blynk.virtualWrite(V3, LOW);  //LED 2(slot 2)  
  }
}
