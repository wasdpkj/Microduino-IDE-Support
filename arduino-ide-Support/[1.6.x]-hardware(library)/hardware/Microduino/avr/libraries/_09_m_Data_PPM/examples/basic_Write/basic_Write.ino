/*
  PPM发送例程
  数值范围1000到2000
  中点为1500
*/

#include <Microduino_PPM.h>

PPM PPM;

#define OUTPUT_PIN 2
#define CHAN_NUM  8

uint16_t value[8] = {
  128, 256, 384, 512, 128, 256, 384, 512
};

void setup () {
  Serial.begin(115200);
  PPM.beginWrite(OUTPUT_PIN, CHAN_NUM);  //引脚,通道数（可选，默认8）
  PPM.setFix(-5);    //发送数据修正,单位us
}

void loop () {
  for (int i = 0; i < CHAN_NUM; i++) {
    int pulseWidth = map(value[i], 0, 1023, 1000, 2000);
    PPM.Write(i, pulseWidth);
  }
  delay(20);
}
