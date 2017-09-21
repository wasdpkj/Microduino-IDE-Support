/*
  [head,2byte,0xAA 0xBB] [type,1byte,TYPE_NUM] [data,16byte] [body,1byte,getChecksum()]
  Example:
  AA BB C8 DC 05 DC 05 D0 07 EF 03 DC 05 DC 05 DC 05 DC 05 E3
*/

#include <Microduino_Protocol.h>

//Core UART Port: [SoftSerial] [D2,D3]
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D2, TX:D3 */
#define ProSerial mySerial
ProtocolSer protocol(&ProSerial, 16);  //采用ProSerial，数据长度为16个字节
#endif

//Core+ UART Port: [Serial1] [D2,D3]
#if defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define ProSerial Serial1
ProtocolSer protocol(&ProSerial, 16);  //采用ProSerial，数据长度为16个字节
#endif

#if defined (ESP32)
HardwareSerial mySerial1(1);  //UART1
ProtocolSer protocol(&mySerial1, 16, D2, D3);  //采用mySerial1，数据长度为16个字节
#endif

uint16_t recData[8];
uint8_t recCmd;

void setup() {
  Serial.begin(9600);
  protocol.begin(9600);  //9600/19200/38400
}

void loop() {
  
  if(protocol.available())
  {
    protocol.readWords(&recCmd, recData, 8);
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
