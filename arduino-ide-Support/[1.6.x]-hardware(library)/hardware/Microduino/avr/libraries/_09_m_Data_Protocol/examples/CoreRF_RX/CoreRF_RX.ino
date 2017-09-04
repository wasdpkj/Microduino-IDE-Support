/*
  [head,2byte,0xAA 0xBB] [type,1byte,TYPE_NUM] [data,16byte] [body,1byte,getChecksum()]
  Example:
  AA BB C8 DC 05 DC 05 D0 07 EF 03 DC 05 DC 05 DC 05 DC 05 E3
*/

#include <Microduino_Protocol.h>

ProtocolZig protocolA(16); //数据长度为16个字节

uint16_t recData[8];
uint8_t recCmd;

void setup() {
  Serial.begin(9600);
  protocolA.begin(11);  //括号内参数为CoreRF通道号
}

void loop() {
	if(protocolA.available())
	{
		protocolA.readWords(&recCmd, recData, 8);
		Serial.println("protocolB Received !");
		Serial.print("recCmd: ");
		Serial.print(recCmd);
		Serial.print("  Data:");
		for(uint8_t i=0; i<8; i++)
		{
		  Serial.print(" ");
		  Serial.print(recData[i]);
		}
		Serial.println();
	}
  delay(10);
}
