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

uint16_t tonelist[8] = {523, 587, 659, 697, 784, 880, 988, 0};

uint8_t dataspecialcolor[2];

void setup() {
  Serial.begin(57600);
  while (!car.begin())
    ;
}

void loop() {
  //  dataspecialcolor[0]=car.getColorA();
  //  dataspecialcolor[1]=car.getColorB();

  car.getColor(dataspecialcolor);
  Serial.print(dataspecialcolor[0]);
  Serial.print(" , ");
  Serial.println(dataspecialcolor[1]);

  if (dataspecialcolor[0] > 0 && dataspecialcolor[0] < 8 && (dataspecialcolor[0] == dataspecialcolor[1])) {
    car.tone(tonelist[dataspecialcolor[0] - 1]);
  } else {
    car.noTone();
  }
}