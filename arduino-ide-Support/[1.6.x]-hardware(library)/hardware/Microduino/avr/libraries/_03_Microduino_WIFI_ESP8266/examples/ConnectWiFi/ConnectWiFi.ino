/**
   @example ConnectWiFi.ino
   @brief The ConnectWiFi demo of library WeeESP8266.
   @author Wu Pengfei<pengfei.wu@itead.cc>
   @date 2015.03

   @par Copyright:
   Copyright (c) 2015 ITEAD Intelligent Systems Co., Ltd. \n\n
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version. \n\n
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/
#include "ESP8266.h"

uint16_t baud_data[5] = {9600, 19200, 38400, 57600, 115200};

//CoreUSB UART Port: [Serial1] [D0,D1]
#if defined(__AVR_ATmega32U4__)
#define EspSerial Serial1
#define UARTSPEED  115200
#endif

//Core+ UART Port: [Serial1] [D2,D3]
#if defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define EspSerial Serial1
#define UARTSPEED  115200
#endif


//Core UART Port: [SoftSerial] [D2,D3]
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D3, TX:D2 */
#define EspSerial mySerial
#define UARTSPEED  19200
#endif


#define SSID        "Microduino_IoT"
#define PASSWORD    "MakerModule2017"

ESP8266 wifi(&EspSerial);

void setup(void)
{
  Serial.begin(115200);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately
  Serial.print("setup begin\r\n");
  delay(100);

  for (int a = 0; a < 5; a++) {
    EspSerial.begin(baud_data[a]);
    delay(100);
    while (EspSerial.available() > 0) {
      EspSerial.read();
    }

    wifi.setUart(UARTSPEED, DEFAULT_PATTERN);
  }

  EspSerial.begin(UARTSPEED);
  delay(100);
  while (EspSerial.available() > 0) {
    EspSerial.read();
  }

  Serial.print("FW Version: ");
  Serial.println(wifi.getVersion().c_str());


  if (wifi.setOprToStation()) {
    Serial.print("to station ok\r\n");
  } else {
    Serial.print("to station err\r\n");
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print("Join AP success\r\n");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
  }

  Serial.print("setup end\r\n");
}

void loop(void)
{
}

