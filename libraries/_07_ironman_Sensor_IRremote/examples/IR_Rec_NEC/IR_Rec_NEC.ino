#include <ironman_Sensor_IRremote.h>
ironmanIRremote IRremote;

void setup() {
  Serial.begin(115200);
  IRremote.begin(0x03);
  Serial.print("Start");
}

void loop() {
  
  if (IRremote.available()) {
    if (IRremote.irrecvAvailable()) {
      Serial.print("0x");
      Serial.println(IRremote.irrecvValue(), HEX);
    }
  }
  
}