/*
  [head,2byte,0xAA 0xBB] [type,1byte,TYPE_NUM] [data,16byte]
  Example:
  AA BB C8 DC 05 DC 05 D0 07 EF 03 DC 05 DC 05 DC 05 DC 05
*/

#include <Microduino_Protocol.h>

#define NRF_CHANNEL 70  //nRF通道

/* 预先设置好两个通讯地址，总长度为6位   */
uint8_t addresses[][6] = {"1Node","2Node"};

RF24 radio(D9, D10);
ProtocolnRF nrfProtocol(&radio, 16);   //采用ProtocolnRF，数据长度为16个字节

uint16_t recData[8];
uint8_t recCmd;

void setup() {
  Serial.begin(9600);
  //设置nRF通道， 本机地址，目标地址
  if (nrfProtocol.begin(NRF_CHANNEL, addresses[0], addresses[1])) {
	  Serial.println("nrf24 module Initialization ");
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