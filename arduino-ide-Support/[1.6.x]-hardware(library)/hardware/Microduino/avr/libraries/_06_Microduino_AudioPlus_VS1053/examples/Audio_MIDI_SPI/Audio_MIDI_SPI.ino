/*
 * http://wiki.microduino.cn/index.php/Microduino-Module_AudioPro/zh
 */
// include SPI, MP3 and SD libraries
#include <SPI.h>
#include "Audio.h"

uint8_t toneNum[12] = {57, 59, 60, 62, 64, 65, 67, 69, 71, 72, 74, 76};

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Adafruit VS1053 Simple Test");
  if (! musicPlayer.begin()) { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }
  Serial.println(F("VS1053 found"));
  musicPlayer.applyPatch(MIDIPatch, sizeof(MIDIPatch) / sizeof(uint16_t));

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(0, 0);

  midiSetChannelBank(0, VS1053_BANK_MELODY);
  midiSetInstrument(0, VS1053_GM1_RHODES_PIANO);

  musicPlayer.GPIO_pinMode(4, OUTPUT);  //LM4863,EN
  musicPlayer.GPIO_digitalWrite(4, LOW);
  //midiSetChannelVolume(0, 127);
}

void loop() {
  for (uint8_t i = 0; i < 12; i++) {
    midiNoteOn(0, toneNum[i], 127);
    delay(100);
    midiNoteOff(0, toneNum[i], 127);
    delay(50);
  }
  delay(1000);

  for (uint8_t i = 27; i < 87; i++) {
    midiNoteOn(0, i, 127);
    delay(100);
    midiNoteOff(0, i, 127);
    delay(50);
  }
  delay(1000);
}
