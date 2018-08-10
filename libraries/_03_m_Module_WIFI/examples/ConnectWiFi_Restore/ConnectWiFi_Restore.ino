/**
   This command is used to reset all parameters saved in flash (according to appendix).
   restore the factory default settings of the module.
   The chip will be restarted when this command is executed.
*/
#include <ESP8266.h>

#ifdef ESP32
#error "This code is not recommended to run on the ESP32 platform! Please check your Tools->Board setting."
#endif

/**
**CoreUSB UART Port: [Serial1] [D0,D1]
**Core+ UART Port: [Serial1] [D2,D3]
**/
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
#define EspSerial Serial1
#define UARTSPEED  115200
#endif

/**
**Core UART Port: [SoftSerial] [D2,D3]
**/
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D2, TX:D3 */

#define EspSerial mySerial
#define UARTSPEED  9600
#endif

#define SSID        "Makermodule"
#define PASSWORD    "microduino"

ESP8266 wifi(&EspSerial);

void setup(void)
{
  Serial.begin(115200);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately
  Serial.print(F("setup begin\r\n"));
  delay(100);

  WifiInit(EspSerial, UARTSPEED);

  if (wifi.restore()) {
    Serial.println("Module restore ok");
  }
  else {
    Serial.println("Module restore err");
  }
  delay(2000);

  Serial.print(F("FW Version: "));
  Serial.println(wifi.getVersion().c_str());


  if (wifi.setOprToStation()) {
    Serial.print(F("to station ok\r\n"));
  } else {
    Serial.print(F("to station err\r\n"));
  }

error:
  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print(F("Join AP success\r\n"));
    Serial.print(F("IP: "));
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print(F("Join AP failure\r\n"));
    goto error;
  }

  Serial.print(F("setup end\r\n"));
}

void loop(void)
{
}
