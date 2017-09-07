/*
*修改者:ray
*zhanglei@microduino.cc
*SmartConfiger例子
*/


#include <EEPROM.h>
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

#define MQTTSERVER   "mCotton.microduino.cn"
#define PROJECTID   "your project id"
#define PROJECTTOKEN "your project token"
#define deviceName "your device name"

#define MQTT_PORT   (1883)
#define REG_PORT   (1881)
#define DeviceID_Token_Range 200
#define EEPROMLengh 1024
#define smartConfigerButtonPin 5

bool isSmartConfiger = false;
bool getDevID_TokenEnable = false;


#define INTERVAL_sensor 2000
unsigned long sensorlastTime = millis();

String mCottenData;
String jsonData;
String subscribTopic;
String publishTopic;
String MACAddr;
String deviceID, secureToken;
const uint8_t lenNum = 5;

ESP8266 wifi(&EspSerial);


//写EEPROM
void writeToEEPROM(uint16_t start, uint16_t end) {
	//clear EEPROM
	for (uint16_t i = start; i < end; i++) {
		EEPROM.write(i, 0);
	}
//	EEPROM.commit();
//		EEPROM.end();
	String storInfo;

	storInfo = deviceID + "," + secureToken + ",";

	//write
	uint16_t storInfoLen = storInfo.length();
	storInfo = String(storInfoLen) + "," + storInfo;
	for (uint16_t i = start; i < (start + storInfo.length()); i++) {
		EEPROM.write(i, storInfo[i]);
	}
	Serial.print(F("---write from "));
	Serial.print(start);
	Serial.println(F(" once---"));
//	EEPROM.commit();
//		EEPROM.end();
}

//显示EEPROM
void showEEPROM() {
	Serial.println();
	Serial.print(F("Total memory is:"));
	Serial.println(EEPROMLengh);
	Serial.println(F("-----EEPROM image here:-----"));
	for (uint16_t i = 0; i < EEPROMLengh; i++) {
		char ascii = EEPROM.read(i);
		Serial.print(ascii);
	}
	Serial.println();
//		EEPROM.commit();
//		EEPROM.end();
}

//读EEPROM
void readEEPROM(uint8_t start, uint8_t end) {
	//get lenNum
	String lenNumStr;
	for (uint8_t i = start; i < (start + lenNum); i++) {
		char ascii = EEPROM.read(i);
		if (i == 0) {
			lenNumStr = ascii;
		} else {
			if (EEPROM.read(i) == ',') {
				break;
			} else {
				lenNumStr += ascii;
			}
		}
	}

	uint8_t startIndex = start + lenNumStr.length() + 1;
	uint16_t lenNumber = lenNumStr.toInt();

	String praseStr;
	for (uint16_t i = startIndex; i < lenNumber + startIndex; i++) {
		char printChar = EEPROM.read(i);
		if (praseStr.length() > 0) {
			praseStr += printChar;
		} else {
			praseStr = printChar;
		}
	}
//		EEPROM.commit();
//		EEPROM.end();
	String printStr;
	int strLength = praseStr.length();
	int startSplit = 0;
	uint8_t num = 0;
	for (uint16_t i = 0; i < strLength; i++) {

		Serial.println(i);
		if (praseStr.substring(i, i + 1) == ",") {
			printStr = praseStr.substring(startSplit, i);
			startSplit = i + 1;
			switch (num) {
			case 0:
				Serial.print(F("diviceID:"));
				deviceID = printStr;
				Serial.println(printStr);
				break;
			case 1:
				Serial.print(F("secureToken:"));
				secureToken = printStr;
				Serial.println(printStr);
				break;
			}
			num++;
		}
	}
//		EEPROM.commit();
//		EEPROM.end();
}

