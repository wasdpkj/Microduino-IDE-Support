/*
  [head,2byte,0xAA 0xBB] [type,1byte,TYPE_NUM] [data,16byte] [body,1byte,getChecksum()]
  Example:
  AA BB C8 DC 05 DC 05 D0 07 EF 03 DC 05 DC 05 DC 05 DC 05 E3
*/

#include <Microduino_Protocol.h>

SoftwareSerial mySerial(4, 5);

ProtocolSer protocolA(&Serial1, 16);
ProtocolSer protocolB(&mySerial, 16);

uint8_t sendData[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
uint8_t recData[16];
uint8_t recCmd;
uint32_t sendTime;

void setup() {
  Serial.begin(9600);
  protocolA.begin(9600);  //9600/19200/38400
  protocolB.begin(9600);  //9600/19200/38400
  sendTime = millis();
}

void loop() {
	
  if(millis() - sendTime > 1000)
  {
	  sendTime = millis();
	  protocolA.write(0x01, sendData, 16);
    Serial.println("protocolA send !");
  }
  
  if(protocolB.available())
  {
	  protocolB.readBytes(&recCmd, recData, 16);
	  Serial.println("protocolB Received !");
	  Serial.print("recCmd: ");
	  Serial.print(recCmd);
	  Serial.print("  Data:");
	  for(uint8_t i=0; i<16; i++)
	  {
		  Serial.print(" ");
		  Serial.print(recData[i]);
	  }
    Serial.println();
  }
 
  delay(10);
}