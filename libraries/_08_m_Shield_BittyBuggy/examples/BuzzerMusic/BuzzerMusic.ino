#include <BittyBuggy.h>

#define mySerial Serial1 // Core+ D2,D3
BittyBuggy car(&mySerial);

#include "music.h"

void setup() {
  Serial.begin(57600);
  while (!car.begin())
    ;
}

void loop() {
  if (!playingStop())//播放音乐。
  {
    playSound(9);
  } else {
    //playingRest();
  }
}