/*
*修改者:ray
*zhanglei@microduino.cc
*MQTT连接OneNet例子
*/


#include <ESP8266.h>
#include <aJSON.h>

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

#define SSID        "your ssid"
#define PASSWORD    "your pass"
#define MQTTSERVER   "183.230.40.39"
#define DEVICEID   "your divice id"
#define DEVICETOKEN   "your divice token"
#define PROJECTID "your project id"
//#define SUBSCRIBTOPIC   "ca/4725388"
#define PUBLISHTOPIC   "$dp"
#define MQTT_PORT   (6002)

//#define USERID   "56164"
//#define APIKEY "Gp575wKA=GFaFOBOR2UD6TJFvBs="

static const uint8_t stateLEDPin=4;

#define INTERVAL_sensor 2000
unsigned long sensorlastTime = millis();

float tempOLED, humiOLED, lightnessOLED, gasOLED;

String mCottenData;
String jsonData;

ESP8266 wifi(&EspSerial);

void setup(void) {

	Serial.begin(115200);

	pinMode(stateLEDPin,OUTPUT);
	digitalWrite(stateLEDPin,LOW);


	while (!Serial); // wait for Leonardo enumeration, others continue immediately
	Serial.print(F("setup begin\r\n"));
	delay(100);

	WifiInit(EspSerial, UARTSPEED);

	Serial.print(F("FW Microduino Version:"));
	Serial.println(wifi.getMVersion().c_str());

	if (wifi.setOprToStation()) {
		Serial.print(F("to station ok\r\n"));
	} else {
		Serial.print(F("to station err\r\n"));
	}

	if (wifi.joinAP(SSID, PASSWORD)) {
		wifi.setWiFiconnected(true);
		Serial.print(F("Join AP success\r\n"));
		Serial.print(F("IP:"));
		Serial.println(wifi.getMLocalIP().c_str());
	} else {
		wifi.setWiFiconnected(false);
		Serial.print(F("Join AP failure\r\n"));
		Serial.print(F("Make sure your SSID, PASS correctly!\r\n"));
		while (true) {
			mCottenData = wifi.getMqttJson();
			if (mCottenData != "") {
				mCottenData.trim();
				Serial.println(mCottenData);
			}
		}
	}

	if (wifi.mqttSetServer(MQTTSERVER, MQTT_PORT)) {
		Serial.print(F("mqtt set server ok\r\n"));
	} else {
		Serial.print(F("mqtt set server err\r\n"));
	}
	if (wifi.mqttConnect(DEVICEID, PROJECTID, DEVICETOKEN)) {
		wifi.setMqttConnected(true);
		digitalWrite(stateLEDPin,HIGH);
		Serial.print(F("mqtt connect onenet ok\r\n"));
	} else {
		wifi.setMqttConnected(false);
		Serial.print(F("mqtt connect onenet err\r\n"));
	}

//	if (wifi.mqttSetSubscrib(SUBSCRIBTOPIC)) {
//		Serial.print(F("mqtt set subscrib topic ok\r\n"));
//	} else {
//		Serial.print(F("mqtt set subscrib topic err\r\n"));
//	}

}

void loop(void) {

	mCottenData = wifi.getMqttJson();

	if (mCottenData != "") {
		mCottenData.trim();

		Serial.println(mCottenData);

		if (mCottenData.equals("WIFI DISCONNECT")) {
			wifi.setWiFiconnected(false);
			wifi.setMqttConnected(false);
			digitalWrite(stateLEDPin,LOW);
		} else if (mCottenData.equals("WIFI Connected")) {
			wifi.setWiFiconnected(true);
		} else if (mCottenData.equals("MQTT: Disconnected")) {
			wifi.setMqttConnected(false);
			digitalWrite(stateLEDPin,LOW);
		} else if (mCottenData.equals("MQTT: Connected")) {
			wifi.setMqttConnected(true);
			digitalWrite(stateLEDPin,HIGH);
		}

		if (!wifi.isWiFiconnected()) {
			return;
		}

//		if (wifi.isMqttConnected()) {
//			if (mCottenData.startsWith("{") && mCottenData.endsWith("}")) {
//
//				uint8_t length = mCottenData.length();
//				char buf[length + 1];
//				mCottenData.toCharArray(buf, length + 1);
//
//				aJsonObject* root = aJson.parse(buf);
//				aJsonObject* state = aJson.getObjectItem(root, "undefined"));
//
//				if (strcmp(state->valuestring, "true") == 0) {
//					Serial.println(F("ON"));
//				}
//				if (strcmp(state->valuestring, "false") == 0) {
//					Serial.println(F("OFF"));
//				}
//			}
//		}

	}

	if (wifi.isMqttConnected()) {
		if (sensorlastTime > millis())
			sensorlastTime = millis();
		if (millis() - sensorlastTime > INTERVAL_sensor) {


			float temp = map(analogRead(A2), 0, 1023, 0, 255);
			float humi = map(analogRead(A1), 0, 1023, 0, 255);
			float lightness = map(analogRead(A0), 0, 1023, 0, 255);
			tempOLED = temp;
			humiOLED = humi;
			lightnessOLED = lightness;
			jsonData = "{\"Humidity\":";
			jsonData += String(humi);
			jsonData += ",\"Temperature\":";
			jsonData += String(temp);
			jsonData += ",\"Lightness\":";
			jsonData += String(lightness);
			jsonData += "}";

//			String jsonData = "{\"temperature\":33}";
			uint16_t bodyLen = jsonData.length();

			unsigned char header[3];
			header[0] = 3;
			header[1] = (bodyLen >> 8);
			header[2] = (bodyLen & 0xFF);

			if (wifi.mqttPubOneNet(PUBLISHTOPIC, header, jsonData)) {
				Serial.print(F("mqtt publish for oneNet ok\r\n"));
			} else {
				Serial.print(F("mqtt publish for oneNet err\r\n"));
			}

			sensorlastTime = millis();

		}
	}
}
