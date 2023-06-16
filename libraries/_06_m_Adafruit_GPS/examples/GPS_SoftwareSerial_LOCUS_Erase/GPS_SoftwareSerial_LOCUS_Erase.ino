// Test code for Adafruit GPS modules using MTK3329/MTK3339 driver
//
// This code erases the LOCUS built-in datalogger storage
//
// Tested and works great with the Adafruit Ultimate GPS module
// using MTK33x9 chipset
//    ------> http://www.adafruit.com/products/746
// Pick one up today at the Adafruit electronics shop
// and help support open source hardware & software! -ada

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

// Connect the GPS Power pin to 5V
// Connect the GPS Ground pin to ground
// Connect the GPS TX (transmit) pin to Digital 8
// Connect the GPS RX (receive) pin to Digital 7

// you can change the pin numbers to match your wiring:
SoftwareSerial mySerial(8, 7);
Adafruit_GPS GPS(&mySerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO  false

// this keeps track of whether we're using the interrupt
// off by default!
#ifndef ESP8266 // Sadly not on ESP8266
bool usingInterrupt = false;
#endif

void setup()
{
  while (!Serial) ;  //wait for serial port on Leonardo

  // connect at 115200 so we can read the GPS fast enuf and
  // also spit it out
  Serial.begin(115200);
  Serial.println("Adafruit GPS erase FLASH!");

  // 9600 NMEA is the default baud rate for MTK
  GPS.begin(9600);

  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_OFF);

  // the nice thing about this code is you can have a timer0 interrupt go off
  // every 1 millisecond, and read data from the GPS for you. that makes the
  // loop code a heck of a lot easier!
#ifndef ESP8266 // Not on ESP8266
  useInterrupt(true);
#endif

  Serial.println("This code will ERASE the data log stored in the FLASH - Permanently!");
  Serial.print("Are you sure you want to do this? [Y/N]: ");
  while (Serial.read() != 'Y')   delay(10);
  Serial.println("\nERASING! UNPLUG YOUR ARDUINO WITHIN 5 SECONDS IF YOU DIDNT MEAN TO!");
  delay(5000);
  GPS.sendCommand(PMTK_LOCUS_ERASE_FLASH);
  Serial.println("Erased");
}

void loop()                     // run over and over again
{
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}

/******************************************************************/
// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
#ifndef ESP8266 // Not on ESP8266
ISR(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO && c) {
#ifdef UDR0
    UDR0 = c;
    // writing direct to UDR0 is much much faster than Serial.print
    // but only one character can be written at a time.
#else
    Serial.write(c);
#endif
  }
}

void useInterrupt(bool v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}
#endif // ESP8266
