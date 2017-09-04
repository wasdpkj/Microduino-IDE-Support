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

  if (gestureSensor.setGestureGain(GGAIN_2X)) { //GGAIN_1X GGAIN_2X GGAIN_4X GGAIN_8X
    Serial.println(F("Gesture setGestureGain complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 setGestureGain!"));
  }

  // Start running the Gesture gesture sensor engine
  if ( gestureSensor.enableGestureSensor(true) ) {
    Serial.println(F("Gesture sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
}

void loop() {
  if ( gestureSensor.isGestureAvailable() ) {
    switch ( gestureSensor.readGesture() ) {
      case DIR_UP:
        Serial.println("UP");
        break;
      case DIR_DOWN:
        Serial.println("DOWN");
        break;
      case DIR_LEFT:
        Serial.println("LEFT");
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        break;
      case DIR_NEAR:
        Serial.println("NEAR");
        break;
      case DIR_FAR:
        Serial.println("FAR");
        break;
      default:
        Serial.println("NONE");
    }
  }
}