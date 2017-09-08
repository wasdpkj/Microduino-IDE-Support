/******************************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  @Microduino_sy  shenyang@microduino.cc

  Microduino_MotorPlus维基网址:
  https://wiki.microduino.cn/index.php/MCookie-MotorPlus/zh

  本示例给出了MotorPlus控制电机的一些基本方法
  MotorPlus模块，采用IIC通讯，IIC地址可通过模块上的拨码开关设置，
  IIC地址设置可选择：
  MOTOR_ADDR1   0x70
  MOTOR_ADDR2   0x71
  MOTOR_ADDR3   0x72
  MOTOR_ADDR4   0x73
  一个MotorPlus模块可以同时驱动两个直流电机，
*******************************************************/

#include <Microduino_MotorPlus.h>

MotorPlus motor(MOTOR_ADDR4);

void setup()
{
  Serial.begin(115200); //串口初始化
  Serial.println("Microduino_MotorPlus!");

  motor.begin();   //电机MotorLeft初始化
//motor.begin(BIT_8);	//可以设置电机的分辨率，BIT_8/BIT_9/BIT_10/BIT_11/BIT_12/BIT_13/BIT_14

}

void loop()
{
  Serial.println("Forward!");
  motor.setSpeed1(100);		//设置电机1速度为100
  motor.setSpeed2(100);		//设置电机2速度为100
  delay(2000);

  Serial.println("Brake!");
  motor.setSpeed1(BRAKE);	//电机1刹车
  motor.setSpeed2(BRAKE);	//电机2刹车
  delay(2000);

  Serial.println("Back!");
  motor.setSpeed(-100, -100);  //同时设置电机1和电机2速度为-100
  delay(2000);

  Serial.println("Free!");
  motor.setSpeed(FREE, FREE);  //同时设置电机1和电机2为释放状态，即速度为0
  delay(2000);
}

