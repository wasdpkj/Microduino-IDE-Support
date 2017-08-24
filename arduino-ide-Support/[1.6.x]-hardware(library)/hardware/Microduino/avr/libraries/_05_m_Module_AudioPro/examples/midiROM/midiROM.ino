/*
   Microduino_AudioPro支持库例程
   模块WIKI：https://wiki.microduino.cn/index.php/MCookie-Module_AudioPro
   可以用以下工具转换：https://wiki.microduino.cn/index.php/File:DataToHex.zip
   播放ROM乐曲的例程，推荐MIDI格式
*/

#include <Microduino_AudioPro.h>
#include "file.h"

AudioPro midiPlayer;

void setup() {
  uint8_t result; //result code from some function as to be tested at later time.
  Serial.begin(115200);
  delay(200);

  if (! midiPlayer.begin()) { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }
  Serial.println(F("VS1053 found"));

  midiPlayer.setVolume(10, 10);

  // If DREQ is on an interrupt pin, we can do background
  midiPlayer.useInterrupt(VS1053_PIN_DREQ);  // DREQ int
  
  Serial.println(F("pleast input 'a' or 'm' to play the midi file:."));
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'a')
      midiPlayer.playROM(Array, sizeof(Array));
    else if (c == 'm')
      midiPlayer.playROM(marselje, sizeof(marselje));
  }
}