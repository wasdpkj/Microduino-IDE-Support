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

void setup()
{
  Serial.begin(57600);
  while (!car.begin())
    ;
  car.setSpeedA(0);
  car.setSpeedB(0);
}

void loop()
{
  Serial.println("Forward!");
  car.setSpeed(50, 50);
  delay(2000);
  Serial.println("BRAKE!");
  car.setSpeed(BRAKE, BRAKE);
  delay(2000);
  Serial.println("Back!");
  car.setSpeed(-250, -250);
  delay(2000);
  Serial.println("Free!");
  car.setSpeed(FREE, FREE);
  delay(2000);
}