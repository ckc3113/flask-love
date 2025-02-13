#include <AccelStepper.h>

// 定义 A4988 驱动器的引脚 (X 轴)
#define X_STEP_PIN 25      // X 轴步进信号引脚
#define X_DIR_PIN 26       // X 轴方向信号引脚
#define X_ENABLE_PIN 27   // X 轴使能引脚

// 定义 A4988 驱动器的引脚 (Y 轴)
#define Y_STEP_PIN 32      // Y 轴步进信号引脚
#define Y_DIR_PIN 33       // Y 轴方向信号引脚
#define Y_ENABLE_PIN 27   // Y 轴使能引脚

// 定义 A4988 驱动器的引脚 (Z 轴)
#define Z_STEP_PIN 22     // Z 轴步进信号引脚
#define Z_DIR_PIN 23       // Z 轴方向信号引脚
#define Z_ENABLE_PIN 27    // Z 轴使能引脚

// 定义步进电机接口类型 (X, Y 和 Z 轴)
AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);
AccelStepper stepperZ(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);

// 设置全步模式下每圈的步数
int stepsPerRevolution = 200;  // 全步模式，每圈 200 步

void setup() {
  // X 轴设置
  pinMode(X_ENABLE_PIN, OUTPUT);
  digitalWrite(X_ENABLE_PIN, HIGH); // 初始禁用 X 轴电机
  stepperX.setMaxSpeed(500);        // 最大速度（步/秒）
  stepperX.setAcceleration(300);    // 加速度（步/秒^2）

  // Y 轴设置
  pinMode(Y_ENABLE_PIN, OUTPUT);
  digitalWrite(Y_ENABLE_PIN, HIGH); // 初始禁用 Y 轴电机
  stepperY.setMaxSpeed(500);        // 最大速度（步/秒）
  stepperY.setAcceleration(300);    // 加速度（步/秒^2）

  // Z 轴设置
  pinMode(Z_ENABLE_PIN, OUTPUT);
  digitalWrite(Z_ENABLE_PIN, HIGH); // 初始禁用 Z 轴电机
  stepperZ.setMaxSpeed(500);        // 最大速度（步/秒）
  stepperZ.setAcceleration(300);    // 加速度（步/秒^2）

  // 启动串行监视器
  Serial.begin(115200);
}

void loop() {
  // 启用 X 轴电机
  Serial.println("启用 X 轴电机...");
  digitalWrite(X_ENABLE_PIN, LOW);  // 启用 X 轴电机

  // X 轴顺时针旋转一整圈
  Serial.println("X 轴电机顺时针旋转一圈...");
  stepperX.moveTo(stepsPerRevolution);  // X 轴移动 200 步（1 圈）
  while (stepperX.distanceToGo() != 0) {
    stepperX.run();  // X 轴电机移动到目标位置
  }

  delay(1000);  // 等待 1 秒

  // X 轴逆时针旋转一整圈
  Serial.println("X 轴电机逆时针旋转一圈...");
  stepperX.moveTo(-stepsPerRevolution);  // 反方向移动 200 步
  while (stepperX.distanceToGo() != 0) {
    stepperX.run();  // X 轴电机移动到目标位置
  }

  delay(1000);  // 等待 1 秒

  // 启用 Y 轴电机
  Serial.println("启用 Y 轴电机...");
  digitalWrite(Y_ENABLE_PIN, LOW);  // 启用 Y 轴电机

  // Y 轴顺时针旋转一整圈
  Serial.println("Y 轴电机顺时针旋转一圈...");
  stepperY.moveTo(stepsPerRevolution);  // Y 轴移动 200 步（1 圈）
  while (stepperY.distanceToGo() != 0) {
    stepperY.run();  // Y 轴电机移动到目标位置
  }

  delay(1000);  // 等待 1 秒

  // Y 轴逆时针旋转一整圈
  Serial.println("Y 轴电机逆时针旋转一圈...");
  stepperY.moveTo(-stepsPerRevolution);  // 反方向移动 200 步
  while (stepperY.distanceToGo() != 0) {
    stepperY.run();  // Y 轴电机移动到目标位置
  }

  delay(1000);  // 等待 1 秒

  // 启用 Z 轴电机
  Serial.println("启用 Z 轴电机...");
  digitalWrite(Z_ENABLE_PIN, LOW);  // 启用 Z 轴电机

  // Z 轴顺时针旋转一整圈
  Serial.println("Z 轴电机顺时针旋转一圈...");
  stepperZ.moveTo(stepsPerRevolution);  // Z 轴移动 200 步（1 圈）
  while (stepperZ.distanceToGo() != 0) {
    stepperZ.run();  // Z 轴电机移动到目标位置
  }

  delay(1000);  // 等待 1 秒

  // Z 轴逆时针旋转一整圈
  Serial.println("Z 轴电机逆时针旋转一圈...");
  stepperZ.moveTo(-stepsPerRevolution);  // 反方向移动 200 步
  while (stepperZ.distanceToGo() != 0) {
    stepperZ.run();  // Z 轴电机移动到目标位置
  }

  delay(1000);  // 等待 1 秒

  // 禁用 X, Y 和 Z 轴电机
  Serial.println("禁用 X, Y 和 Z 轴电机...");
  digitalWrite(X_ENABLE_PIN, HIGH);  // 禁用 X 轴电机
  digitalWrite(Y_ENABLE_PIN, HIGH);  // 禁用 Y 轴电机
  digitalWrite(Z_ENABLE_PIN, HIGH);  // 禁用 Z 轴电机

  delay(1000);  // 等待 1 秒后重复循环
}
