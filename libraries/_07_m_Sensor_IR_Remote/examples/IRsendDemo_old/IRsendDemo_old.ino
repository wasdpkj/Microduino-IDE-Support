/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * New features: {
 *      At AVR The IR_LED Pin fixed as D4
 * }
 * Old:{
 *      An IR LED must be connected to Arduino PWM pin:
 *      Microduino Core:D3(Timer2)/D10(Timer1)
 *      Microduino Core+:D8(Timer2)/D22(Timer1)
 *      Microduino CoreUSB:D6(Timer3)/D9(Timer1)
 *      Microduino CoreRF:D5(Timer3)/D8(Timer1)
 * }
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>

IRsend irsend;

void setup()
{
  Serial.begin(115200);
}

void loop() {
  if (Serial.read() != -1) {
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0xa90, 12); // Sony TV power code
      delay(100);
    }
  }
}

