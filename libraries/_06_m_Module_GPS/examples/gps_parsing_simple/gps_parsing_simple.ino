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
}
void loop() 
{
  if (GPS.available() == GPS_DATA_READY) {
    timePrint();
    gpsPrint();
  }
  else if (GPS.available() == GPS_TIME_READY) {
    timePrint();
  }
}
void timePrint() {
  Serial.print("Time: ");
  Serial.print(GPS.hour, DEC);
  Serial.print(':');
  Serial.print(GPS.minute, DEC);
  Serial.print(':');
  Serial.println(GPS.seconds, DEC);
  Serial.print("Date: ");
  Serial.print(GPS.day, DEC);
  Serial.print('/');
  Serial.print(GPS.month, DEC);
  Serial.print("/20");
  Serial.println(GPS.year, DEC);
}
void gpsPrint() {
  Serial.print("Location: ");
  Serial.print(GPS.latitude, 4);
  Serial.print(GPS.lat);
  Serial.print(", ");
  Serial.print(GPS.longitude, 4);
  Serial.println(GPS.lon);
  Serial.print("Speed (knots): ");
  Serial.println(GPS.speed);
  Serial.print("Angle: ");
  Serial.println(GPS.angle);
  Serial.print("Altitude: ");
  Serial.println(GPS.altitude);
  Serial.print("Satellites: ");
  Serial.println((int)GPS.satellites);
}

