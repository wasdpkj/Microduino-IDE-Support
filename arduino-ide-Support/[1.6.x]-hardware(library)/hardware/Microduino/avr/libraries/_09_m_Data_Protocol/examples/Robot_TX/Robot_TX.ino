/*
  [head,2byte,0xAA 0xBB] [type,1byte,TYPE_NUM] [data,16byte] [body,1byte,getChecksum()]
  Example:
  AA BB C8 DC 05 DC 05 D0 07 EF 03 DC 05 DC 05 DC 05 DC 05 E3
*/

#include <Microduino_Protocol.h>

ProtocolSer protocolA(&Serial1, 16);

uint16_t sendData[8] = {1500, 1500, 1500, 1500, 1000, 1000, 1000, 1000};
uint32_t sendTime;

void setup() {
  Serial.begin(9600);
  protocolA.begin(9600);  //9600/19200/38400
  sendTime = millis();
}

void loop() {
  if(millis() - sendTime > 1000)
  {
	  sendTime = millis();
	  protocolA.write(0x01, (uint8_t *)sendData, 16);
    Serial.println("protocolA send !");
  }
  delay(10);
}
