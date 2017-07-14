/***************************************
本示例给出了使用Pulse传感器测量心率的方法
 ****************************************/

#include <Microduino_Pulse.h>

Pulse pulse(A0);    //括号内为Pulse传感器所用引脚号

uint16_t pulseValue;
uint32_t testTime = millis();

void setup() {
  Serial.begin(9600);
}

void loop() {
  if(pulse.available()){  
    pulseValue = pulse.getPulse();
    testTime = millis();
    Serial.print(pulseValue);
    Serial.println(" /min");
  }
  else if(millis()-testTime>10000){
    testTime = millis();
    Serial.println("Test error! please try again.");
  }
}