void doRegMqtt() {

//	Serial.println(F("station MAC addrress:"));
//	MACAddr=wifi.getStationMac();
//	MACAddr.replace(":", "");
//	MACAddr.toLowerCase();
//	Serial.println(MACAddr);

	subscribTopic = "s\/";
	subscribTopic += PROJECTID;
	subscribTopic += "\/";
	subscribTopic += MACAddr;

	publishTopic = "q\/";
	publishTopic += PROJECTID;
	publishTopic += "\/";
	publishTopic += MACAddr;

	//setRegister
	if (wifi.mqttSetServer(MQTTSERVER, REG_PORT)) {
		Serial.print(F("mqtt set reg server ok\r\n"));
	} else {
		Serial.print(F("mqtt set reg server err\r\n"));
	}

	if (wifi.mqttConnect(MACAddr, PROJECTID, PROJECTTOKEN)) {
		wifi.setMqttConnected(true);
		Serial.print(F("mqtt reg connect ok\r\n"));
	} else {
		wifi.setMqttConnected(false);
		Serial.print(F("mqtt reg connect err\r\n"));
	}

	if (wifi.mqttSetSubscrib(subscribTopic)) {
		Serial.print(F("reg set subscrib topic ok\r\n"));
	} else {
		Serial.print(F("reg set subscrib topic err\r\n"));
	}

	jsonData = "{\"";
	jsonData += "dM\":\"";
	jsonData += MACAddr;
	jsonData += "\",\"pId\":\"";
	jsonData += PROJECTID;
	jsonData += "\",\"dN\":\"";
	jsonData += deviceName;
	jsonData += "\"}";

	if (wifi.mqttSetTopic(publishTopic)) {
		Serial.print(F("mqtt reg set publish topic ok\r\n"));
	} else {
		Serial.print(F("mqtt reg set publish topic err\r\n"));
	}
	if (wifi.mqttSetMessage(jsonData)) {
		Serial.print(F("mqtt reg set message ok\r\n"));
	} else {
		Serial.print(F("mqtt reg set message err\r\n"));
	}

	if (wifi.mqttPub()) {
		Serial.print(F("mqtt reg pub ok\r\n"));
	} else {
		Serial.print(F("mqtt reg pub err\r\n"));
	}


	Serial.println(F("Waitting for DeviceID, DeviceToken"));



	while (true) {
		if (sensorlastTime > millis())
			sensorlastTime = millis();
		if (millis() - sensorlastTime > INTERVAL_sensor) {
			Serial.println(F("..."));
			sensorlastTime = millis();
		}
		mCottenData = wifi.getMqttJson();
		if (mCottenData != "") {
			mCottenData.trim();
			if (mCottenData.startsWith("{") && mCottenData.endsWith("}")) {
				uint8_t length = mCottenData.length();
				char buf[length + 1];
				mCottenData.toCharArray(buf, length + 1);
				aJsonObject* root = aJson.parse(buf);
				aJsonObject* DID = aJson.getObjectItem(root, "deviceId");
				aJsonObject* DToken = aJson.getObjectItem(root, "secureToken");
				deviceID = DID->valuestring;
				secureToken = DToken->valuestring;
				break;
			}
		}
	}


}


//设置传输数据Mqtt
void setTransportMqtt() {
	readEEPROM(0, DeviceID_Token_Range);

	if (wifi.mqttSetServer(MQTTSERVER, MQTT_PORT)) {
		Serial.print(F("mqtt set server ok\r\n"));
	} else {
		Serial.print(F("mqtt set server err\r\n"));
	}

	//subscribe:ca

	//publish:dp->数据
	//da->控制
	//		:de->事件

	subscribTopic = "ca\/";
	subscribTopic += deviceID;

	publishTopic = "dp\/";
	publishTopic += deviceID;


	if (wifi.mqttConnect(MACAddr, deviceID, secureToken)) {
		wifi.setMqttConnected(true);
		Serial.print(F("mqtt connect ok\r\n"));
	} else {
		wifi.setMqttConnected(false);
		Serial.print(F("mqtt connect err\r\n"));
	}
	if (wifi.mqttSetDiveceIDToken(MACAddr, secureToken)) {
		Serial.print(F("mqtt set device ID Token ok\r\n"));
	} else {
		Serial.print(F("mqtt set device ID Token err\r\n"));
	}
	if (wifi.mqttSetSubscrib(subscribTopic)) {
		Serial.print(F("mqtt set subscrib ca topic ok\r\n"));
	} else {
		Serial.print(F("mqtt set subscrib ca topic err\r\n"));
	}

	subscribTopic[1]='p';
	if (wifi.mqttSetSubscrib(subscribTopic)) {
		Serial.print(F("mqtt set subscrib cp topic ok\r\n"));
	} else {
		Serial.print(F("mqtt set subscrib cp topic err\r\n"));
	}
	subscribTopic[1]='a';

}

