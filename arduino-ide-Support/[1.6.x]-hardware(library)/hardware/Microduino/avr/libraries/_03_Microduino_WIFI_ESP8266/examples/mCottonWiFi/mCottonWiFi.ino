#include <ESP8266.h>
#include <aJSON.h>
#include <Wire.h>
#include <SHT2x.h>
#include <U8glib.h>

#define SSID        "your ssid"
#define PASSWORD    "your password"
#define MQTTSERVER   "mCotton.microduino.cn"
#define ID   "your divice id"
#define USER   "your divice id"
#define PASS   "your divice token"
#define SUBSCRIBTOPIC   "ca/your divice id"
#define SUBSCRIBTOPICP   "cp/your divice id"
#define PUBLISHTOPIC   "dp/your divice id"
#define MQTT_PORT   (1883)

static const uint8_t stateLEDPin=4;

#define INTERVAL_sensor 2000
unsigned long sensorlastTime = millis();

float tempOLED, humiOLED, lightnessOLED, gasOLED;

#define INTERVAL_OLED 1000
//#define setFont_S u8g.setFont(u8g_font_fixed_v0r)
unsigned long OLEDlastTime = millis();
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
//
#define setFont_L u8g.setFont(u8g_font_7x13)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_chikitar)

const unsigned char bmp_tem[] U8G_PROGMEM = { 0xE0, 0x81, 0x30, 0x83, 0x10,
		0x82, 0x10, 0x82, 0x10, 0xFA, 0x10, 0x82, 0x10, 0x82, 0x10, 0xFA, 0x10,
		0x82, 0xD0, 0x82, 0xD0, 0xFA, 0xD0, 0x82, 0xD0, 0x82, 0xD0, 0xFA, 0xD0,
		0x82, 0xD0, 0x82, 0xD0, 0xFA, 0xD0, 0x82, 0xD0, 0x82, 0xD8, 0x86, 0xC4,
		0x88, 0xF2, 0x93, 0xFB, 0xB7, 0xF9, 0xA7, 0xFD, 0xAF, 0xFD, 0xAF, 0xF9,
		0xA7, 0xFA, 0x97, 0xF2, 0x93, 0xC4, 0x88, 0x18, 0x86, 0xF0, 0x83 };

const unsigned char bmp_hum[] U8G_PROGMEM = { 0x00, 0x00, 0x01, 0x00, 0x00,
		0x01, 0x00, 0x80, 0x03, 0x08, 0x80, 0x03, 0x18, 0x80, 0x07, 0x1C, 0xC0,
		0x07, 0x3C, 0xC0, 0x07, 0x3E, 0xE0, 0x0F, 0x3E, 0xE0, 0x0F, 0x7A, 0xF0,
		0x1F, 0x7B, 0xF8, 0x1F, 0x72, 0xF8, 0x1F, 0x3E, 0xF8, 0x3F, 0x1C, 0xFC,
		0x3F, 0x00, 0xFC, 0x7F, 0x00, 0xFE, 0x7F, 0x00, 0xFE, 0x7F, 0x00, 0xFE,
		0x7F, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xF3,
		0xFF, 0x00, 0xF2, 0x7F, 0x00, 0xE6, 0x7F, 0x00, 0xC6, 0x7F, 0x00, 0x0E,
		0x3F, 0x00, 0x3C, 0x1E, 0x00, 0xF8, 0x1F, 0x00, 0xE0, 0x07, 0x00, 0x80,
		0x01 };

const unsigned char led_on[] U8G_PROGMEM = { 0x00, 0x0E, 0x00, 0x08, 0x00, 0x02,
		0x18, 0x1F, 0x03, 0x90, 0x3F, 0x01, 0xC0, 0x60, 0x00, 0x60, 0xC0, 0x00,
		0x30, 0x80, 0x01, 0x37, 0x80, 0x1D, 0x30, 0x80, 0x01, 0x30, 0x80, 0x01,
		0x30, 0x91, 0x01, 0x37, 0x8E, 0x1D, 0x30, 0x80, 0x01, 0x30, 0x80, 0x01,
		0x30, 0x80, 0x01, 0x36, 0x80, 0x0D, 0xF3, 0xFF, 0x19, 0xE0, 0xFF, 0x00,
		0x80, 0x31, 0x00, 0x80, 0x31, 0x00, 0x80, 0x31, 0x00, 0x80, 0x31, 0x00,
		0x80, 0x31, 0x00, 0x80, 0x31, 0x00, 0x80, 0x31, 0x00, 0x80, 0x01, 0x00,
		0x80, 0x01, 0x00, 0x80, 0x01, 0x00, 0x80, 0x01, 0x00, 0x80, 0x01, 0x00,
		0x80, 0x01, 0x00, 0x00, 0x00, 0x00, };

