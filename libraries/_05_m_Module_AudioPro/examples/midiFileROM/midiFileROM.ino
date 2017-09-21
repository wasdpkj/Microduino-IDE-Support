/*
   Microduino_AudioPro支持库例程
   模块WIKI：https://wiki.microduino.cn/index.php/MCookie-Module_AudioPro
   可以用以下工具转换：https://wiki.microduino.cn/index.php/File:DataToHex.zip
   播放ROM乐曲的例程，推荐MIDI格式
   歌曲可选：Jay、Canon、CastleA、CastleB
*/

#include <Microduino_AudioPro.h>
#include <utility/MIDI_file.h>

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

  midiPlayer.setVolume(96);  //left & right 0-127
  //or
  //midiPlayer.setVolume(96, 96);  //left right 0-127

  // If DREQ is on an interrupt pin, we can do background
  midiPlayer.useInterrupt(VS1053_PIN_DREQ);  // DREQ int

  Serial.println(F("pleast input 'a' or 'b' or 'c' or 'j' to play the midi file:."));
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'j')
      midiPlayer.playROM(Jay, sizeof(Jay));
    else if (c == 'c')
      midiPlayer.playROM(Canon, sizeof(Canon));
    else if (c == 'a')
      midiPlayer.playROM(CastleA, sizeof(CastleA));
    else if (c == 'b')
      midiPlayer.playROM(CastleB, sizeof(CastleB));
  }
}
