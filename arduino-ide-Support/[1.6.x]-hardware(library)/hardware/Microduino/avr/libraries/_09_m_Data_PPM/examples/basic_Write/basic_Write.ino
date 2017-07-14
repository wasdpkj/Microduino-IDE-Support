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
*本例程作用：发送8通道PPM数据
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
  PPM.beginWrite(OUTPUT_PIN, CHAN_NUM);  //引脚,通道数（不填默认8）
  PPM.setFix(-5);    //发送数据修正,单位us
}

void loop () {
  for (int i = 0; i < CHAN_NUM; i++) {
    int pulseWidth = map(value[i], 0, 1023, 1000, 2000);
    PPM.Write(i, pulseWidth);	//数值范围须为1000到2000之间
  }
  delay(20);
}
