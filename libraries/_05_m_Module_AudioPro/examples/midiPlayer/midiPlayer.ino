/*
   Microduino_AudioPro支持库例程
   模块WIKI：https://wiki.microduino.cn/index.php/MCookie-Module_AudioPro
   SPI的MIDI例程
*/

#include <Microduino_AudioPro.h>
#include <SPI.h>

#define AUDIO_MIDI      D2
#define AUDIO_CS        A3
#define AUDIO_DC        A2
#define AUDIO_IRQ       D3

// AudioPro midiPlayer;
AudioPro midiPlayer(&SPI, AUDIO_MIDI, AUDIO_CS, AUDIO_DC, AUDIO_IRQ);


uint8_t toneNum[12] = {57, 59, 60, 62, 64, 65, 67, 69, 71, 72, 74, 76};

void setup() {
  Serial.begin(115200);
  delay(200);
  
  pinMode(AUDIO_CS, OUTPUT);
  digitalWrite(AUDIO_CS, HIGH);  
  SPI.begin();

  if (! midiPlayer.begin()) { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }
  Serial.println(F("VS1053 found"));
  
  if (!midiPlayer.pathMidi()) {
    Serial.println(F("Couldn't switch mide mode!"));
    while (1);
  }

  midiPlayer.midiSetVolume(0, 127); //channels,volume
  // See http://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf Pg 31
  // VS1053_BANK_DEFAULT VS1053_BANK_MELODY VS1053_BANK_DRUMS1 VS1053_BANK_DRUMS2
  midiPlayer.midiSetBank(0, VS1053_BANK_MELODY);
  // See http://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf Pg 32 for more!
  midiPlayer.midiSetInstrument(0, VS1053_GM1_RHODES_PIANO);
}

void loop() {
  for (uint8_t i = 0; i < 12; i++) {
    midiPlayer.noteOn(0, toneNum[i], 127);
    delay(100);
    midiPlayer.noteOff(0, toneNum[i], 127);
    delay(50);
  }
  delay(1000);

  for (uint8_t i = 27; i < 87; i++) {
    midiPlayer.noteOn(0, i, 127);
    delay(100);
    midiPlayer.noteOff(0, i, 127);
    delay(50);
  }
  delay(1000);
}