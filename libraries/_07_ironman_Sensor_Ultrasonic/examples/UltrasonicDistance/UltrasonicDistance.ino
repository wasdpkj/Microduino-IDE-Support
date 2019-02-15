#include <ironman_Sensor_Ultrasonic.h>
ironmanUltrasonic Ultrasonic1;

void setup() {
  Serial.begin(115200);
  Ultrasonic1.begin(0x03);
}

void loop() {
  if (Ultrasonic1.available()) {
    Serial.print(Ultrasonic1.getDistance());
    Serial.println("mm");
    delay(100);
  }
}