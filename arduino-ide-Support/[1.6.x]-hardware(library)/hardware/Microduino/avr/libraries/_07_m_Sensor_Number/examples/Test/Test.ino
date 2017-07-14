/*

  模块wiki:https://wiki.microduino.cn/index.php/Sensor-Number 

  控制4个数码管
   1.初始化显示每段，a,b,c,d,e,f，h不能通过段控制
   2.初始化设置数码管方向
   3.循环显示数字0-9
   4.熄灭数码管

  电路：
  * 数码管接到核心的4，5引脚

  注意：
  Core的A6，A7接口不能用于软串口控制

  2017年7月5日修改
  By YLB

*/
#include <Microduino_Number.h>

#define NUM 4

#define LIGHT 255

SoftwareSerial mySerial(4, 5); // RX, TX
Number LED(NUM, &mySerial);    //使用软串口（4，5）
//Number LED(NUM, &Serial1);    //使用硬串口

byte num_data[7] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40};

void setup() {

LED.begin(); //初始化数码管

//初始化显示每段，测试数码管好坏
for (uint8_t i = 0; i < 7; i++)
{
  for (uint8_t j = 0; j < 4; j++)
    LED.setSeg(j, num_data[i], LIGHT);
  LED.show();
  delay(300);
}
//初始化设置方向，正向：POSITIVE  反向：NEGATIVE
LED.direction(POSITIVE);
//LED.direction(NEGATIVE);
}

void loop() {
//显示0-9数字
  for (uint8_t i = 0; i < 10; i++)
  {
    for (uint8_t j = 0; j < 4; j++)
    {
      if (i % 2)
        LED.setPoint(j, false);
      else
        LED.setPoint(j, true);
      LED.setNumber(j, i, LIGHT);
    }
    LED.show();
    delay(500);
  }
//熄灭第1个数码管
  LED.turnOff(0);
  delay(500);
//熄灭所有数码管
  LED.turnOff(ALL);
  delay(500);
}