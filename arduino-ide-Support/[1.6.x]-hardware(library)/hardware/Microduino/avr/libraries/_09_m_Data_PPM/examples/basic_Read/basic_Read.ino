/*
*PPM接收例程
*数值范围1000到2000
*中点为1500
*/

#include <Microduino_PPM.h>

PPM PPM;

#define INPUT_PIN 2

uint16_t value[8];

void setup () {
  Serial.begin(115200);
  PPM.beginRead(INPUT_PIN); //引脚
}

void loop () {
  uint8_t numChannel = PPM.getChannel();  //获取通道数
  Serial.print("numChannel:");
  Serial.println(numChannel);
  PPM.Read(value);
  for (int i = 0; i < numChannel; i++) {
    Serial.print(map(value[i], 1000, 2000, 0, 1023));
    if (i < numChannel - 1) {
      Serial.print(",");
    }
  }
  Serial.println("");
  delay(20);
}
