/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  Simple e-mail example

  App project setup:
    E-mail Widget

  Connect a button to digital pin 2 and GND
  Pressing this button will send an e-mail

  WARNING: You are limited to send ONLY ONE E-MAIL PER 15 SECONDS!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

/**
**CoreUSB UART Port: [Serial1] [D0,D1]
**Core+ UART Port: [Serial1] [D2,D3]
**/
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define EspSerial Serial1
#define ESP8266_BAUD  115200
#endif

/**
**Core UART Port: [SoftSerial] [D2,D3]
**/
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); /* RX:D2, TX:D3 */
#define ESP8266_BAUD  9600
#endif

ESP8266 wifi(&EspSerial);

void emailOnButtonPress()
{
  // *** WARNING: You are limited to send ONLY ONE E-MAIL PER 15 SECONDS! ***

  // Let's send an e-mail when you press the button
  // connected to digital pin 2 on your Arduino

  int isButtonPressed = !digitalRead(2); // Invert state, since button is "Active LOW"

  if (isButtonPressed) // You can write any condition to trigger e-mail sending
  {
    Serial.println("Button is pressed."); // This can be seen in the Serial Monitor
    Blynk.email("your_email@mail.com", "Subject: Button Logger", "You just pushed the button...");

    // Or, if you want to use the email specified in the App (like for App Export):
    //Blynk.email("Subject: Button Logger", "You just pushed the button...");
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  delay(10);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);

  // Send e-mail when your hardware gets connected to Blynk Server
  // Just put the recepient's "e-mail address", "Subject" and the "message body"
  Blynk.email("your_email@mail.com", "Subject", "My Blynk project is online.");

  // Setting the button
  pinMode(2, INPUT_PULLUP);
  // Attach pin 2 interrupt to our handler
  attachInterrupt(digitalPinToInterrupt(2), emailOnButtonPress, CHANGE);
}

void loop()
{
  Blynk.run();
}

