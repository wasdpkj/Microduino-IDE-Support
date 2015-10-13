#include <Adafruit_NeoPixel.h>

#include <Wire.h>
#include "I2Cdev.h"

#include <AM2321.h>
AM2321 am2321;

#define PIXEL_PIN    6    // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT  6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

#define temp1    28
#define temp2    30
#define temp3    32

float sensor_tem;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  readByAM2321();
  if (sensor_tem <= temp1)
    colorSet(strip.Color(0, 255, 0));
  
  else if (sensor_tem > temp1 && sensor_tem <= temp2)
    colorSet(strip.Color(0, 0, 255));
  
  else if (sensor_tem > temp2 && sensor_tem <= temp3)
    colorSet(strip.Color(255, 255, 0));
  
  else
    colorSet(strip.Color(255, 0, 0));
}

void readByAM2321()
{
  AM2321 am2321;
  am2321.read();
  sensor_tem = am2321.temperature / 10.0;
  Serial.println(sensor_tem);
  delay(100);
}

void colorSet(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    //delay(wait);
  }
}
