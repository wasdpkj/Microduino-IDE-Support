#include <Microduino_Gesture.h>

// Global Variables
Gesture gestureSensor;


void setup() {

  // Initialize Serial port
  Serial.begin(9600);

  // Initialize Gesture (configure I2C and initial values)
  if ( gestureSensor.begin() ) {
    Serial.println(F("Gesture initialization complete"));
  } else {
    Serial.println(F("Something went wrong during Gesture init!"));
  }

  // Start running the Gesture light sensor (no interrupts)
  if ( gestureSensor.enableLightSensor(false) ) {
    Serial.println(F("Light sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during light sensor init!"));
  }

  // Wait for initialization and calibration to finish
  delay(500);
}

void loop() {
  // Read the light levels (ambient, red, green, blue)

  Serial.print("Ambient: ");
  Serial.print(gestureSensor.readAmbientLight());
  Serial.print(" Red: ");
  Serial.print(gestureSensor.readRedLight());
  Serial.print(" Green: ");
  Serial.print(gestureSensor.readGreenLight());
  Serial.print(" Blue: ");
  Serial.println(gestureSensor.readBlueLight());

  // Wait 1 second before next reading
  delay(1000);
}