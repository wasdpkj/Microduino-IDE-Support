#include <ironman_shield_coreNrf.h>

ironmanShieldNrf shieldNrf;

void setup() {
  Serial.begin(115200);
  Serial.println(shieldNrf.begin());
  Serial.println("Start");
}

void loop() {
  uint8_t micdata = shieldNrf.micVal();
  Serial.println(micdata);
  delay(1);
}