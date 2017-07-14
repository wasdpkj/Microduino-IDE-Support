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
  
  // Adjust the Proximity sensor gain
  if ( !gestureSensor.setProximityGain(PGAIN_2X) ) {
    Serial.println(F("Something went wrong trying to set PGAIN"));
  }
  
  // Start running the Gesture proximity sensor (no interrupts)
  if ( gestureSensor.enableProximitySensor(false) ) {
    Serial.println(F("Proximity sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during sensor init!"));
  }
}

void loop() { 
  Serial.print("Proximity: ");
  Serial.println(gestureSensor.readProximity());
  
  // Wait 250 ms before next reading
  delay(250);
}