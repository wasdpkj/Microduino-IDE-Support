#include <Microduino_Ai.h>
#include <aJSON.h>

#define WIFI_SSID   "ssid"
#define WIFI_PWD    "password"
#define MQTTSERVER   "mqtt://mCotton.microduino.cn"
#define ID   "58f881ae42c66e00013cf445"
#define USER   "58f881ae42c66e00013cf445"
#define PASS   "Secure Token"
#define SUBSCRIBTOPIC   "ca/58f881ae42c66e00013cf445"
#define SUBSCRIBTOPICP   "cp/58f881ae42c66e00013cf445"
#define PUBLISHTOPIC   "dp/58f881ae42c66e00013cf445"

#define INTERVAL_sensor 2000
unsigned long sensorlastTime = millis();

//Core UART Port: [SoftSerial] [D2,D3]
#if defined (__AVR_ATmega168__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D2, TX:D3 */
MicroAi mAi(&mySerial);
#endif

//Core+ UART Port: [Serial1] [D2,D3]
#if defined(__AVR_ATmega1284P__) || defined (__AVR_ATmega644P__) || defined(__AVR_ATmega128RFA1__)
MicroAi mAi(&Serial1);
#endif

#if defined (ESP32)
HardwareSerial mySerial1(1);  //UART1
MicroAi mAi(&mySerial1, D2, D3);
#endif

String jsonData;
char strBuf[64] = {0};

void setup() {
  Serial.begin(115200);

  Serial.print("MicroAi set mode: MQTT.");
  Serial.println("start initiation. ");
  if (mAi.begin(MODE_MQTT)) {
    Serial.println("init OK!");
  } else {
    Serial.println("init failed!");
    while (1);
  }

  if (mAi.setWifi(WIFI_SSID, WIFI_PWD)) {
    Serial.println("set wifi ssid and password OK!");
  } else {
    Serial.println("set wifi ssid and password failed!");
    while (1);
  }
  Serial.print("wifi connecting");
  while (mAi.getRssi() == 0) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\r\nwifi connected.");

  if (mAi.mqttSetServer(MQTTSERVER)) {
    Serial.println(F("set mqtt server OK!"));
  } else {
    Serial.println(F("set mqtt server failed!"));
    while (1);
  }

  if (mAi.mqttConnect(ID, USER, PASS)) {
    Serial.println(F("set mqtt connect params OK!"));
  } else {
    Serial.println(F("set mqtt connect params failed!"));
    while (1);
  }

  Serial.print("mqtt server connecting");
  while (mAi.mqttGetStatus() <= 0) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\r\nmqtt server connected.");

  if (mAi.mqttSetSubscrib(SUBSCRIBTOPIC)) {
    Serial.println(F("set mqtt subscrib ca topic OK!"));
  } else {
    Serial.println(F("set mqtt subscrib ca topic failed"));
  }

  if (mAi.mqttSetSubscrib(SUBSCRIBTOPICP)) {
    Serial.println(F("set mqtt subscrib cp topic OK!"));
  } else {
    Serial.println(F("set mqtt subscrib cp topic failed"));
  }

  char *str = "{\"Humidity\":66.66}";
  if (mAi.mqttPublish(PUBLISHTOPIC, str)) {
    Serial.println(F("mqtt publish OK!"));
  } else {
    Serial.println(F("mqtt publish failed!"));
  }

  delay(1000);
}

void loop() {

  if (mAi.mqttQuery(strBuf) > 0) {
    Serial.print("recv: ");
    Serial.println(strBuf);
  }

  if (mAi.mqttGetStatus() > 0) {
    if (sensorlastTime > millis())
      sensorlastTime = millis();
    if (millis() - sensorlastTime > INTERVAL_sensor) {
      float temp = map(analogRead(A2), 0, 1023, 0, 255);
      float humi = map(analogRead(A1), 0, 1023, 0, 255);
      float lightness = map(analogRead(A0), 0, 1023, 0, 255);
      jsonData = "{\"Humidity\":";
      jsonData += String(humi);
      jsonData += ",\"Temperature\":";
      jsonData += String(temp);
      jsonData += ",\"Lightness\":";
      jsonData += String(lightness);
      jsonData += "}";

      //jsonData = "{\"Humidity\":\"mCookie\"}";

      int str_len = jsonData.length() + 1;
      jsonData.toCharArray(strBuf, str_len);

      // Serial.println(strBuf);

      if (mAi.mqttPublish(PUBLISHTOPIC, strBuf)) {
        Serial.println(F("mqtt publish OK!"));
      } else {
        Serial.println(F("mqtt publish failed!"));
      }
      sensorlastTime = millis();
    }
  }
  delay(100);
}
