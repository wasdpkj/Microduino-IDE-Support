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
*本例程作用：接收8通道PPM数据
*/

#include <Microduino_PPM.h>

PPM PPM;

#define INPUT_PIN 2	//必须为中断脚

uint16_t value[8];

void setup () {
  Serial.begin(115200);
  PPM.beginRead(INPUT_PIN); //必须为中断脚
}

void loop () {
  uint8_t numChannel = PPM.getChannel();  //获取通道数
  Serial.print("numChannel:");
  Serial.println(numChannel);
  PPM.Read(value);		//得到8通道PPM数据，写入变量value
  for (int i = 0; i < numChannel; i++) {
    Serial.print(map(value[i], 1000, 2000, 0, 1023));
    if (i < numChannel - 1) {
      Serial.print(",");
    }
  }
  Serial.println("");
  delay(20);
}
