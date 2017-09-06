// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @Machine  maxing@microduino.cc
// ==============
#include <Sensor_Motion.h>

#if defined (__AVR__)
SoftwareSerial mySerial(4, 5);   // Core RX, TX
//#define mySerial Serial1      // Core+ D2，D3
sensorMotion motion(&mySerial);  //使用串口
#else
HardwareSerial mySerial(1);
sensorMotion motion(&mySerial, D4, D5); //使用串口
#endif

float ypr[3];//用来存放三轴姿态角

void setup() {
  Serial.begin(115200);

  motion.begin();    // RX, TX
}

void loop() {
  //如果成功接收数据，则串口输出三轴姿态角
  if (motion.getData(MOTION_3, ypr)) {
    Serial.print(ypr[0]);
    Serial.print("  ");
    Serial.print(ypr[1]);
    Serial.print("  ");
    Serial.println(ypr[2]);
  }
  delay(10);
}