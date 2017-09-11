/*
  [header.type,1byte,TYPE_NUM] [millis,4byte] [data,16byte]
  Example:
  hear(C8) millis(time) DC 05 DC 05 D0 07 EF 03 DC 05 DC 05 DC 05 DC 05
*/

#include <Microduino_Protocol.h>

#define NRF_CHANNEL 70  //nRF通道
#define this_node  	0  //设置本机ID
#define other_node  1  //设置目标ID

RF24 radio(D9, D10);
RF24Network network(radio);
ProtocolnRF nrfProtocol(&network, 16);   //采用ProtocolnRF，数据长度为16个字节

uint16_t sendData[8] = {1500, 1500, 1500, 1500, 1000, 1000, 1000, 1000};
uint8_t sendCmd = 0x01;
uint32_t sendTime;

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
  if(millis() - sendTime > 1000)
  {
	  sendTime = millis();
	  nrfProtocol.write(other_node, sendCmd, (uint8_t *)sendData, 16);
    Serial.println("protocol send !");
  }
  delay(10);
}