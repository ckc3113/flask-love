#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Stepper.h>

// WiFi网络信息
char ssid[] = "CHONG";
char pass[] = "11031023";

// Blynk认证密钥
char auth[] = "BLYNK_AUTH_TOKEN";

// 步进电机参数
const int stepsPerRevolution = 200;
Stepper myStepperX(stepsPerRevolution, 14, 15, 16, 17); // X轴步进电机引脚
Stepper myStepperY(stepsPerRevolution, 18, 19, 20, 21); // Y轴步进电机引脚
Stepper myStepperZ(stepsPerRevolution, 22, 23, 24, 25); // Z轴步进电机引脚

// 红外线传感器引脚
const int irSensors[3][3] = {
  {2, 3, 4},
  {5, 6, 7},
  {8, 9, 10}
};

// 当前位置
int currentX = 0;
int currentY = 0;
int currentZ = 0;

// 移动步进电机到指定位置
void moveToPosition(int targetX, int targetY, int targetZ) {
  int stepsX = targetX - currentX;
  int stepsY = targetY - currentY;
  int stepsZ = targetZ - currentZ;

  myStepperX.step(stepsX);
  myStepperY.step(stepsY);
  myStepperZ.step(stepsZ);

  currentX = targetX;
  currentY = targetY;
  currentZ = targetZ;
}

void setup() {
  Serial.begin(9600);

  // 连接WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // 初始化Blynk
  Blynk.begin(auth, ssid, pass);

  // 设置步进电机速度
  myStepperX.setSpeed(60);
  myStepperY.setSpeed(60);
  myStepperZ.setSpeed(60);

  // 设置红外线传感器引脚为输入模式
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      pinMode(irSensors[row][col], INPUT);
    }
  }
}

void loop() {
  Blynk.run();

  // 移动步进电机到每个位置并检测红外线传感器状态
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      moveToPosition(row, col, 0); // 移动到每个格子的底部
      delay(1000); // 等待1秒，让步进电机移动到位

      // 读取红外线传感器状态
      int sensorValue = digitalRead(irSensors[row][col]);
      if (sensorValue == HIGH) {
        Serial.println("Shoe detected in cell (" + String(row) + ", " + String(col) + ")");
        // 在这里执行相关操作，比如提醒用户有鞋子存在
      } else {
        Serial.println("No shoe detected in cell (" + String(row) + ", " + String(col) + ")");
      }
    }
  }
}

