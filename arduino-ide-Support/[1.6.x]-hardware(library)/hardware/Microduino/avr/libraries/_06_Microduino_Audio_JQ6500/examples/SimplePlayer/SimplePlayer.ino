#include "JQ6500.h"

//Core和CoreUSB默认使用软串口
SoftwareSerial mySerial(2, 3); // RX, TX
JQ6500 Audio(&mySerial);     //音频控制类实例,使用软串口（2，3）
//Core+默认使用Serial1
//JQ6500 Audio(&Serial1);

int musicVol = 20;             //初始音量20
int musicMode = MODE_ALL;      //初始播放模式--全部循环
boolean music_status = false;  //歌曲播放状态
int fileNum = 0;               //查询当前播放歌曲编号
int fileNumCache = 0;          //播放歌曲编号缓存
int totalNum = 0;              //歌曲总数量
int totalTime = 0;             //当前播放歌曲总时间
int playTime = 0;              //当前播放歌曲播放时间
int key, keyCache;

String nameCache = "";

void setup()
{
  Serial.begin(9600);

  Audio.init(DEVICE_TF, musicMode, musicVol);   //播放控制类初始化，设置播放设备为TF卡

  delay(1000);
  totalNum = Audio.queryTF();         //读取TF卡总文件数量
  Audio.choose(1);                  //默认从第一首曲目开始
  Audio.pause();                    //暂停
}

void loop()
{
  keyCache = key;
  key = analogRead(A6);               //获得按键指令

  if (key > 500)
  {
    if (keyCache < 50)              //按键按下
    {
      music_status = !music_status; //播放或暂停
      music_status ? Audio.play() : Audio.pause();
    }
    else if (keyCache > 50 && keyCache < 100)     //按键左拨
    {
      Audio.next();
    }
    else if (keyCache > 100 && keyCache < 200)    //按键右拨
    {
      Audio.prev();
    }
  }

  fileNum = Audio.queryTFFile();        //查询当前播放歌曲的编号
  delay(100);
  if (fileNum != fileNumCache) {      //发现播放歌曲更换
    fileNumCache = fileNum;
    nameCache = Audio.queryName();    //查询当前播放歌曲名称
    delay(100);
    totalTime = Audio.queryTotalTime();     //查询当前播放歌曲总时间
    delay(100);
  }
  playTime = Audio.queryPlayTime();       //查询当前播放歌曲播放时间
  delay(100);
  Serial.print("FileNum:");
  Serial.print(fileNum);
  Serial.print(", FileName:");
  Serial.print(nameCache);
  Serial.print(", TotleTime:");
  Serial.print(totalTime);
  Serial.print(", PlayTime:");
  Serial.println(playTime);
  delay(100);
}