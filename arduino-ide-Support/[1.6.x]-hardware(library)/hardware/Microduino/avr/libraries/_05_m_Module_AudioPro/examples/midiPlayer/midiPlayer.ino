#include <Microduino_AudioPro.h>

AudioPro midiPlayer;

void setup() {
  Serial.begin(115200);
  delay(200);
  
  Serial.print(F("F_CPU = "));
  Serial.println(F_CPU);
  Serial.print(F("Free RAM = ")); // available in Version 1.0 F() bases the string to into Flash, to use less SRAM.
  Serial.print(FreeRam(), DEC);  // FreeRam() is provided by SdFatUtil.h

  if ( midiPlayer.begin()) { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }
  Serial.println(F("VS1053 found"));
  midiPlayer.applyPatch(MIDIPatch, sizeof(MIDIPatch) / sizeof(uint16_t));

  // Set volume for left, right channels. lower numbers == louder volume!
  midiPlayer.setVolume(0, 0);
  midiPlayer.midiSetBank(0, VS1053_BANK_DEFAULT);
  midiPlayer.midiSetInstrument(0, 11);
  
}

void loop() {  

  for (uint8_t i=57; i<87; i++) {
    midiPlayer.noteOn(0, i, 127);
    delay(100);
    midiPlayer.noteOff(0, i, 127);
    delay(100);
  }
  delay(1000);
}