/******************************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  @Microduino_sy  shenyang@microduino.cc

  Microduino_Motor维基网址:
  https://wiki.microduino.cn/index.php/MCookie-Motor/zh

  本示例给出了Motor控制电机的一些基本方法
  一个Motor模块可以同时驱动两个直流电机，占用引脚5，6，7，8
  此示例中将两个电机分别命名为MotorLeft、MotorRight
  
  如果使用核心Microduino_ESP32，任意引脚都可以用来驱动电机
  供电允许条件下最多可驱动四个电机
  四个电机分别使用LEDC_PWM的4、5、6、7通道驱动
  用数字引脚前要加D,例：D5
  Microduino/mCookie motor模块默认使用引脚D5，D6，D7，D8
  本示例可直接使用
*******************************************************/

#include <Microduino_Motor.h>

Motor MotorLeft(MOTOR0_PINA, MOTOR0_PINB);
Motor MotorRight(MOTOR1_PINA, MOTOR1_PINB);

void setup()
{
  Serial.begin(115200); //串口初始化
  Serial.println("Microduino_Motor!");

  MotorLeft.begin();   //电机MotorLeft初始化
  MotorRight.begin();  //电机MotorLeft初始化
}

void loop()
{
  Serial.println("Forward!");
  MotorLeft.setSpeed(100);   //设置电机MotorLeft速度为100
  MotorRight.setSpeed(100);  //设置电机MotorRight速度为100
  delay(2000);

  Serial.println("Break!");
  MotorLeft.Brake();        //电机MotorLeft刹车
  MotorRight.Brake();       //电机MotorRight刹车
  delay(2000);

  Serial.println("Back!");
  MotorLeft.setSpeed(-100);  //设置电机MotorLeft速度为-100
  MotorRight.setSpeed(-100); //设置电机MotorRight速度为-100
  delay(2000);

  Serial.println("Free!");
  MotorLeft.setSpeed(FREE);  //设置电机MotorLeft为释放状态，即速度为0
  MotorRight.setSpeed(FREE); //设置电机MotorRight为释放状态，即速度为0
  delay(2000);
}
