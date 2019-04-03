#include <ironman_Sensor_IRremote.h>
ironmanIRremote IRremote;

void setup() {
  Serial.begin(115200);
  IRremote.begin(0x03);
  pinMode(6, INPUT_PULLUP);
  Serial.println("Start");
}

void loop() {

  if (IRremote.available()) {
    if (!digitalRead(6)) {
      IRremote.sendNEC(0x1FEA05F);
      Serial.println("Send...");
    }
  }
}