void setup(void) {
	Serial.begin(115200);


	pinMode(smartConfigerButtonPin, INPUT_PULLUP);
	bool smartState = digitalRead(smartConfigerButtonPin);
	isSmartConfiger = getDevID_TokenEnable = !smartState;


	while (!Serial); // wait for Leonardo enumeration, others continue immediately
	Serial.print(F("setup begin\r\n"));
	delay(100);

	WifiInit(EspSerial, UARTSPEED);

	EEPROM.begin();
	Serial.print(F("FW Microduino Version:"));
	Serial.println(wifi.getMVersion());

	if (wifi.setOprToStation()) {
		Serial.print(F("to station ok\r\n"));
	} else {
		Serial.print(F("to station err\r\n"));
	}

	Serial.println(F("station MAC addrress:"));
	MACAddr=wifi.getSimpleMac();
	MACAddr.replace(":", "");
	MACAddr.toLowerCase();
	Serial.println(MACAddr);

	if (isSmartConfiger) {
		wifi.smartConfiger(true);
		Serial.println(F("Smart Configer ..."));
		while (true) {

			if (sensorlastTime > millis())
				sensorlastTime = millis();
			if (millis() - sensorlastTime > INTERVAL_sensor) {

				Serial.println(F("..."));
				sensorlastTime = millis();
			}

			mCottenData = wifi.getMqttJson();
			if (mCottenData != "") {
				mCottenData.trim();
				Serial.println(mCottenData);
				if (mCottenData == "smartconfig connected wifi") {
					break;
				}
			}
		}
	} else {
		wifi.setAutoConnect(true);
		Serial.println(F("restart"));
		wifi.restart();
	}

	Serial.println(F("wifi info"));
	Serial.println(wifi.queryWiFiInfo());

	if (getDevID_TokenEnable) {
		//执行注册
		doRegMqtt();

		writeToEEPROM(0, DeviceID_Token_Range);
		if(wifi.mqttDisconnect()) {//断开连接
			Serial.print(F("disconnect ok\r\n"));
		}else {
			Serial.print(F("disconnect err\r\n"));
		}

	} else {//否则，直接设置传输数据Mqtt
		showEEPROM();
		delay(1000);//??????
	}
	//设置传输数据Mqtt
	setTransportMqtt();
}

void loop(void) {

	mCottenData = wifi.getMqttJson();

	if (mCottenData != "") {
		mCottenData.trim();
		Serial.println(mCottenData);
		if (mCottenData.equals("WIFI DISCONNECT")) {
			wifi.setWiFiconnected(false);
			wifi.setMqttConnected(false);
		} else if (mCottenData.equals("WIFI Connected")) {
			wifi.setWiFiconnected(true);
		} else if (mCottenData.equals("MQTT: Disconnected")) {
			wifi.setMqttConnected(false);
		} else if (mCottenData.equals("MQTT: Connected")) {
			wifi.setMqttConnected(true);
		}
		if (!wifi.isWiFiconnected()) {
			return;
		}
		if (wifi.isMqttConnected()) {
			if (mCottenData.startsWith("{") && mCottenData.endsWith("}")) {

				uint8_t length = mCottenData.length();
				char buf[length + 1];
				mCottenData.toCharArray(buf, length + 1);

				aJsonObject* root = aJson.parse(buf);
				aJsonObject* state = aJson.getObjectItem(root, "undefined");

				if (strcmp(state->valuestring, "true") == 0) {
					Serial.println(F("ON"));
				}
				if (strcmp(state->valuestring, "false") == 0) {
					Serial.println(F("OFF"));
				}
			}
		}
	}

	if (wifi.isMqttConnected()) {
		if (sensorlastTime > millis())
			sensorlastTime = millis();
		if (millis() - sensorlastTime > INTERVAL_sensor) {

			uint16_t value = analogRead(A0);

			jsonData = "{\"Humidity\":";
			jsonData += value / 2;
			jsonData += ",\"Temperature\":";
			jsonData += value / 4;
			jsonData += ",\"Lightness\":";
			jsonData += value;
			jsonData += "}";

			if (wifi.mqttPublishM(jsonData)) {
				Serial.print(F("mqtt publishM ok\r\n"));
			} else {
				Serial.print(F("mqtt publishM err\r\n"));
			}

			sensorlastTime = millis();

		}
	}

}

