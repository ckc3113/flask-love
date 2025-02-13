#include <MultiStepper.h>
#include <AccelStepper.h>

//--------- Flag structure --------------------------------------
typedef struct _vFlag
{
  uint8_t LEDFlag = 1;
} vFlag;
vFlag *flag_Ptr;
vFlag flag;

//------LED------------------
#define LED_BUILTIN 2
char ctemp[20];

//--------- uart structure --------------------------------------
#define LINE_BUFFER_LENGTH 64
typedef struct _vUart
{
  char c;
  int lineIndex = 0;
  int line1Index = 0;
  int BTlineIndex = 0;
  bool lineIsComment;
  bool lineSemiColon;
  char line[128];
  char BTline[20];
  String inputString;
  String BTinputString;
  int V[16];
  char ctemp[30];
  char I2C_Data[80];
  int DC_Speed = 50;
  int sensorValue = 0;
} vUart;
vUart *Uart_Ptr;
vUart Uart;

//--------- motor structure --------------------------------------
typedef struct _axis_state_t
{
  uint8_t pulPin[6];
  uint8_t dirPin[6];
  uint8_t enaPin[6];
  long RotatorAngle[6];
  boolean AXISFlag[6];
  uint8_t HomeFlag[6];
  uint8_t RotatorFlag[6];
  float FeedRate[6];
  float MaxSpeed[6];
  float Acceleration[6];
  int AXISNumber[2];
} axis_state_t;
axis_state_t *gc_Ptr;
axis_state_t gc;

//----------multi motor ---------------------------------------
MultiStepper steppers;
long positions[3];  // Positions for X, Y, Z axis
AccelStepper stepperX(1, 14, 12);  // X axis motor
AccelStepper stepperY(1, 32, 33);  // Y axis motor
AccelStepper stepperZ(1, 16, 17);  // Z axis motor

//------------------------------------------------------------------------------
// Task handles
TaskHandle_t huart;
TaskHandle_t hfunction;
TaskHandle_t hmultimotor;

void MultiMotorTask(void *pvParameters);
void vUARTTask(void *pvParameters);

//------------------------------------------------------------------------------
// Initialization function
void initial()
{
  Serial.println(F("Create Task"));

  // UART task creation
  xTaskCreatePinnedToCore(
      vUARTTask, "UARTTask", 1024, NULL, 3, &huart, 0);

  // Initialize motor task
  initMultiMotorTask();
}

//------------------------------------------------------------------------------
// Initialize the motor control task
void initMultiMotorTask(void)
{
  xTaskCreatePinnedToCore(
      MultiMotorTask, "MultiMotorTask", 1024, NULL, 3, &hmultimotor, 1);

  if (hmultimotor == NULL)
  {
    Serial.println("Create MultiMotor task failed");
  }
  else
  {
    Serial.println("MultiMotor task up and running");
    vTaskSuspend(hmultimotor);
  }
}

//------------------------------------------------------------------------------
// MultiMotor task to control stepper motors
void MultiMotorTask(void *pvParameters)
{
  (void)pvParameters;

  while (1)
  {
    if (gc.AXISFlag[0] || gc.AXISFlag[1] || gc.AXISFlag[2])
    {
      if (gc.RotatorFlag[0] == 1)
      {
        gc.RotatorAngle[0] = gc.RotatorAngle[0] * 500;
        positions[0] = gc.RotatorAngle[0];
      }
      else
      {
        positions[0] = stepperX.currentPosition();
      }

      if (gc.RotatorFlag[1] == 1)
      {
        gc.RotatorAngle[1] = gc.RotatorAngle[1] * 500;
        positions[1] = gc.RotatorAngle[1];
      }
      else
      {
        positions[1] = stepperY.currentPosition();
      }

      if (gc.RotatorFlag[2] == 1)
      {
        gc.RotatorAngle[2] = gc.RotatorAngle[2] * 100;
        positions[2] = gc.RotatorAngle[2];
      }
      else
      {
        positions[2] = stepperZ.currentPosition();
      }

      // Move motors to the target positions
      steppers.moveTo(positions);
      steppers.runSpeedToPosition();

      // Check for completion of movement
      if (abs(stepperX.currentPosition()) == abs(gc.RotatorAngle[0])) gc.AXISFlag[0] = false;
      if (abs(stepperY.currentPosition()) == abs(gc.RotatorAngle[1])) gc.AXISFlag[1] = false;
      if (abs(stepperZ.currentPosition()) == abs(gc.RotatorAngle[2])) gc.AXISFlag[2] = false;

      // Suspend the task if all movements are done
      if (gc.AXISNumber[0] == 0) vTaskSuspend(hmultimotor);
    }
    vTaskDelay(1);
  }
}

