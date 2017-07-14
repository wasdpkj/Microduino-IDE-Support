/********************************************************************************************************
本示例给出了MotorPlus控制电机的一些基本方法
一个MotorPlus模块可以同时驱动两个直流电机,使用IIC接口
通过改IIC地址，在供电允许的情况下，可同时使用四个MotorPlus模块
控制一个MotorPlus模块上的两个电机时，可使用moto1.setSpeed(speed1, speed2)语句控制，
                                     也可使用moto1.setSpeed1(speed1)、moto1.setSpeed2(speed2)来分别控制
********************************************************************************************************/

#include "Microduino_MotorPlus.h"  //头文件

MotorPlus moto1;    //默认MotorPlus IIC地址为0x73,如果更改IIC地址，可在括号里填写新地址，例：MotorPlus moto1(MOTOR_ADDR1); 

void setup()
{ 
  Serial.begin(115200);
  Serial.println("Microduino_MotorPlus!");
  
  if(moto1.begin()){  //电机初始化
    Serial.println("moto1 is online");
  }
  else{
    Serial.println("moto1 error");
    while(1);
  }  
  
}

void loop()
{
  Serial.println("Forward!");
  moto1.setSpeed(100, 100);//设置motor1 上的电机1速度为100，电机2为100
  delay(2000);
  
  Serial.println("Break!");
  moto1.setSpeed(BRAKE, BRAKE);//电机刹车
  delay(2000);
  
  Serial.println("Back!"); 
  moto1.setSpeed(-100, -100);//设置motor1 上的电机1速度为-100，电机2为-100
  delay(2000);
  
  Serial.println("Free!");
  moto1.setSpeed(FREE, FREE);//设置电机为释放状态，即速度为0
  delay(2000);
 
}