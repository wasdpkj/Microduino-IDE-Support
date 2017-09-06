// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// By YLB  yanglibin@microduino.cc
// ==============

/*
  模块wiki:https://wiki.microduino.cn/index.php/Sensor-Number

  依次循环显示0-9数字。
  数字能被2整除显示点，否则不显示。

  电路：
  数码管接到核心的4，5引脚

  注意：
  Core、ESP32的A6，A7接口不能用于串口控制

  2017年9月1日修改

*/

#include <Microduino_Number.h>

#define NUM   1
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


void setup() {
  LED.begin();
}

void loop() {
  for (uint8_t i = 0; i < 10; i++)
  {
    if (i % 2) {                 //数字能被2整除
      LED.setPoint(0, false);   //关闭点
    }
    else {
      LED.setPoint(0, true);   //显示点
    }
    LED.setNumber(0, i, LIGHT);//设置数字
    LED.show();                //显示数字
    delay(1000);              //切换数字之间时间间隔，单位毫秒
  }
}