//------------------------------------------------------------------------------
// Initialize multi-motor settings
void initMultiMotorSetup(void)
{
  // Pin configuration
  gc.pulPin[0] = 26;
  gc.dirPin[0] = 25;
  gc.pulPin[1] = 32;
  gc.dirPin[1] = 33;
  gc.pulPin[2] = 34;
  gc.dirPin[2] = 35;

  // Motor default settings
  for (int i = 0; i < 6; i++)
  {
    gc.RotatorAngle[i] = 0;
    gc.AXISFlag[i] = false;
    gc.HomeFlag[i] = 0;
    gc.RotatorFlag[i] = 0;
    gc.FeedRate[i] = 1000;
    gc.MaxSpeed[i] = 4000;
    gc.Acceleration[i] = 800;
  }
  gc.AXISNumber[0] = 0;

  // Configure stepper motors
  stepperX.setMaxSpeed(4000);
  stepperX.setAcceleration(1000);
  stepperX.setSpeed(4000);

  stepperY.setMaxSpeed(4000);
  stepperY.setAcceleration(800);
  stepperY.setSpeed(1000);

  stepperZ.setMaxSpeed(4000);
  stepperZ.setAcceleration(800);
  stepperZ.setSpeed(2000);

  // Add steppers to MultiStepper object
  steppers.addStepper(stepperX);
  steppers.addStepper(stepperY);
  steppers.addStepper(stepperZ);
}

//------------------------------------------------------------------------------
// Process incoming commands via UART
void processCommand(char *data)
{
  int len = Uart.inputString.length();
  
  // Command for X-axis
  if (strstr(data, ":X") != NULL)
  {
    gc.AXISFlag[0] = true;
    strncpy(ctemp, data + 2, len - 2);
    ctemp[len - 2] = '\0';
    gc.RotatorAngle[0] = atoi(ctemp);
    gc.RotatorFlag[0] = 1;
    gc.AXISNumber[0] = 1;
    vTaskResume(hmultimotor);
  }
  
  // Command for Y-axis
  if (strstr(data, ":Y") != NULL)
  {
    gc.AXISFlag[1] = true;
    strncpy(ctemp, data + 2, len - 2);
    ctemp[len - 2] = '\0';
    gc.RotatorAngle[1] = atoi(ctemp);
    gc.RotatorFlag[1] = 1;
    gc.AXISNumber[0] = 1;
    vTaskResume(hmultimotor);
  }
  
  // Command for Z-axis
  if (strstr(data, ":Z") != NULL)
  {
    gc.AXISFlag[2] = true;
    strncpy(ctemp, data + 2, len - 2);
    ctemp[len - 2] = '\0';
    gc.RotatorAngle[2] = atoi(ctemp);
    gc.RotatorFlag[2] = 1;
    gc.AXISNumber[0] = 1;
    vTaskResume(hmultimotor);
  }
}

//------------------------------------------------------------------------------
// UART task for handling serial communication
void vUARTTask(void *pvParameters)
{
  (void)pvParameters;
  Serial.print(F("UARTTask at core:"));
  Serial.println(xPortGetCoreID());

  for (;;)
  {
    while (Serial.available() > 0)
    {
      Uart.c = Serial.read();
      if ((Uart.c == '\n') || (Uart.c == '\r'))
      {
        if (Uart.lineIndex > 0)
        {
          Uart.line[Uart.lineIndex] = '\0';
          processCommand(Uart.line);
          Uart.lineIndex = 0;
        }
        Serial.println(F("ok>"));
      }
      else if (Uart.lineIndex < LINE_BUFFER_LENGTH - 1)
      {
        Uart.line[Uart.lineIndex++] = Uart.c;
      }
    }
    vTaskDelay(1);
  }
}

//------------------------------------------------------------------------------
// Setup function
void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("Starting multitask stepper control");

  // Initialize motor setup and UART
  initMultiMotorSetup();
  initial();
}

//------------------------------------------------------------------------------
// Loop function
void loop()
{
  vTaskDelay(1);
}
