/**
   @example test.ino
   @brief The test demo of library WeeESP8266.
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
  
  Serial.println(wifi.getWifiModeList().c_str());
  if (wifi.setOprToSoftAP(2, 2)) {
    Serial.println(F("it is STA"));
  }
  if (wifi.setOprToStation(2, 2)) {
    Serial.println(F("it is AP"));
  }
  if (wifi.setOprToStationSoftAP(2, 2)) {
    Serial.println(F("it is AP+SoftAP"));
  }
  Serial.println(wifi.getAPList().c_str());
  wifi.joinAP(SSID, PASSWORD);
  Serial.println(wifi.getNowConecAp(1).c_str());
  if (wifi.leaveAP())
  {
    Serial.println(F("it is leave"));
  }
  Serial.println(wifi.getNowConecAp(1).c_str());
  if (wifi.setSoftAPParam("aaa", "12345678"))
  {
    Serial.println(F("it is set param ok"));
  }
  Serial.println(wifi.getSoftAPParam());
  Serial.println(wifi.getJoinedDeviceIP().c_str());
  Serial.print(F("the state of DHCP:"));
  Serial.println(wifi.getDHCP().c_str());
  if (wifi.setDHCP(2, 1)) {
    Serial.println(F("it is set DHCP OK"));
  }
  Serial.println(wifi.getDHCP().c_str());
  if (wifi.setAutoConnect(false)) {
    Serial.println(F("take off auto connect ok"));
  }
  Serial.print(F("get the station mac: "));
  Serial.println(wifi.getStationMac().c_str());
  // if (wifi.setStationMac("18:fe:35:98:d3:7b")) {
  //   Serial.println(F("set station mac is ok "));
  // }
  // else {
  //   Serial.println(F("it is error"));
  // }
  // Serial.print(F("get the station mac: "));
  // Serial.println(wifi.getStationMac().c_str());
  Serial.print(F("get the station's ip"));
  Serial.println(wifi.getStationIp().c_str());

  if (wifi.setStationIp("192.168.1.6", "192.168.1.1", "255.255.255.0")) {
    Serial.println(F("set station's ip is ok"));
  }
  else {
    Serial.println(F("set station's ip is error"));
  }
  Serial.print(F("get the station's ip"));
  Serial.println(wifi.getStationIp().c_str());

  Serial.print(F("get the ap's ap"));
  Serial.println(wifi.getAPIp().c_str());

  if (wifi.setAPIp("192.168.1.1")) {
    Serial.println(F("set ap's ip is ok"));
  }
  else
  {
    Serial.println(F("set ap's is is error"));
  }
  Serial.print(F("get the ap's ap"));
  Serial.println(wifi.getAPIp().c_str());

  if (wifi.startSmartConfig(1)) {
    Serial.println(F("start smartconfig is ok"));
  }
  else {
    Serial.println(F("start smartconfig is error"));
  }
  if (wifi.stopSmartConfig()) {
    Serial.println(F("stop smartconfig is ok"));
  }
  else {
    Serial.println(F("stop smartconfig is error"));
  }
  Serial.print(F("get the current status of connection:"));
  Serial.println(wifi.getIPStatus().c_str());
  // if (wifi.saveTransLink(1, "192.168.1.18", 1006)) {
  //   Serial.println(F("save trans link is ok"));
  // }
  // else {
  //   Serial.println(F("save trans link is error"));
  // }
  Serial.println(F("setup end\r\n"));
}

void loop(void)
{
}
