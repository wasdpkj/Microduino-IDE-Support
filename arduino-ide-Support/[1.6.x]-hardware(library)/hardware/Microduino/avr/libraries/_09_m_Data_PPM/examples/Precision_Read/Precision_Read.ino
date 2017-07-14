/*
  PPM精确接收例程
  数值范围1000到2000
  中点为1500
*/

#include <Microduino_PPM.h>

PPM PPM;

#define INPUT_PIN 2

uint16_t value[4];

void setup () {
  Serial.begin(115200);
  PPM.beginRead(INPUT_PIN); //引脚
}

void loop () {
  uint8_t numChannel = PPM.getChannel() / 2; //获取通道数
  Serial.print("numChannel:");
  Serial.println(numChannel);
  readPrecisionData(value, numChannel);
  for (int i = 0; i < numChannel; i++) {
    Serial.print(value[i]);
    if (i < numChannel - 1) {
      Serial.print(",");
    }
  }
  Serial.println("");
  delay(20);
}

float f_map(float x, float in_min, float in_max, float out_min, float out_max) {
  return float((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

void readPrecisionData(uint16_t* _value , uint8_t _num) {
  uint16_t _data[8];
  PPM.Read(_data);
  float _f_data[_num * 2];

  for (uint8_t a = 0; a < _num * 2; a++) {
    _f_data[a] = f_map( _data[a], 1000, 2000, 0, 31);
    _f_data[a] += 0.5f;
    _data[a] = (uint16_t) _f_data[a];
  }
  for (uint8_t a = 0; a < _num; a ++) {
    _value[a] = (_data[a * 2] << 5) + _data[a * 2 + 1];
  }
}

