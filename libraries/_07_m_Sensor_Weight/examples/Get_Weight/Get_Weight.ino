/*********************************************************
  //  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  // 支持Microduino_Sensor_Weight

  // Microduino wiki:
  // http://wiki.microduino.cn

  // E-mail:
  // jinqipeng@mircoduino.cc

  //日期：2018.8
*********************************************************/
#include "Sensor_Weight.h"

SensorWeight WT(WEIGHT_ADDRESS);

float Weight = 0;
uint16_t gap = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!WT.begin())
  {
    Serial.println("Error while initialling...");
  }
  gap = WT.getGap(); //getGap from sensor EEP
  Serial.print("Version : ");
  Serial.println(WT.getVersion());
  //while (WT.getVersion() != 1);
  WT.getMaopi();
  WT.setGain(GAIN_128); //Better NOT to change the gain
}

void loop() {
  if (Serial.available())
  {
    WT.setReset();
    while (!WT.begin())
    {
      Serial.println("Error while initialling...");
    }
    char c = Serial.read();
  }
  Serial.print("OUTPUT : ");
  Weight = WT.getWeight();
  Serial.println(Weight,1);
  //A more-than-100MS delay is highly recommended since the Firmware based on HX711 got a sequence of 10Hz.
  delay(400);
}
