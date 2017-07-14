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

    
    电路：
    * 数码管接到核心的4，5引脚

    注意：
    Core的A6，A7接口不能用于软串口控制。
    控制段时不能直接控制点，只能通过setPoint()函数来控制点。

    2017年7月5日修改
    By YLB

*/

#include <Microduino_Number.h>

#define NUM   1
#define LIGHT 255

SoftwareSerial mySerial(4, 5);   // RX, TX
Number LED(NUM, &mySerial);  //使用软串口

//Number LED(NUM, &Serial1); //使用硬串口0，1

//控制段数据,从低位开始对应a,b,c,d,e,f,g
byte num_data[7] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40};

void setup() {
  LED.begin();               //初始化数码管
}

void loop() {
  for (uint8_t i = 0; i < 7; i++)
  {
    LED.setSeg(0, num_data[i], LIGHT);    //点亮段
    LED.show();                          //显示
    delay(500);               //切换显示之间时间间隔，单位毫秒
  }
}
