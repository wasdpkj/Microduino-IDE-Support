/*********************************************************
  //  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  // 支持Microduino_Sensor_Weight
  //该示例用于重置传感器示数，即重新去皮
  // Microduino wiki:
  // http://wiki.microduino.cn

  // E-mail:
  // jinqipeng@mircoduino.cc

  //日期：2018.8
*********************************************************/
#include "Sensor_Weight.h"

SensorWeight WT(WEIGHT_ADDRESS);

uint32_t Weight = 0;
uint16_t gap = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!WT.begin())
  {
    Serial.println("Error while initialling...");
  }
  gap = WT.getGap();
  WT.getMaopi();
}

void loop() {
  if (Serial.available())
  {
    WT.setZero();
    Serial.read();
  }
  Serial.print("OUTPUT : ");
  Serial.println(WT.getWeight());
  //A more-than-100MS delay is highly recommended since the Firmware based on HX711 got a sequence of 10Hz.
  delay(400);
}