//This example is just for Microduino/mCookie Core+/CoreRF
//More information please visit https://wiki.microduino.cn/index.php/MCookie-GPS

#include <Adafruit_GPS.h>
Adafruit_GPS GPS(&Serial1);

void setup() {
  Serial.begin(115200);
  GPS.begin(38400);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    Serial1.write(c);
  }
  if (Serial1.available()) {
    char c = Serial1.read();
    Serial.write(c);
  }
}
