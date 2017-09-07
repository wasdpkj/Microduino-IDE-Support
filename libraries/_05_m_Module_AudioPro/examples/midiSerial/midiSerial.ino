/*
   Microduino_AudioPro支持库例程
   模块WIKI：https://wiki.microduino.cn/index.php/MCookie-Module_AudioPro
   串口MIDI例程
   使用前应该先将模块背后的跳线短接，才可进入串口模式
*/

#include <Microduino_AudioPro.h>
#include "Midi.h"


void setup() {
  Serial.begin(9600);
  delay(200);

#if defined(ESP32)
  VS1053_MIDI.begin(31250, SERIAL_8N1, -1, VS1053_PIN_MIDI);
#endif
#if defined (__AVR__)
  VS1053_MIDI.begin(31250);
#endif

  midiSetChannelVolume(0, 127);//channels,volume
  // See http://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf Pg 31
  // VS1053_BANK_DEFAULT VS1053_BANK_MELODY VS1053_BANK_DRUMS1 VS1053_BANK_DRUMS2
  midiSetChannelBank(0, VS1053_BANK_MELODY);
  // See http://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf Pg 32 for more!
  midiSetInstrument(0, VS1053_GM1_PIANO);
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
