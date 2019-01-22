#include <ironman_Sensor_ColorLED.h>
ironmanColorLED ColorLED;


void setup() {
  Serial.begin(115200);
  ColorLED.begin(0x03);
  ColorLED.setBrightness(25);
  ColorLED.clear();
  delay(500);
}

void loop() {
  if (ColorLED.available()) {
    Serial.println("Welcome AVR Core...");
    ColorLED.setAllLED(255, 0, 0);
    delay(500);
    ColorLED.setAllLED(0, 255, 0);
    delay(500);
    ColorLED.setAllLED(0, 0, 255);
    delay(500);
    ColorLED.clear();
    delay(500);
    ColorLED.setAllLED(0x00FF00);
    delay(500);
    ColorLED.setAllLED(0x0000FFF);
    delay(500);
    ColorLED.setAllLED(0xFF0000);
    delay(500);
    ColorLED.clear();
    delay(500);
    ColorLED.setPixelColor(1, 255, 0, 255);
    ColorLED.setPixelColor(2, 0, 255, 255);
    ColorLED.setPixelColor(3, 255, 0, 255);
    ColorLED.setPixelColor(4, 0, 255, 255);
    delay(500);
    ColorLED.clear();
    delay(500);
    ColorLED.setPixelColor(1, 0x00FFFF);
    ColorLED.setPixelColor(2, 0xFFFF00);
    ColorLED.setPixelColor(3, 0x00FFFF);
    ColorLED.setPixelColor(4, 0xFFFF00);
    delay(500);
    ColorLED.clear();
    delay(500);
    ColorLED.setOneLED(1, 0xFFFFFF);
    ColorLED.setOneLED(2, 0xFFFFFF);
    ColorLED.setOneLED(3, 0xFFFFFF);
    ColorLED.setOneLED(4, 0xFFFFFF);
    delay(500);
    ColorLED.clear();
    delay(500);
  }
}
