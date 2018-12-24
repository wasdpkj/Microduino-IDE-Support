#include <BittyBuggy.h>

//Core UART Port: [SoftSerial] [D2,D3]
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D2, TX:D3 */
BittyBuggy car(&mySerial);
#endif

//Core+ UART Port: [Serial1] [D2,D3]
#if defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define mySerial Serial1
BittyBuggy car(&mySerial);
#endif

#if defined (ESP32)
HardwareSerial mySerial1(1);  //UART1
BittyBuggy car(&mySerial1, D2, D3);
#endif

uint8_t datacolor[6];

void setup() {
  Serial.begin(57600);
  while (!car.begin())
    ;
}

void loop() {
  car.getColorRaw(datacolor);
  for (int8_t i = 0; i < 6; i++) {
    Serial.print(datacolor[i]);
    Serial.print(" , ");
  }
  Serial.print("\n");
}
