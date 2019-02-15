#include <ironman_Sensor_Light.h>
ironmanLight light;

void setup() {
  Serial.begin(115200);
  light.begin(0x03);
}

void loop() {
  if (light.available()) {
    light.setAllLED(5, 0, 0);
    delay(300);
    light.setAllLED(0, 5, 0);
    delay(300);
    light.setAllLED(0, 0, 5);
    delay(300);
    //  light.setColorLEDA(5, 0, 0);
    //  light.setColorLEDB(5, 0, 0);
    //  light.setColorLEDA(0x080000);
    //  light.setColorLEDB(0x000008);
  }
}
