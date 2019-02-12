#include <ironman_shield_coreNrf.h>

ironmanShieldNrf shieldNrf;

void setup() {
  Serial.begin(115200);
  Serial.println(shieldNrf.begin());
  Serial.println("Start");
}

void loop() {
  Serial.print("X:");
  Serial.print(shieldNrf.readKeyX());
  Serial.print("  Y:");
  Serial.println(shieldNrf.readKeyY());
}
