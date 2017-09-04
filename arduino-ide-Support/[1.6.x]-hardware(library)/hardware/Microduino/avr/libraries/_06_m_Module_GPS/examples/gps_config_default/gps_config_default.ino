#include <Microduino_GPS.h>

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


Microduino_GPS GPS(&SerialGPS);

void setup()
{
  Serial.begin(115200);

  GPS.begin(38400);  //9600/19200/38400/57600/115200
  GPS.set_config(CONFIG_DEFAULT);  //CONFIG_DEFAULT/CONFIG_SAVE
}

void loop() // run over and over again
{
  if (SerialGPS.available()) {
    char c = SerialGPS.read();
    Serial.write(c);
  }
}
