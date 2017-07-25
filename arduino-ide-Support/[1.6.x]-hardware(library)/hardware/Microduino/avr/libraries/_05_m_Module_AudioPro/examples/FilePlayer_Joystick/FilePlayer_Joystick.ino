/*
   Microduino_AudioPro支持库例程
   模块WIKI：https://wiki.microduino.cn/index.php/MCookie-Module_AudioPro
   简易的MP3，需配合Microduino_SD模块、Sensor Joystick传感器使用
   本例程中 getMusicFile 可以获取SD卡中曲目列表以及数量
   为了防止内存不足：
     SD卡中的 歌曲最大数量通过 MUSIC_MAX 设置,并且要和字符串 FileName 数量对应上
*/

#include <Microduino_Key.h>
#include <Microduino_AudioPro.h>
#include <SD.h>

AudioPro_FilePlayer musicPlayer =  AudioPro_FilePlayer(SD);

AnalogKey keyAnalog[5] {(A0), (A0), (A0), (A0), (A0)};
enum KeyName {
  UP, DOWN, LEFT, RIGHT, PRESS
};

#define MUSIC_MAX 10
String FileName[MUSIC_MAX] = {
  "Microduino.mp3",
  "Microduino.mp3",
  "Microduino.mp3",
  "Microduino.mp3",
  "Microduino.mp3",
  "Microduino.mp3",
  "Microduino.mp3",
  "Microduino.mp3",
  "Microduino.mp3",
  "Microduino.mp3"
};

uint8_t musicNum = 1; //歌曲序号
uint8_t fileNum = 0;  //文件数量

void playNum(uint8_t num) {
  if (!musicPlayer.paused() || !musicPlayer.stopped()) {
    musicPlayer.stopPlaying();  //必要，否则SD类得不到关闭，内存溢出
  }

  Serial.print(F("Playing:"));
  if (!musicPlayer.playMP3( FileName[num])) {
    Serial.println(F("ERROR"));
  }
  else {
    Serial.print(F("OK \t File: "));
    Serial.println(FileName[num]);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(SD_PIN_SEL, OUTPUT);		//先初始化AudioPro，所以先使能SD卡
  digitalWrite(SD_PIN_SEL, HIGH);
  delay(500);

  if (! musicPlayer.begin()) { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }
  Serial.println(F("VS1053 found"));

  if (!SD.begin(SD_PIN_SEL)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20, 20);

  Serial.println(F("Enter Index of File to play"));
  fileNum = musicPlayer.getMusicFile(FileName);		//可以获取SD卡中曲目列表以及数量
  if (fileNum > MUSIC_MAX) {
    fileNum = MUSIC_MAX;
  }
  Serial.print(F("Music Files : "));
  Serial.println(fileNum);
  for (uint8_t a = 0; a < fileNum; a++) {
    Serial.print("\t File[");
    Serial.print(a);
    Serial.print("]: ");
    Serial.println(FileName[a]);
  }

  for (uint8_t a = 0; a < 5; a++) {
    keyAnalog[a].begin(INPUT);
  }
  delay(200);
}


void loop() {
  switch (keyAnalog[UP].readEvent(700 - 50, 700 + 50)) {
    case SHORT_PRESS:  {
        int _volume = musicPlayer.volumeUp();
        Serial.print(F("Volume changed to -"));
        Serial.print(_volume >> 1, 1);
        Serial.println(F("[dB]"));
      }
      break;
    case LONG_PRESS:
      int _volume = musicPlayer.volumeUp();
      Serial.print(F("Volume changed to -"));
      Serial.print(_volume >> 1, 1);
      Serial.println(F("[dB]"));
      delay(100);
      break;
  }

  switch (keyAnalog[DOWN].readEvent(330 - 50, 330 + 50)) {
    case SHORT_PRESS: {
        int _volume = musicPlayer.volumeDown();
        Serial.print(F("Volume changed to -"));
        Serial.print(_volume >> 1, 1);
        Serial.println(F("[dB]"));
      }
      break;
    case LONG_PRESS:
      int _volume = musicPlayer.volumeDown();
      Serial.print(F("Volume changed to -"));
      Serial.print(_volume >> 1, 1);
      Serial.println(F("[dB]"));
      delay(100);
      break;
  }

  switch (keyAnalog[LEFT].readEvent(512 - 50, 512 + 50)) {
    case SHORT_PRESS:
      musicNum--;
      if (musicNum < 1 ) {
        musicNum = fileNum;
      }
      playNum(musicNum);
      break;
  }

  switch (keyAnalog[RIGHT].readEvent(860 - 50, 860 + 50)) {
    case SHORT_PRESS:
      musicNum++;
      if (musicNum > fileNum) {
        musicNum = 1;
      }
      playNum(musicNum);
      break;
  }

  switch (keyAnalog[PRESS].readEvent(0, 50)) {
    case SHORT_PRESS:
      if (musicPlayer.stopped()) {
        Serial.println(F("Playing!"));
        playNum(musicNum);
      }
      else if (! musicPlayer.paused()) {
        Serial.println("Paused");
        musicPlayer.setAmplifier(false);	//关闭运放
        musicPlayer.pausePlaying(true);		//暂停
      } else {
        Serial.println("Resumed");
        musicPlayer.setAmplifier(true);		//开启运放
        musicPlayer.pausePlaying(false);	//取消暂停
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
