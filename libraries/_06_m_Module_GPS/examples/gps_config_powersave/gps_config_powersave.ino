#include <Microduino_GPS.h>

//Core UART Port: [SoftSerial] [D2,D3]
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D2, TX:D3 */
#define GPSSerial mySerial
Microduino_GPS GPS(&GPSSerial);
#endif

//Core+ UART Port: [Serial1] [D2,D3]
#if defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define GPSSerial Serial1
Microduino_GPS GPS(&GPSSerial);
#endif

//CoreESP32 UART Port: [D2,D3]
#if defined (ESP32)
HardwareSerial GPSSerial(1);
Microduino_GPS GPS(&GPSSerial,D2,D3);
#endif

void setup()
{
  Serial.begin(115200);

  GPS.begin(38400);  //9600/19200/38400/57600/115200
  GPS.set_powermode(POWERSAVE);  //CONTINUOUS/POWERSAVE
  GPS.set_config(CONFIG_SAVE);  //CONFIG_DEFAULT/CONFIG_SAVE
}

void loop() // run over and over again
{
  if (GPSSerial.available()) {
    char c = GPSSerial.read();
    Serial.write(c);
  }
}