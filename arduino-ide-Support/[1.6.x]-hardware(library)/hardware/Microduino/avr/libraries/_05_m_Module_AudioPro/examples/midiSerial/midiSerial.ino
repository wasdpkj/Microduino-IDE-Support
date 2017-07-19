/*
  使用前应该先将模块背后的跳线短接，才可进入串口模式
*/

#include <Microduino_AudioPro.h>
#include "MIDI.h"

void setup() {
  Serial.begin(9600);
  delay(200);

  Serial.print(F("F_CPU = "));
  Serial.println(F_CPU);
  Serial.print(F("Free RAM = ")); // available in Version 1.0 F() bases the string to into Flash, to use less SRAM.
  Serial.print(FreeRam(), DEC);  // FreeRam() is provided by SdFatUtil.h

  VS1053_MIDI.begin(31250);
  // Set volume for left, right channels. lower numbers == louder volume!
  midiSetChannelBank(0, VS1053_BANK_MELODY);
  midiSetInstrument(0, VS1053_GM1_PIANO);
  midiSetChannelVolume(0, 127);
}

void loop() {
  for (uint8_t i = 57; i < 87; i++) {
    midiNoteOn(0, i, 127);
    delay(100);
    midiNoteOff(0, i, 127);
    delay(100);
  }
  delay(1000);
}
