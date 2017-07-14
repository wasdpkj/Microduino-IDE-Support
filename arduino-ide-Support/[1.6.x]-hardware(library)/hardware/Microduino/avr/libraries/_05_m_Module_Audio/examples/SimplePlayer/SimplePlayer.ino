#include "Microduino_Audio.h"

#include <Microduino_Key.h>

AnalogKey keyA6[3] {(A6), (A6), (A6)};

//Core UART Port: [SoftSerial] [D2,D3]
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D2, TX:D3 */
#define AudioSerial mySerial
#endif

//Core+ UART Port: [Serial1] [D2,D3]
#if defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define AudioSerial Serial1
#endif

Audio Audio(&AudioSerial);

uint8_t musicVol = 20;             //初始音量20
uint8_t musicMode = MODE_ALL;      //初始播放模式--全部循环
boolean music_status = false;      //歌曲播放状态
uint16_t fileNum = 0;               //查询当前播放歌曲编号
uint16_t fileNumCache = 0;          //播放歌曲编号缓存
uint16_t totalNum = 0;              //歌曲总数量
uint16_t totalTime = 0;             //当前播放歌曲总时间
uint16_t playTime = 0;              //当前播放歌曲播放时间

uint32_t time1 = 0;

String nameCache = "";

void setup() {
  Serial.begin(9600);
  Serial.println("Hello");   //短按

  keyA6[0].begin();
  keyA6[1].begin();
  keyA6[2].begin();

  Serial.println("init Audio");   //短按
  Audio.init(DEVICE_TF, musicMode, musicVol);   //播放控制类初始化，设置播放设备为TF卡

  delay(1000);
  totalNum = Audio.queryTF();         //读取TF卡总文件数量
  Audio.choose(1);                  //默认从第一首曲目开始
  Audio.pause();                    //暂停
  Serial.println("Done");   //短按
}

void loop() {
  switch (keyA6[0].readEvent(140 - 30, 140 + 30)) {
    case SHORT_PRESS:
      Audio.prev();
      Serial.println("prev");   //短按
      break;
    case LONG_PRESS:
      Audio.volUp();
      Serial.println("volUp");    //长按
	  delay(500);
      break;
  }

  switch (keyA6[1].readEvent(80 - 30, 80 + 30)) {
    case SHORT_PRESS:
      Audio.next();
      Serial.println("next");   //短按
      break;
    case LONG_PRESS:
      Audio.volDown();
      Serial.println("volDown");    //长按
	  delay(500);
      break;
  }

  switch (keyA6[2].readEvent(0, 30)) {
    case SHORT_PRESS:
      music_status = !music_status; //播放或暂停
      if (music_status) {
        Audio.play();
        Serial.println("play");
      }
      else {
        Audio.pause();
        Serial.println("pause");
      };
      break;
  }

  if (millis() - time1 > 1000) {
    time1 = millis();

    fileNum = Audio.queryTFFile();        //查询当前播放歌曲的编号
    if (fileNum != fileNumCache) {      //发现播放歌曲更换
      fileNumCache = fileNum;
      nameCache = Audio.queryName();    //查询当前播放歌曲名称
      totalTime = Audio.queryTotalTime();     //查询当前播放歌曲总时间
    }
    playTime = Audio.queryPlayTime();       //查询当前播放歌曲播放时间
    Serial.print("FileNum:");
    Serial.print(fileNum);
    Serial.print(", FileName:");
    Serial.print(nameCache);
    Serial.print(", TotleTime:");
    Serial.print(totalTime);
    Serial.print(", PlayTime:");
    Serial.println(playTime);
  }
  
  delay(10);
}