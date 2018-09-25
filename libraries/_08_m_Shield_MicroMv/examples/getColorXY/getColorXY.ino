#include <Microduino_MicroMV.h>
MicroMV microMv;


void setup() {
  Serial.begin(115200);
  if (microMv.Init()) {
    Serial.println("microMv online");
  }
  else {
    Serial.println("microMv offline");
    while (1);
  }
}

void loop() {
  Serial.println();
  Serial.println();
  Serial.print("microMv:");
  if (microMv.getData()) {
    Serial.println("get Data");
    if (microMv.getTarget(THE_RED)) {
      Serial.print("get red x:");
      Serial.print(microMv.getX(THE_RED));
      Serial.print(" y:");
      Serial.println(microMv.getY(THE_RED));
    }
    else {
      Serial.println("no red");
    }
    if (microMv.getTarget(THE_GREEN)) {
      Serial.print("get green x:");
      Serial.print(microMv.getX(THE_GREEN));
      Serial.print(" y:");
      Serial.println(microMv.getY(THE_GREEN));
    }
    else {
      Serial.println("no green");
    }
    if (microMv.getTarget(THE_BLUE)) {
      Serial.print("get blue x:");
      Serial.print(microMv.getX(THE_BLUE));
      Serial.print(" y:");
      Serial.println(microMv.getY(THE_BLUE));
    }
    else {
      Serial.println("no blue");
    }
  }
  else {
    Serial.println("no Data");
  }
  delay(100);
}


