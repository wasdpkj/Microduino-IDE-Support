/*
  [head,2byte,0xAA 0xBB] [type,1byte,TYPE_NUM] [data,16byte] [body,1byte,getChecksum()]
  Example:
  AA BB C8 DC 05 DC 05 D0 07 EF 03 DC 05 DC 05 DC 05 DC 05 E3
*/

#include <Microduino_Protocol.h>

ProtocolZig protocolA(16);

uint16_t sendData[8] = {1500, 1500, 1500, 1500, 1000, 1000, 1000, 1000};
uint16_t recData[8];
uint8_t recCmd;
uint32_t sendTime;
uint8_t proRole = 0;	//0 is sender ,1 is receiver

void setup() {
  Serial.begin(9600);
  protocolA.begin(11);  //9600/19200/38400
  sendTime = millis();
}

void loop() {
	
  if(proRole == 0){
	if(millis() - sendTime > 1000)
	{
		sendTime = millis();
		protocolA.write(0x01, (uint8_t *)sendData, 16);
		Serial.println("protocolA send !");
	}
  }
  
  else if(proRole == 1){
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
  }
  delay(10);
}