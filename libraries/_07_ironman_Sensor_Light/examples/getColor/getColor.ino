#include <ironman_Sensor_Light.h>
ironmanLight light;

uint8_t colorAB[2];
uint8_t colordata[6];

void setup() {
  Serial.begin(115200);
  light.begin(0x03);
}

void loop() {
  if (light.available()) {
    light.getColor(colorAB);
    Serial.print(colorAB[0]);
    Serial.print(" , ");
    Serial.println(colorAB[1]);
    delay(100);
    
    /*
        light.getColorRaw(colordata);
        for (uint8_t i = 0; i < 6; i++) {
          Serial.print(colordata[i]);
          Serial.print(" , ");
        }
        Serial.println(" ");
        delay(100);
    */
    /*
        Serial.print(light.getColorA());
        Serial.print(" , ");
        Serial.println(light.getColorB());
        delay(100);
    */
  }
}
