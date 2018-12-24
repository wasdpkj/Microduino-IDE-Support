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

uint8_t dataline[2];
int16_t carspeed[2];

void setup() {
  Serial.begin(57600);
  while (!car.begin())
    ;
}

void loop() {
  //car.getLineAlone(dataline, RED, GREEN);
  //car.getLineA(RED);
  //car.getLineB(GREEN);

  car.getLine(dataline, GREEN);
  trackControl(dataline[0], dataline[1]);
  car.setSpeed(carspeed[0], carspeed[1]);
  Serial.print("A:");
  Serial.print(dataline[0]);
  Serial.print("  B:");
  Serial.println(dataline[1]);
}

void trackControl(uint8_t trackVal1, uint8_t trackVal2)
{
  if (trackVal1 < 135 && trackVal2 < 135) {
    carspeed[0] = 0;
    carspeed[1] = 0;
  }
  else {
    carspeed[0] = 220 + (trackVal1 - 240) * 2; //白248 黑 130
    carspeed[1] = 220 + (trackVal2 - 240) * 2;
  }
  /*
  if (trackVal1 > 200 && trackVal2 > 200) {
    car.setSpeed(0, 0);
    delay(300);
    carspeed[0] = 0;
    carspeed[1] = 0;
  }
  else {
    carspeed[0] = 180 - (trackVal1 - 150) * 2;
    carspeed[1] = 180 - (trackVal2 - 150) * 2;
  }
  */
}