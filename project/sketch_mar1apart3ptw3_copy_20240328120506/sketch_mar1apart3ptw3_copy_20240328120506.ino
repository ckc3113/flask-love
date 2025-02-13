#include <ESP32Servo.h>

// 定义舵机对象
Servo servoBase;
Servo servoShoulder;
Servo servoElbow;
Servo servoWrist;
Servo servoClaw; // 夹爪舵机

// 定义舵机初始角度
const int baseInitAngle = 90;
const int shoulderInitAngle = 90;
const int elbowInitAngle = 90;
const int wristInitAngle = 90;
const int clawOpenAngle = 90; // 夹爪张开的角度
const int clawCloseAngle = 0; // 夹爪关闭的角度

void setup() {
  // 初始化舵机对象
  servoBase.setPeriodHertz(50); // 设置舵机控制周期为50赫兹
  servoShoulder.setPeriodHertz(50);
  servoElbow.setPeriodHertz(50);
  servoWrist.setPeriodHertz(50);
  servoClaw.setPeriodHertz(50);

  servoBase.attach(13);       // 连接基座舵机到数字引脚13
  servoShoulder.attach(12);   // 连接肩部舵机到数字引脚12
  servoElbow.attach(14);      // 连接肘部舵机到数字引脚14
  servoWrist.attach(27);      // 连接腕部舵机到数字引脚27
  servoClaw.attach(26);       // 连接夹爪舵机到数字引脚26

  // 可选：将各个舵机移动到初始位置
  servoBase.write(baseInitAngle);
  servoShoulder.write(shoulderInitAngle);
  servoElbow.write(elbowInitAngle);
  servoWrist.write(wristInitAngle);
  servoClaw.write(clawOpenAngle); // 初始时夹爪张开
}

void loop() {
  // 放下物体
  putDownObject();
  delay(2000); // 延迟2秒

  // 拿取物体
  pickUpObject();
  delay(2000); // 延迟2秒
}

// 放下物体的函数
void putDownObject() {
  // 将夹爪舵机张开
  servoClaw.write(clawOpenAngle);
  delay(500); // 等待0.5秒

  // 将机械臂移动到放下物体的位置
  servoBase.write(90);
  servoShoulder.write(90);
  servoElbow.write(90);
  servoWrist.write(90);
}

// 拿取物体的函数
void pickUpObject() {
  // 将机

}