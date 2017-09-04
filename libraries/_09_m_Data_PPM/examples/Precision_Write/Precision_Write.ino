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
*PPM发送引脚可以是任意引脚
*本例程作用：将10bit精度的4通道数据，通过8通道PPM发送出去
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
