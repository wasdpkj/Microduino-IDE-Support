/*
  PPM精确发送例程
  数值范围1000到2000
  中点为1500
*/

#include <Microduino_PPM.h>

PPM PPM;

#define OUTPUT_PIN 2
#define CHAN_NUM  4

uint16_t value[4] = {
  128, 256, 384, 512
};

void setup () {
  Serial.begin(115200);
  PPM.beginWrite(OUTPUT_PIN, CHAN_NUM * 2); //引脚,通道数（可选，默认8）
  PPM.setFix(-5);    //发送数据修正,单位us
}

void loop () {
  writePrecisionData(value, CHAN_NUM);
  delay(20);
}

void writePrecisionData(uint16_t* _value , uint8_t _num) {
  uint16_t _data[8];
  for (uint8_t a = 0; a < _num; a ++) {
    _data[a * 2] = _value[a] >> 5;
    _data[a * 2 + 1] = _value[a] & 0x1f;
  }

  for (int i = 0; i < CHAN_NUM * 2; i++) {
    int pulseWidth = map(_data[i], 0, 31, 1000, 2000);
    PPM.Write(i, pulseWidth);
  }
}
