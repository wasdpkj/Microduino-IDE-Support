/***************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  @小崔  cuiwenjing@microduino.cc

  Sensor_Pulse维基网址:
  https://wiki.microduino.cn/index.php/Sensor-Pulse/zh

  本示例给出了使用Pulse传感器测量心率的方法
 ****************************************/

#include <Microduino_Pulse.h>

Pulse pulse(A0);    //将Pulse类实例化，括号内为Pulse传感器所用引脚号

uint16_t pulseValue;         //定义变量
uint32_t testTime = millis();//定义时间变量

void setup() {
  Serial.begin(9600);  //串口初始化
}

void loop() {
  if (pulse.available()) {        //如果测到心率值
    pulseValue = pulse.getPulse();//获取心率值
    testTime = millis();          //更新testTime时间值
    Serial.print(pulseValue);    //串口打印心率值
    Serial.println(" /min");     //串口打印信息
  }
  else if (millis() - testTime > 10000) { //如果超过10s测不到脉搏
    testTime = millis();                  //更新testTime时间值
    Serial.println("Test error! please try again.");//串口打印信息
  }
}
