#include <ESP8266.h>
#include <Wire.h>
#include <SHT2x.h>
#include <SoftwareSerial.h>


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
#define UARTSPEED  19200
#endif


#define SSID        "your ssid"
#define PASSWORD    "your password"
#define MQTTSERVER   "mCotton.microduino.cn"
#define ID   "your divece id"
#define USER   "your divece id"
#define PASS   "your divece token"
#define SUBSCRIBTOPIC   "ca/your divece id"
#define SUBSCRIBTOPICP   "cp/your divece id"
#define PUBLISHTOPIC   "dp/your divece id"
#define MQTT_PORT   (1883)

static const uint8_t stateLEDPin=4;

#define INTERVAL_sensor 2000
unsigned long sensorlastTime = millis();

String mCottenData;
String jsonData;

ESP8266 wifi(&EspSerial);

void setup(void) {

	Serial.begin(115200);

	pinMode(stateLEDPin,OUTPUT);
	digitalWrite(stateLEDPin,LOW);

	while (!Serial); // wait for Leonardo enumeration, others continue immediately
  	Serial.print("setup begin\r\n");
  	delay(100);

	WifiInit(EspSerial, UARTSPEED);

	Serial.print("FW Microduino Version:");
	Serial.println(wifi.getMVersion().c_str());


	if (wifi.setOprToStation()) {
		Serial.print("to station ok\r\n");
	} else {
		Serial.print("to station err\r\n");
	}

//	wifi.setAutoConnect(false);
//	wifi.restart();

	if (wifi.joinAP(SSID, PASSWORD)) {
		wifi.setWiFiconnected(true);
		Serial.print("Join AP success\r\n");
		Serial.print("IP:");
		Serial.println(wifi.getMLocalIP().c_str());
	} else {
		wifi.setWiFiconnected(false);
		Serial.print("Join AP failure\r\n");
		Serial.print("Make sure your SSID, PASS correctly!\r\n");
		while (true) {
			mCottenData = wifi.getMqttJson();
			if (mCottenData != "") {
				mCottenData.trim();
				Serial.println(mCottenData);
			}
		}
	}

	if (wifi.mqttSetServer(MQTTSERVER, MQTT_PORT)) {
		Serial.print("mqtt set server ok\r\n");
	} else {
		Serial.print("mqtt set server err\r\n");
	}
	if (wifi.mqttConnect(ID, USER, PASS)) {
		wifi.setMqttConnected(true);
		digitalWrite(stateLEDPin,HIGH);
		Serial.print("mqtt connect ok\r\n");
	} else {
		wifi.setMqttConnected(false);
		Serial.print("mqtt connect err\r\n");
		while (true) {
			mCottenData = wifi.getMqttJson();
			if (mCottenData != "") {
				mCottenData.trim();
				Serial.println(mCottenData);
			}
		}
	}
	if (wifi.mqttSetDiveceIDToken(ID, PASS)) {
		Serial.print("mqtt set device ID Token ok\r\n");
	} else {
		Serial.print("mqtt set device ID Token err\r\n");
	}
	if (wifi.mqttSetSubscrib(SUBSCRIBTOPIC)) {
		Serial.print("mqtt set subscrib ca topic ok\r\n");
	} else {
		Serial.print("mqtt set subscrib ca topic err\r\n");
	}
	if (wifi.mqttSetSubscrib(SUBSCRIBTOPICP)) {
		Serial.print("mqtt set subscrib cp topic ok\r\n");
	} else {
		Serial.print("mqtt set subscrib cp topic err\r\n");
	}


	jsonData = "{\"Humidity\":66.66}";
	if (wifi.mqttPublish(PUBLISHTOPIC, jsonData)) {
		Serial.print("mqtt publish ok\r\n");
	} else {
		Serial.print("mqtt publish err\r\n");
	}

	Wire.begin();

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

		if (wifi.isMqttConnected()) {
			if (mCottenData.startsWith("{") && mCottenData.endsWith("}")) {
				Serial.println(mCottenData);
			}
		}
	}

	if (wifi.isMqttConnected()) {
		if (sensorlastTime > millis())
			sensorlastTime = millis();
		if (millis() - sensorlastTime > INTERVAL_sensor) {

			float temp = SHT2x.GetTemperature();
			float humi = SHT2x.GetHumidity();
			float lightness = map(analogRead(A0), 0, 1023, 0, 255);
			jsonData = "{\"Humidity\":";
			jsonData += String(humi);
			jsonData += ",\"Temperature\":";
			jsonData += String(temp);
			jsonData += ",\"Lightness\":";
			jsonData += String(lightness);
			jsonData += "}";


			if (wifi.mqttPublishM(jsonData)) {
				Serial.print("mqtt publishM ok\r\n");
			} else {
				Serial.print("mqtt publishM err\r\n");
			}

			sensorlastTime = millis();

		}
	}

}


