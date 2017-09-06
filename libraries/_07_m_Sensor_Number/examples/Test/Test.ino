// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// By YLB  yanglibin@microduino.cc
// ==============

/*
  模块wiki:https://wiki.microduino.cn/index.php/Sensor-Number

  控制4个数码管
     1.初始化显示每段，a,b,c,d,e,f
   2.初始化设置数码管方向
   3.循环显示数字0-9
   4.熄灭数码管

    电路：
    -数码管接到核心的4，5引脚

    注意：
    Core、ESP32的A6，A7接口不能用于串口控制

    2017年9月1日修改
*/
#include <Microduino_Number.h>

#define NUM 4
#define LIGHT 255

#if defined (__AVR__)
SoftwareSerial mySerial(4, 5);   // Core RX, TX
//#define mySerial Serial1      // Core+ D2,D3
Number LED(NUM, &mySerial);  //使用串口
#endif

#if defined(ESP32)
HardwareSerial mySerial(1);
Number LED(NUM, &mySerial, D4, D5);  //使用串口
#endif

byte num_data[7] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40};

void setup() {

  LED.begin();

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