const unsigned char led_off[] U8G_PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x1F, 0x00, 0x80, 0x3F, 0x00, 0xC0, 0x60, 0x00, 0x60, 0xC0,
		0x00, 0x30, 0x80, 0x01, 0x30, 0x80, 0x01, 0x30, 0x80, 0x01, 0x30, 0x80,
		0x01, 0x30, 0x91, 0x01, 0x30, 0x8E, 0x01, 0x30, 0x80, 0x01, 0x30, 0x80,
		0x01, 0x30, 0x80, 0x01, 0x30, 0x80, 0x01, 0xF0, 0xFF, 0x01, 0xE0, 0xFF,
		0x00, 0x80, 0x31, 0x00, 0x80, 0x31, 0x00, 0x80, 0x31, 0x00, 0x80, 0x31,
		0x00, 0x80, 0x31, 0x00, 0x80, 0x31, 0x00, 0x80, 0x31, 0x00, 0x80, 0x01,
		0x00, 0x80, 0x01, 0x00, 0x80, 0x01, 0x00, 0x80, 0x01, 0x00, 0x80, 0x01,
		0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, };


String mCottenData;
String jsonData;

ESP8266 wifi(&Serial1);

void setup(void) {
	Serial1.begin(115200);
	Serial.begin(115200);

	pinMode(stateLEDPin,OUTPUT);
	digitalWrite(stateLEDPin,LOW);

	Serial.print("FW Microduino Version:");
	Serial.println(wifi.getMVersion().c_str());


//	if (wifi.setUart(57600)) {
//		Serial.print("set uart ok\r\n");
//	} else {
//		Serial.print("set uart err\r\n");
//	}

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

				uint8_t length = mCottenData.length();
				char buf[length + 1];
				mCottenData.toCharArray(buf, length + 1);

				aJsonObject* root = aJson.parse(buf);
				aJsonObject* state = aJson.getObjectItem(root, "undefined");

				if (strcmp(state->valuestring, "true") == 0) {
					Serial.println("ON");
				}
				if (strcmp(state->valuestring, "false") == 0) {
					Serial.println("OFF");
				}
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


			if (wifi.mqttPublishM(jsonData)) {
				Serial.print("mqtt publishM ok\r\n");
			} else {
				Serial.print("mqtt publishM err\r\n");
			}

			sensorlastTime = millis();

		}
	}

	oledShow(tempOLED, humiOLED, lightnessOLED, gasOLED);

}



void oledShow(float temp, float humi, float lightness, float gas) {

	if (OLEDlastTime > millis())
		OLEDlastTime = millis();
	if (millis() - OLEDlastTime > INTERVAL_OLED) {

		u8g.firstPage();
		do {
			u8g.setDefaultForegroundColor();

			u8g.drawXBMP(4, 1, 15, 32, bmp_tem);
			u8g.drawXBMP(70, 2, 24, 30, bmp_hum);

			setFont_M;                             //��������Ϊ��
			u8g.setPrintPos(20, 16);         //�������ֿ�ʼ����
			u8g.print("`C ");
			setFont_L;                             //��������Ϊ��
			u8g.setPrintPos(20, 32);         //�������ֿ�ʼ����
			u8g.print(temp, 1);             //�¶�

			setFont_M;                             //��������Ϊ��
			u8g.setPrintPos(100, 16);         //�������ֿ�ʼ����
			u8g.print("%");
			setFont_L;                             //��������Ϊ��
			u8g.setPrintPos(100, 32);         //�������ֿ�ʼ����
			u8g.print(humi, 0);             //ʪ��

			setFont_L;                       //��������
			u8g.setPrintPos(4, 49);          //�������ֿ�ʼ����
			u8g.print(lightness, 0);       //����ǿ��
			setFont_M;                       //��������
			u8g.print(" Lux");

			// setFont_L;                       //��������
			// u8g.setPrintPos(80, 49);          //�������ֿ�ʼ����
			// u8g.print(gas, 0);       //����ǿ��
			// setFont_M;                       //��������
			// u8g.print(" ppm");

		} while (u8g.nextPage());

		OLEDlastTime = millis();
	}

}

