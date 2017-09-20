/*
   Microduino_AudioPro支持库例程
   模块WIKI：https://wiki.microduino.cn/index.php/MCookie-Module_AudioPro
   简易的MP3，需配合Microduino_SD模块、Sensor Joystick传感器使用
   本例程中 getMusicNum 可以获取SD卡中曲目列表以及数量
*/

#include <Microduino_Key.h>
#include <Microduino_AudioPro.h>
#include <SD.h>

AudioPro_FilePlayer musicPlayer =  AudioPro_FilePlayer(SD);

AnalogKey keyAnalog[5] {(A0), (A0), (A0), (A0), (A0)};
enum KeyName {
  UP, DOWN, LEFT, RIGHT, PRESS
};


uint8_t musicNum = 1; //歌曲序号
uint8_t fileNum = 0;  //文件数量

void playNum(uint8_t num) {
  if (num > musicPlayer.getMusicNum() - 1) {
    return;
  }

  if (!musicPlayer.stopped()) {
    musicPlayer.stopPlaying();  //必要，否则SD类得不到关闭，内存溢出
  }
  musicPlayer.flushCancel(both);  //清缓存，播放MIDI等格式文件时必要

  String _name = musicPlayer.getMusicName(num);
  Serial.print(F("Playing:"));
  if (!musicPlayer.playMP3(_name)) {
    Serial.println(F("ERROR"));
  }
  else {
    Serial.print(F("OK \t File: "));
    Serial.println(_name);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("AudioPro(VS1053) Simple Test"));
  pinMode(SD_PIN_SEL, OUTPUT);    //先初始化AudioPro，所以先使能SD卡
  digitalWrite(SD_PIN_SEL, HIGH);
  delay(500);

  if (! musicPlayer.begin()) { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }
  Serial.println(F("VS1053 found"));

  if (!SD.begin(SD_PIN_SEL)) {
    Serial.println(F("initialization failed!"));
    return;
  }
  Serial.println(F("initialization done."));

  musicPlayer.setVolume(96);  //left & right 0-127
  //or
  //musicPlayer.setVolume(96, 96);  //left right 0-127

  Serial.println(F("Enter Index of File to play"));
  fileNum = musicPlayer.getMusicNum();    //可以获取SD卡中曲目列表以及数量
  Serial.print(F("Music Files : "));
  Serial.println(fileNum);
  for (uint8_t a = 0; a < fileNum; a++) {
    Serial.print(F("\t File["));
    Serial.print(a);
    Serial.print(F("]: "));
    Serial.println(musicPlayer.getMusicName(a));
  }

  // If DREQ is on an interrupt pin, we can do background
  musicPlayer.useInterrupt(VS1053_PIN_DREQ);  // DREQ int

  for (uint8_t a = 0; a < 5; a++) {
    keyAnalog[a].begin(INPUT);
  }
#if defined (ESP32)
  analogReadResolution(10);  //9-12 在此ADC设置为10bit精度
#endif
  delay(200);
}


void loop() {
  switch (keyAnalog[UP].readEvent(700 - 50, 700 + 50)) {
    case SHORT_PRESS:  {
        int _volume = musicPlayer.volumeUp();
        Serial.print(F("Volume changed to "));
        Serial.println(_volume);
        delay(100);
      }
      break;
    case LONG_PRESS:
      int _volume = musicPlayer.volumeUp();
      Serial.print(F("Volume changed to "));
      Serial.println(_volume);
      delay(100);
      break;
  }

  switch (keyAnalog[DOWN].readEvent(330 - 50, 330 + 50)) {
    case SHORT_PRESS: {
        int _volume = musicPlayer.volumeDown();
        Serial.print(F("Volume changed to "));
        Serial.println(_volume);
      }
      break;
    case LONG_PRESS:
      int _volume = musicPlayer.volumeDown();
      Serial.print(F("Volume changed to "));
      Serial.println(_volume);
      delay(100);
      break;
  }

  switch (keyAnalog[LEFT].readEvent(512 - 50, 512 + 50)) {
    case SHORT_PRESS:
      musicNum--;
      if (musicNum < 1 ) {
        musicNum = fileNum;
      }
      playNum(musicNum - 1);
      break;
  }

  switch (keyAnalog[RIGHT].readEvent(860 - 50, 860 + 50)) {
    case SHORT_PRESS:
      musicNum++;
      if (musicNum > fileNum) {
        musicNum = 1;
      }
      playNum(musicNum - 1);
      break;
  }

  switch (keyAnalog[PRESS].readEvent(0, 50)) {
    case SHORT_PRESS:
      if (musicPlayer.stopped()) {
        Serial.println(F("Playing!"));
        playNum(musicNum - 1);
      }
      else if (! musicPlayer.paused()) {
        Serial.println(F("Paused"));
        musicPlayer.pausePlaying(true);   //暂停
      } else {
        Serial.println(F("Resumed"));
        musicPlayer.pausePlaying(false);  //取消暂停
      }

      break;
    case LONG_PRESS:
      Serial.println(F("Stopping"));
      musicPlayer.stopPlaying();
      delay(500);
      break;
  }
  delay(50);
}
