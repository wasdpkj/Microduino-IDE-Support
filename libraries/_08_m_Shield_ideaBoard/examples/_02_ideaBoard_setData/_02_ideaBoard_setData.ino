/*************************************************
  本示例给出了控制ideaBoard上传感器的方法
  包含：
        0号LED灯（红）
        1号LED灯（黄）
        2号LED灯（绿）
        全彩RGB灯
        右侧三路DAC输出
        摇杆阈值（默认50）
        (蜂鸣器的控制见:toneTest)
****************************************
   2017-08-10  by CWJ
****************************************/

#include <Microduino_ideaBoard.h>

#if !defined (__AVR_ATmega328P__)
#error "Board selection error,you should choose Core(328p)@16M,5V"
#endif

void setup() {
  Serial.begin(9600);
  ideaBoard.begin();
  ideaBoard.setSensitive(60);  //设置摇杆阈值，可不设置，默认50
  Serial.println("TEST START");
}

void loop() {
  //*****************set*******输出控制
  ideaBoard.setLED(0, HIGH);
  ideaBoard.setColorLED(10, 0, 0);
  delay(150);
  ideaBoard.setLED(0, LOW);
  ideaBoard.setLED(1, HIGH);
  ideaBoard.setColorLED(0, 10, 0);
  delay(150);
  ideaBoard.setLED(1, LOW);
  ideaBoard.setLED(2, HIGH);
  ideaBoard.setColorLED(0, 0, 10);
  delay(150);
  ideaBoard.setLED(2, LOW);
  ideaBoard.setColorLED(0, 0, 0);
  
  ideaBoard.setOUT(0, map(25, 0, 50, 0, 255));
  ideaBoard.setOUT(1, map(25, 0, 50, 0, 255));
  ideaBoard.setOUT(2, map(25, 0, 50, 0, 255));  

  delay(100);
}

