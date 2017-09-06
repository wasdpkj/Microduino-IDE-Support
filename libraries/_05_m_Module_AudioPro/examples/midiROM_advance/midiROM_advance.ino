/*
   Microduino_AudioPro支持库例程
   模块WIKI：https://wiki.microduino.cn/index.php/MCookie-Module_AudioPro
   播放ROM乐曲的例程，推荐MIDI格式
   可以用以下工具转换：https://wiki.microduino.cn/index.php/File:DataToHex.zip
   详情见help
*/

#include <Microduino_AudioPro.h>
#include "file.h"

AudioPro midiPlayer;

void setup() {
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
  help();
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if ((0x20 <= c) && (c <= 0x126)) {  // strip off non-ASCII, such as CR or LF
      if (c >= '0' && c <= '9') {
        int num = c - 48;

        if (num == 0) {
          Serial.print(F("playing:"));
          Serial.print(F("romAddr:"));
          Serial.print(uint32_t(Array));
          Serial.print(F(" ,romLen:"));
          Serial.println(sizeof(Array));
          if (!midiPlayer.playROM(Array, sizeof(Array))) {
            Serial.println(F("play ERROR"));
          }
          else {
            Serial.println(F("play OK"));
          }
        }
        if (num == 1) {
          Serial.print(F("playing:"));
          Serial.print(F("romAddr:"));
          Serial.print(uint32_t(Jay));
          Serial.print(F(" ,romLen"));
          Serial.println(sizeof(Jay));
          if (!midiPlayer.playROM(Jay, sizeof(Jay))) {
            Serial.println(F("play ERROR"));
          }
          else {
            Serial.println(F("play OK"));
          }
        }
      }
      else if (c == 'a') {             //控制运放开关
        if (! midiPlayer.getAmplifier()) {
          Serial.println(F("Amplifier On"));
          midiPlayer.setAmplifier(true);
        } else {
          Serial.println(F("Amplifier Off"));
          midiPlayer.setAmplifier(false);
        }
      }
      else if (c == 's') {    //停止播放音乐
        Serial.println(F("Stopping"));
        midiPlayer.stopPlaying();
      }
      // if we get an 'p' on the serial console, pause/unpause!
      else if (c == 'p') {    //暂停播放音乐
        if (midiPlayer.stopped()) {
          Serial.println("Player is Stopping.");
        }
        else if (! midiPlayer.paused()) {
          Serial.println("Paused");
          midiPlayer.pausePlaying(true);
        } else {
          Serial.println("Resumed");
          midiPlayer.pausePlaying(false);
        }
      }
      else if ((c == '-') || (c == '+')) {  //调整音量
        int _volume;
        if (c == '-') { // note dB is negative
          _volume  = midiPlayer.volumeDown();
        }
        else {
          _volume = midiPlayer.volumeUp();
        }
        Serial.print(F("Volume changed to "));
        Serial.println(_volume);
      }
      else if (c == 'i') {          //显示系统常见信息
        Serial.print(F("getVolume:"));
        Serial.println(midiPlayer.getVolume());
        Serial.print(F("getAmplifier:"));
        Serial.println(midiPlayer.getAmplifier());
        Serial.print(F("getPlaySpeed:"));
        Serial.println(midiPlayer.getPlaySpeed());
        Serial.print(F("decodeTime:"));
        Serial.println(midiPlayer.decodeTime());
      }
      else if ((c == '>') || (c == '<')) {  //控制播放速度
        uint16_t playspeed = midiPlayer.getPlaySpeed(); // create key_command existing variable
        // note playspeed of Zero is equal to ONE, normal speed.
        if (c == '>') { // note dB is negative
          playspeed ++; // keep it simpler with whole dB's
        } else {
          playspeed --;
        }
        playspeed = constrain(playspeed, 1, 127);
        midiPlayer.setPlaySpeed(playspeed); // commit new playspeed
        Serial.print(F("playspeed to "));
        Serial.println(playspeed, DEC);
      }
      else if (c == 'm') {          //立体声
        uint16_t monostate = midiPlayer.getMonoMode();
        Serial.print(F("Mono Mode "));
        if (monostate == 0) {
          midiPlayer.setMonoMode(1);
          Serial.println(F("Enabled."));
        } else {
          midiPlayer.setMonoMode(0);
          Serial.println(F("Disabled."));
        }
      }
      else if (c == 'd') {          //差分输出
        uint16_t diff_state = midiPlayer.getDifferentialOutput();
        Serial.print(F("Differential Mode "));
        if (diff_state == 0) {
          midiPlayer.setDifferentialOutput(1);
          Serial.println(F("Enabled."));
        } else {
          midiPlayer.setDifferentialOutput(0);
          Serial.println(F("Disabled."));
        }
      }
      else if (c == 'r') {          //复位VS1053
        Serial.println(F("Reset Player."));
        midiPlayer.reset();
      }
      else if (c == 'n') {          //初始化VS1053
        Serial.println(F("begin Player."));
        midiPlayer.begin();
      }
      else if (c == 'f') {          //关闭VS1053
        Serial.println(F("End Player."));
        midiPlayer.end();
      }

      else if (c == 'h') {          //显示帮助
        help();
      }

    }
  }

  delay(100);
}

void help() {
  Serial.println(F("Microduino AudioPro Library Example:"));
  Serial.println(F("COMMANDS:"));
  Serial.println(F(" [0-1] to play a track"));
  Serial.println(F(" [a] to Set Amplifier"));
  Serial.println(F(" [s] to stop playing"));
  Serial.println(F(" [p] to pause or unpause"));
  Serial.println(F(" [s] to stop playing"));
  Serial.println(F(" [+ or -] to change volume"));
  Serial.println(F(" [> or <] to increment or decrement play speed by 1 factor"));
  Serial.println(F(" [m] Toggle between Mono and Stereo Output."));
  Serial.println(F(" [d] to toggle SM_DIFF between inphase and differential output"));
  Serial.println(F(" [r] Resets and initializes VS10xx chip."));
  Serial.println(F(" [f] turns OFF the VS10xx into low power reset."));
  Serial.println(F(" [n] turns ON the VS10xx out of low power reset."));
  Serial.println(F(" [i] retrieve current audio information (partial list)"));
  Serial.println(F(" [h] this help"));
}