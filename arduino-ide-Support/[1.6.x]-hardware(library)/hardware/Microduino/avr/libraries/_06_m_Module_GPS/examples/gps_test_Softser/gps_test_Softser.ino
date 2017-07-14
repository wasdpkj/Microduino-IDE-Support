#include <Microduino_GPS.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // RX, TX
Microduino_GPS GPS(&mySerial);

void setup() {
  Serial.begin(115200);
  GPS.begin(38400);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    mySerial.write(c);
  }
  if (mySerial.available()) {
    char c = mySerial.read();
    Serial.write(c);
  }
}
