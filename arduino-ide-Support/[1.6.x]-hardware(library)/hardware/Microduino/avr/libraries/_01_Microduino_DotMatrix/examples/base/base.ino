#include <Wire.h>
#include "Microduino_Matrix.h"
#include "Microduino_MatrixMultiple.h"

LedMultiple display = LedMultiple(1, 1);

#define ADDR1 64
LedControl MatrixA = LedControl(ADDR1);

void setup() { // initalizes and sets up the initial values. Declaring function setup.
  Serial.begin(9600); // setting data rate as 9600 bits per second for serial data communication to computer
  //  delay(6000);
  Serial.println("Setup"); //prints data to serial port as human-readable text
  Wire.begin();

  MatrixA.clearDisplay(); //clear the display after each letter
  //MatrixA.setFastMode();  //On Fast mode
  MatrixA.clearFastMode();  //Off Fast Mode
  MatrixA.setColor(255, 255, 255);
  //  MatrixA.writeString(ADDR1, 20, "Hello Microduino!");
  for (int a = display.getStringLeng("Hello mCookie!"); a > -8; a--)
  {
    MatrixA = MatrixA;
    MatrixA.setCursor(0, 0);
    MatrixA.setCursor(a - display.getStringLeng("Hello mCookie!") + 8, 0);
    //led.print("!B");
    MatrixA.print("Hello mCookie!");
    delay(200);
  }
  MatrixA.clearDisplay();
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

  MatrixA.clearDisplay();
  MatrixA.setColor(255, 0, 0);
  MatrixA.writeString(20, "RED");
  delay(500);

  MatrixA.clearDisplay();
  MatrixA.setColor(0, 255, 0);
  MatrixA.writeString(20, "GREEN");
  delay(500);

  MatrixA.clearDisplay();
  MatrixA.setColor(0, 0, 255);
  MatrixA.writeString(20, "BLUE");
  delay(500);

  MatrixA.clearDisplay();
  MatrixA.clearColor();
  MatrixA.writeString(20, "Null");
  delay(500);

  MatrixA.clearDisplay();
}