/*
  使用前应该先将模块背后的跳线短接，才可进入串口模式
*/

#include <Microduino_AudioPro.h>
#include "MIDI.h"
uint8_t toneNum[12] = {59, 60, 62, 64, 65, 67, 69, 71, 72, 74, 76, 77};
int pin_light[12] = {
  4, 5, 6, 7, 8, 9, A0, A1, SDA, SCL, A6, A7
};

int mode = 0;
boolean sta[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
boolean sta_cache[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

unsigned long time_rst = 0;

void setup() {
  Serial.begin(9600);
  delay(200);
  for (int a = 0; a < 12; a++) {
    pinMode(pin_light[a], INPUT_PULLUP);
  }

  VS1053_MIDI.begin(31250);
  // Set volume for left, right channels. lower numbers == louder volume!
  midiSetChannelBank(0, VS1053_BANK_MELODY);
  midiSetInstrument(0, VS1053_GM1_PIANO);
  midiSetChannelVolume(0, 127);
}

void loop() {
  for (int a = 0; a < 12; a++) {
    int b = digitalRead(pin_light[a]);
    sta[a] = b;
    if (!sta[a] && sta_cache[a]) {
      Serial.print(a);
      Serial.println("on");
      midiNoteOn(0, toneNum[a + 2], 127);
      //Led_on(a, 5);
    }
    else if (sta[a] && !sta_cache[a]) {
      Serial.print(a);
      Serial.println("off");
      midiNoteOff(0, toneNum[a + 2], 127);
    }
    sta_cache[a] = sta[a];
  }
  delay(1);
}

