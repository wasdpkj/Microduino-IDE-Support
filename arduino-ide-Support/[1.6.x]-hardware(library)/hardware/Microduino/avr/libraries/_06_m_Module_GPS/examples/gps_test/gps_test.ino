/**
**CoreUSB UART Port: [Serial1] [D0,D1]
**Core+ UART Port: [Serial1] [D2,D3]
**/
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define SerialGPS Serial1
#endif

/**
**Core UART Port: [SoftSerial] [D2,D3]
**/
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#include <SoftwareSerial.h>
SoftwareSerial SerialGPS(2, 3); /* RX:D2, TX:D3 */
#endif


void setup() {
  Serial.begin(115200);
  SerialGPS.begin(38400);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    SerialGPS.write(c);
  }
  if (SerialGPS.available()) {
    char c = SerialGPS.read();
    Serial.write(c);
  }
}
