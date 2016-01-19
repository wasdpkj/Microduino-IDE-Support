#include <Wire.h>
#include "Microduino_Matrix.h"

#define ADDR1 64
LedControl MatrixA = LedControl(ADDR1);
//#define ADDR2 63
//LedControl MatrixB = LedControl(ADDR2);

void setup() { // initalizes and sets up the initial values. Declaring function setup.
  Serial.begin(9600); // setting data rate as 9600 bits per second for serial data communication to computer
  //  delay(6000);
  Serial.println("Setup"); //prints data to serial port as human-readable text
  Wire.begin();

  MatrixA.clearDisplay(); //clear the display after each letter
  MatrixA.setColor(255, 255, 255); //clear the display after each letter
  //  MatrixA.writeString(ADDR1, 20, "Hello Microduino!");
  for (int a = 0; a < 64; a++)
  {
    MatrixA.setCursor(a, 0);
    MatrixA.print("HELLO mCookie!");
    delay(50);
  }
  MatrixA.clearDisplay(); //clear the display after each letter
}

void loop() { //declaring function loop
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      randomSeed(analogRead(A0));
      MatrixA.setLedColor(x, y, random(0, 255), random(0, 255), random(0, 255));
      delay(20);
    }
  }
  delay(2000);

  MatrixA.clearDisplay(); //clear the display after each letter
  MatrixA.setColor(255, 0, 0); //clear the display after each letter
  MatrixA.writeString(20, "RED");
  delay(500);

  MatrixA.clearDisplay(); //clear the display after each letter
  MatrixA.setColor(0, 255, 0); //clear the display after each letter
  MatrixA.writeString(20, "GREEN");
  delay(500);

  MatrixA.clearDisplay(); //clear the display after each letter
  MatrixA.setColor(0, 0, 255); //clear the display after each letter
  MatrixA.writeString(20, "BLUE");
  delay(500);

  MatrixA.clearDisplay(); //clear the display after each letter
  MatrixA.clearColor(); //clear the display after each letter
  MatrixA.writeString(20, "Null");
  delay(500);

  MatrixA.clearDisplay();
}