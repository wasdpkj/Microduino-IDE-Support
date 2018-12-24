#include <Microduino_Key.h>
#include <Microduino_Ai.h>

//Core UART Port: [SoftSerial] [D2,D3]
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D2, TX:D3 */
MicroAi mAi(&mySerial);
#endif

//Core+ UART Port: [Serial1] [D2,D3]
#if defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
MicroAi mAi(&Serial1);
#endif

#if defined (ESP32)
HardwareSerial mySerial1(1);  //UART1
MicroAi mAi(&mySerial1, D2, D3);
#endif

#define BT_NAME     "my MicroAi"

DigitalKey touch1(D4);
DigitalKey touch2(D6);

void setup() {
  Serial.begin(115200);
  touch1.begin(INPUT_PULLUP);
  touch2.begin(INPUT_PULLUP);

  Serial.println("MicroAi set mode: BT.");
  Serial.println("start initiation.");
  if (mAi.begin(MODE_BT)) {
    Serial.println("init OK!");
  } else {
    Serial.println("init failed!");
    while (1);
  }

  if (mAi.nameBT(BT_NAME)) {
    Serial.println("set bt name OK!");
  } else {
    Serial.println("set bt name failed!");
    while (1);
  }
  mAi.setVolume(60);//max 63
  delay(1000);
}

void loop() {

  if (touch2.readEvent() == SHORT_PRESS) {
    mAi.cmdBT(CMD_FORWARD);
  }

  if (touch1.readEvent() == SHORT_PRESS) {
    mAi.cmdBT(CMD_PAUSE);
  }

  delay(50);
}