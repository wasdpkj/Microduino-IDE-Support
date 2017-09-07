// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @Machine  maxing@microduino.cc
// 
//Dust 不支持ESP32
//
// ==============

#include <Microduino_Dust.h>

SoftwareSerial mySerial(4, -1); // RX, TX
Dust pmSensor(&mySerial);    //使用软串口
//Dust pmSensor(&Serial1);   //Core+ D2,D3

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("Read PM2.5: "); 
  
  if(pmSensor.available())
    Serial.println(pmSensor.getPM25());
  else
    Serial.println("none sensor!");
  
  delay(1000);
}
