#include "audio.h"
#include <SoftwareSerial.h>

#include <Adafruit_NeoPixel.h>

#define humanHotSensor 4//PIR传感器D4
#define PIN 6//彩灯

Adafruit_NeoPixel strip = Adafruit_NeoPixel(2, PIN, NEO_GRB + NEO_KHZ800);

bool isUP = true;

int val = 0;
int music_vol = 26; //初始音量0~30
bool humanHotState = false;
boolean on_off;
boolean statusChange;
bool playing = false;

int pos = 0;
int soundNum = 1;

unsigned long Time_millis = millis();

void setup() {
  Serial.begin(9600);
  pinMode(humanHotSensor, INPUT);
  audio_init(DEVICE_Flash, 4, music_vol);  //初始化mp3模块
  //audio_init(DEVICE_TF, MODE_loopOne, music_vol);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  humanHotState = digitalRead(humanHotSensor);
  // print out the state of the button:
  //Serial.println(humanHotState);
  //delay(1);        // delay in between reads for stability

  if (humanHotState) {
    colorWipe(strip.Color(125, 125, 125));

    if (!playing) {
      Serial.println("play sound.....");
      // audio_play();
      audio_choose(1);
      playing = true;
    }
  }
  else
  {
    playing = false;
    //audio_pause();
    Serial.println("stop sound.....");
    colorWipe(strip.Color(0, 0, 0));
  }
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    //delay(wait);
  }
}
