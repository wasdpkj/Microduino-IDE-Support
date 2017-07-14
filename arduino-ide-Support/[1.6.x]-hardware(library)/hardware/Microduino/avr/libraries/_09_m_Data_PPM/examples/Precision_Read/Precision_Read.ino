// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @老潘orz  wasdpkj@hotmail.com
// ==============

/*
*PPM接收例程
*PPM是航模中较常用的一个协议，通过一个I/O口，可以以50hz的频率收发8通道的数据
详见：https://github.com/wasdpkj/Microduino-IDE-Support/tree/master/arduino-ide-Support/%5B1.6.x%5D-hardware(library)/hardware/Microduino/avr/libraries/_09_m_Data_PPM/PPM.png
*PPM接收到的数据，有效数值范围为1000到2000
*其中中点为1500
*PPM接收引脚必须为中断脚：【Core：D2 D3】【Core+：D2 D3 D6】【CoreUSB：D0 D1 D2 SDA SCL】
*本例程作用：将接收到的8通道PPM数据，转换为10bit精度的4通道数据
*/

#include <Microduino_PPM.h>

PPM PPM;

#define INPUT_PIN 2	//必须为中断脚

uint16_t value[4];

void setup () {
  Serial.begin(115200);
  PPM.beginRead(INPUT_PIN); //必须为中断脚
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

