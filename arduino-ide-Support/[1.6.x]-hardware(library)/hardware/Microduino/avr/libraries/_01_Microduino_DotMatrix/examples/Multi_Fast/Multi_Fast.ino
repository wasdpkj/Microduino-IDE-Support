#include <Wire.h>
#include "Microduino_Matrix.h"

LedControl MatrixA = LedControl(61);
LedControl MatrixB = LedControl(62);
LedControl MatrixC = LedControl(63);
LedControl MatrixD = LedControl(64);

LedControl led = 0;

void setup() { // initalizes and sets up the initial values. Declaring function setup.
  Serial.begin(9600); // setting data rate as 9600 bits per second for serial data communication to computer
  //  delay(6000);
  Serial.println("Setup"); //prints data to serial port as human-readable text
  Wire.begin();
}

void loop() { //declaring function loop
  for (int a = 0; a < 4; a++)
  {
    switch (a)
    {
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
    led.clearColor();
	led.setFastMode();	//On Fast mode
	//led.clearFastMode();	//Off Fast Mode
    led.setColor(0, 255, 255);
  }

  int x = 0;
  for (int a = 0; a < 64; a++)
  {
    for (int b = 0; b < 4; b++)
    {
      switch (b)
      {
        case 0:
          led = MatrixA;
          x = 0;
          break;
        case 1:
          led = MatrixB;
          x = 8;
          break;
        case 2:
          led = MatrixC;
          x = 16;
          break;
        case 3:
          led = MatrixD;
          x = 24;
          break;
      }
      led.setCursor(x + a, 0);
      led.print("HELLO mCookie");
      //      delay(50);
    }
  }

  for (int a = 0; a < 4; a++)
  {
    switch (a)
    {
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
      for (int a = 0; a < 4; a++)
      {
        switch (a)
        {
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
        led.setLedColorFast(x, y, random(0, 255), random(0, 255), random(0, 255));
        //led.setLedColor(x, y, random(0, 255), random(0, 255), random(0, 255));
      }
      delay(20);
    }
  }
  delay(1000);

  for (int a = 0; a < 4; a++)
  {
    switch (a)
    {
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

}