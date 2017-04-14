#include "ESP8266.h"

#define UARTSPEED  115200
uint16_t baud_data[5] = {9600, 19200, 38400, 57600, 115200};

//CoreUSB UART Port: [Serial1] [D0,D1]
#if defined(__AVR_ATmega32U4__)
#define EspSerial Serial1
#endif

//Core+ UART Port: [Serial1] [D2,D3]
#if defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define EspSerial Serial1
#endif


//Core UART Port: [SoftSerial] [D2,D3]
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D3, TX:D2 */
#define EspSerial mySerial
#endif

//CoreUSB UART Port: [Serial1] [D0,D1]
#if defined(__AVR_ATmega32U4__)
#define EspSerial Serial1
#endif

//Core+ UART Port: [Serial1] [D2,D3]
#if defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define EspSerial Serial1
#endif


//Core UART Port: [SoftSerial] [D2,D3]
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D3, TX:D2 */
#define EspSerial mySerial
#endif

ESP8266 wifi(&EspSerial);

void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately
  Serial.print("setup begin\r\n");
  delay(100);

  for (int a = 0; a < 5; a++) {
    EspSerial.begin(baud_data[a]);
    delay(100);
    while (EspSerial.available() > 0) {
      EspSerial.read();
    }

    wifi.setUart(UARTSPEED, DEFAULT_PATTERN);
  }

  EspSerial.begin(UARTSPEED);
  delay(100);
  while (EspSerial.available() > 0) {
    EspSerial.read();
  }
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    EspSerial.write(c);
  }
  if (EspSerial.available()) {
    char c = EspSerial.read();
    Serial.write(c);
  }
}
