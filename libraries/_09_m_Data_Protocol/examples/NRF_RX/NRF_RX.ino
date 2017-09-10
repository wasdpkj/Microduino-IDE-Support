/*
   [header.type,1byte,TYPE_NUM] [millis,4byte] [data,16byte]
  Example:
  hear(C8) millis(time) DC 05 DC 05 D0 07 EF 03 DC 05 DC 05 DC 05 DC 05
*/

#include <Microduino_Protocol.h>

#define NRF_CHANNEL 70  //nRF通道
#define this_node  	1  //设置本机ID

RF24 radio(9, 10);
RF24Network network(radio);
ProtocolnRF nrfProtocol(&network, 16);   //采用ProtocolnRF，数据长度为16个字节

uint16_t recData[8];
uint8_t recCmd;

void setup() {
  Serial.begin(9600);
  radio.begin();
  if (radio.isPVariant()) {
	Serial.println("nrf24 module Initialization ");
    nrfProtocol.begin(NRF_CHANNEL, this_node);	
  }
  else{
	Serial.println("nrf24 module is not founded "); 
  }
}

void loop() {
  
  if(nrfProtocol.available())
  {
    nrfProtocol.readWords(&recCmd, recData, 8);
    Serial.println("protocol Received !");
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
