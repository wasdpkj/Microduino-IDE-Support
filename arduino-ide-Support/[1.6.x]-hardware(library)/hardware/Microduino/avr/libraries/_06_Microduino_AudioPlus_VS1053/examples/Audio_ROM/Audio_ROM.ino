/*
 * http://wiki.microduino.cn/index.php/Microduino-Module_AudioPro/zh
 */
#include "file.h"
#include <avr/pgmspace.h>
#include <SPI.h>
#include <Adafruit_VS1053.h>

// These are the pins used for the breakout example
#define BREAKOUT_RESET  -1      // VS1053 reset pin (output)
#define BREAKOUT_CS     A3     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    A2      // VS1053 Data/command select pin (output)
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin
Adafruit_VS1053 player = Adafruit_VS1053(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ);

#define BUFFERNUM 256 // <256
bool playFile(const uint8_t *_buffer, unsigned long _len) {
  unsigned long data_num = 0;
  Serial.println(_len);
  while (data_num < _len - 1) {
    uint8_t buffer[BUFFERNUM];
    unsigned long cache_num = min(_len - data_num, BUFFERNUM);
    Serial.print("cache_num:");
    Serial.println(cache_num);
    for (unsigned long a = 0; a < cache_num; a++) {
      buffer[a] = pgm_read_byte(_buffer + data_num);
      data_num++;
    }
    Serial.print("data_num:");
    Serial.println(data_num);
    player.playBuffer(buffer, cache_num);   //...send them to VS1053B
  }
  Serial.println("Done");
}

void setup() {
  Serial.begin(9600);
  delay(10);
  //while (!Serial);
  SPI.begin();            //Start Serial Peripheral Interface (SPI)
  if (! player.begin()) {  // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }
  Serial.println(F("VS1053 found"));
  // Set volume for left, right channels. lower numbers == louder volume!
  player.setVolume(20, 20);
  player.GPIO_pinMode(4, OUTPUT);   //Amplifier
}

uint8_t cache[1024];
unsigned long cache_num = 0;
void loop() {
  player.stopPlaying();
  player.softReset();

  player.GPIO_digitalWrite(4, LOW);  //Amplifier ON

  delay(1);    while (! player.readyForData());
  playFile(Array, sizeof(Array));
}
