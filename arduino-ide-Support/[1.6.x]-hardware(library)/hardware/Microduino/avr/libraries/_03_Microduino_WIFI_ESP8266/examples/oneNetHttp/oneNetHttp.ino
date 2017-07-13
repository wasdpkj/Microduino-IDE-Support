#include <ESP8266.h>
#include <aJSON.h>
#include <Wire.h>
#include <SHT2x.h>
#include <U8glib.h>



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
#define PASSWORD    "your pass"
#define HOST_NAME   "api.heclouds.com"
#define DEVICEID   "your divice id"
#define PROJECTID "your project id"
#define HOST_PORT   (80)
String apiKey="your apikey";

static const uint8_t stateLEDPin=4;
char buf[10];
int start=0;
int end=0;

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
String jsonToSend;
String postString;


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

	  if (wifi.disableMUX()) {
	      Serial.print("single ok\r\n");
	  } else {
	      Serial.print("single err\r\n");
	  }
	  Serial.print("setup end\r\n");



}

void loop(void) {

	mCottenData = wifi.getMqttJson();

	if (mCottenData != "") {
		mCottenData.trim();

		Serial.println(mCottenData);

		if (mCottenData.equals("WIFI DISCONNECT")) {
			wifi.setWiFiconnected(false);
			digitalWrite(stateLEDPin,LOW);
		} else if (mCottenData.equals("WIFI Connected")) {
			wifi.setWiFiconnected(true);
			digitalWrite(stateLEDPin,HIGH);
		}

		if (!wifi.isWiFiconnected()) {
			return;
		}

	}

	if (wifi.isWiFiconnected()) {
		if (sensorlastTime > millis())
			sensorlastTime = millis();
		if (millis() - sensorlastTime > INTERVAL_sensor) {

			tempOLED = SHT2x.GetTemperature();
			humiOLED = SHT2x.GetHumidity();
			lightnessOLED = map(analogRead(A0), 0, 1023, 0, 255);

			updateWheatherData();

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



void updateWheatherData() {

  //uint8_t buffer[1024] = {0};
  uint8_t buffer[512] = {0};
  char infoData[60]={0};

    if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
        Serial.print("create tcp ok\r\n");
    } else {
        Serial.print("create tcp err\r\n");
    }



    jsonToSend="{\"Temperature\":";
    dtostrf(tempOLED,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+=",\"Humidity\":";
    dtostrf(humiOLED,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+=",\"Light\":";
    dtostrf(lightnessOLED,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+="}";



    postString="POST /devices/";
    postString+=DEVICEID;
    postString+="/datapoints?type=3 HTTP/1.1";
    postString+="\r\n";
    postString+="api-key:";
    postString+=apiKey;
    postString+="\r\n";
    postString+="Host:api.heclouds.com\r\n";
    postString+="Connection:close\r\n";
    postString+="Content-Length:";
    postString+=jsonToSend.length();
    postString+="\r\n";
    postString+="\r\n";
    postString+=jsonToSend;
    postString+="\r\n";
    postString+="\r\n";
    postString+="\r\n";

    const char *postArray = postString.c_str();

    Serial.println(postArray);

    wifi.send((const uint8_t*)postArray, strlen(postArray));

    uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);

    if (len > 0) {
        //Serial.print("Received:[");
        for(uint32_t i = 0; i < len; i++) {
            //Serial.print((char)buffer[i]);
            if((char)buffer[i]=='{') {
              start=i;
            } if((char)buffer[i]=='}') {
              end=i;
            }
        }
        //Serial.print("]\r\n");
        int index=0;
        for(uint32_t i = start; i <= end; i++) {
            infoData[index]=(char)buffer[i];
            index++;
        }
        infoData[index]='\n';
        Serial.print("Received:[");
        Serial.println(infoData);
        Serial.print("]\r\n");

        if(strstr(infoData,"\"Status\":\"true\"")!=NULL) {
          Serial.println("ON");
        } else {
          Serial.println("OFF");
        }

    }


    if (wifi.releaseTCP()) {
        Serial.print("release tcp ok\r\n");
    } else {
        Serial.print("release tcp err\r\n");
    }

    postArray=NULL;

}

