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

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

void setup()
{
  Serial.begin(115200);

  GPS.begin(38400);  //9600/19200/38400/57600/115200
  //GPS.set_cnssmode(GPS_SBAS);  //GPS_SBAS/BEIDOU_SBAS(only NEO-M8N)/QZSS_SBAS/GLONASS_SBAS
  GPS.set_powermode(CONTINUOUS);  //CONTINUOUS/POWERSAVE
  GPS.set_updata(UPDATE_2HZ);  //UPDATE_1HZ/UPDATE_2HZ/UPDATE_4HZ/UPDATE_5HZ
  GPS.set_config(CONFIG_SAVE);  //CONFIG_DEFAULT/CONFIG_SAVE
}


uint32_t timer = millis();
void loop() // run over and over again
{
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }
  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis()) timer = millis();

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    Serial.print("\nTime: ");
    Serial.print(GPS.hour, DEC); 
    Serial.print(':');
    Serial.print(GPS.minute, DEC); 
    Serial.print(':');
    Serial.print(GPS.seconds, DEC); 
    Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); 
    Serial.print('/');
    Serial.print(GPS.month, DEC); 
    Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); 
    Serial.print((int)GPS.fix);
    Serial.print(" quality: "); 
    Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
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
  }
}

