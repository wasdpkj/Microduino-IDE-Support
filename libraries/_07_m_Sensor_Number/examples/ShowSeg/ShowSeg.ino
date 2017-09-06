// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// By YLB  yanglibin@microduino.cc
// ==============

/*
  模块wiki:https://wiki.microduino.cn/index.php/Sensor-Number

  控制数码管7段，高电平点亮。
  数据低位开始对应a,b,c,d,e,f,g

       a
      ————-
  f |       | b
    |   g   |
      ————-
  e |       | c
    |       |
      ————-
       d
  取模软件下载：https://wiki.microduino.cn/index.php/File:%E4%B8%83%E6%AE%B5%E6%95%B0%E7%A0%81%E7%AE%A1%E5%8F%96%E6%A8%A1%E8%BD%AF%E4%BB%B6.zip
  取模方式：https://wiki.microduino.cn/images/a/a6/Sensor_Number_getseg.jpg
   
  电路：
  -数码管接到核心的4，5引脚

  注意：
  -Core、ESP32的A6，A7接口不能用于串口控制。
  -控制段时不能直接控制点，只能通过setPoint()函数来控制点。

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

//控制段数据,从低位开始对应a,b,c,d,e,f,g
byte num_data[7] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40};

void setup() {

  LED.begin();

}

void loop() {
  for (uint8_t i = 0; i < 7; i++)
  {
    LED.setSeg(0, num_data[i], LIGHT);    //点亮段
    LED.show();                          //显示
    delay(500);               //切换显示之间时间间隔，单位毫秒
  }
}