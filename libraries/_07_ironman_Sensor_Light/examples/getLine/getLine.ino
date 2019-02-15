#include <ironman_Sensor_Light.h>
ironmanLight light;

uint8_t line[2];

void setup() {
  Serial.begin(115200);
  light.begin(0x03);
}

void loop() {
  if (light.available()) {
    light.getLine(line, RED, RED);
    Serial.print(line[0]);
    Serial.print(" , ");
    Serial.println(line[1]);
    delay(100);

    //  Serial.print(light.getLineA(RED));
    //  Serial.print(" , ");
    //  Serial.println(light.getLineB(GREEN));
    //  delay(100);
  }
}