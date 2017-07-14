/******************************************************
本示例给出了Motor控制电机的一些基本方法
一个Motor模块可以同时驱动两个直流电机，占用引脚D5，D6，D7，D8
此示例中将两个电机分别命名为MotorLeft、MotorRight
*******************************************************/

#include <Microduino_Motor.h>

Motor MotorLeft(MOTOR0_PINA, MOTOR0_PINB);
Motor MotorRight(MOTOR1_PINA, MOTOR1_PINB);

void setup()
{
  Serial.begin(115200);
  Serial.println("Microduino_Motor!");

  MotorLeft.begin();
  MotorRight.begin();
}

void loop()
{
  Serial.println("Forward!");
  MotorLeft.setSpeed(100);
  MotorRight.setSpeed(100);
  delay(2000);

  Serial.println("Break!");
  MotorLeft.Brake();
  MotorRight.Brake();
  delay(2000);

  Serial.println("Back!");
  MotorLeft.setSpeed(-100);
  MotorRight.setSpeed(-100);
  delay(2000);

  Serial.println("Free!");
  MotorLeft.setSpeed(FREE);
  MotorRight.setSpeed(FREE);
  delay(2000);
}