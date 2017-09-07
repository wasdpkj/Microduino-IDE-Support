/*************************************************
  本示例给出了快速获取ideaBoard上传感器的模拟值的方法
      ADC_Data[0]：左侧第一路输入模拟信号值（0-255）
      ADC_Data[1]：左侧第二路输入模拟信号值（0-255）
      ADC_Data[2]：左侧第三路输入模拟信号值（0-255）
      ADC_Data[3]：摇杆Y轴模拟值（0-255）
      ADC_Data[4]：摇杆X轴模拟值（0-255）
      ADC_Data[5]：光照值（0-255）
      ADC_Data[6]：声音值（0-255）
      ADC_Data[7]：按键原始模拟值（0-255）
  
****************************************
   2017-08-10  by CWJ
****************************************/

#include <Microduino_ideaBoard.h>

#if !defined (__AVR_ATmega328P__)
#error "Board selection error,you should choose Core(328p)@16M,5V"
#endif
  
uint8_t ADC_Data[8];

void setup() {
  Serial.begin(9600);
  ideaBoard.begin();
  Serial.println("TEST START");
}

void loop() {
  ideaBoard.getRAW(ADC_Data);
  for (int i = 0; i < 8; i++) {
    Serial.print(ADC_Data[i]);
    Serial.print("  ");
  }
  Serial.println("  ");

  delay(10);
}

