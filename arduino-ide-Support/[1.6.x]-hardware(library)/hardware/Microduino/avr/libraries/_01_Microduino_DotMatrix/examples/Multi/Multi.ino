#include <Wire.h>
#include "Microduino_Matrix.h"
#include "Microduino_MatrixMultiple.h"

LedMultiple display = LedMultiple(4, 1);

LedControl MatrixA = LedControl(64);
LedControl MatrixB = LedControl(63);
LedControl MatrixC = LedControl(62);
LedControl MatrixD = LedControl(61);

LedControl led = 0;

void setup() { // initalizes and sets up the initial values. Declaring function setup.
  Serial.begin(9600); // setting data rate as 9600 bits per second for serial data communication to computer
  //  delay(6000);
  Serial.println("Setup"); //prints data to serial port as human-readable text
  Wire.begin();
  
  for (int a = 0; a < 4; a++) {
    switch (a) {
      case 0:
        led = MatrixA;
        break;
      case 1:
        led = MatrixB;
        break;
      case 2:
        led = MatrixC;
        break;
      case 3:
        led = MatrixD;
        break;
    }
    led.clearDisplay();
    led.clearColor();
    led.setFastMode();  //On Fast mode
    //led.clearFastMode();  //Off Fast Mode
    led.setColor(0, 255, 255);
  }
}

void loop() { //declaring function loop
  for (int a = 0; a < 4; a++) {
    switch (a) {
      case 0:
        led = MatrixA;
        break;
      case 1:
        led = MatrixB;
        break;
      case 2:
        led = MatrixC;
        break;
      case 3:
        led = MatrixD;
        break;
    }
    led.clearDisplay();
  }

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      for (int a = 0; a < 4; a++) {
        switch (a) {
          case 0:
            led = MatrixA;
            break;
          case 1:
            led = MatrixB;
            break;
          case 2:
            led = MatrixC;
            break;
          case 3:
            led = MatrixD;
            break;
        }
        randomSeed(analogRead(A0));
        //led.setLedColorFast(x, y, random(0, 255), random(0, 255), random(0, 255));
        led.setLedColor(x, y, random(0, 255), random(0, 255), random(0, 255));
      }
      delay(20);
    }
  }
  delay(1000);

  int x = 0;
  for (int a = display.getStringLeng("Hello mCookie!"); a > -32; a--) {
    for (int b = 0; b < 4; b++) {
      switch (b) {
        case 0:
          led = MatrixA;
          x = 24;
          break;
        case 1:
          led = MatrixB;
          x = 16;
          break;
        case 2:
          led = MatrixC;
          x = 8;
          break;
        case 3:
          led = MatrixD;
          x = 0;
          break;
      }
      led.setCursor(a - display.getStringLeng("Hello mCookie!") + 32 - x, 0);
      led.print("Hello mCookie!");
      //      delay(5);
    }
  }
}