/*
*修改者:ray
*zhanglei@microduino.cc
*HTTP连接OneNet例子
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

String mCottenData;
String jsonToSend;
String postString;


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

	  if (wifi.disableMUX()) {
	      Serial.print(F("single ok\r\n"));
	  } else {
	      Serial.print(F("single err\r\n"));
	  }
	  Serial.print(F("setup end\r\n"));



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

			tempOLED = map(analogRead(A2), 0, 1023, 0, 255);
			humiOLED = map(analogRead(A1), 0, 1023, 0, 255);
			lightnessOLED = map(analogRead(A0), 0, 1023, 0, 255);

			updateWheatherData();

			sensorlastTime = millis();
		}
	}

}


void updateWheatherData() {

  //uint8_t buffer[1024] = {0};
  uint8_t buffer[512] = {0};
  char infoData[60]={0};

    if (wifi.createTCP(HOST_NAME, HOST_PORT)) {
        Serial.print(F("create tcp ok\r\n"));
    } else {
        Serial.print(F("create tcp err\r\n"));
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
        //Serial.print(F("Received:["));
        for(uint32_t i = 0; i < len; i++) {
            //Serial.print((char)buffer[i]));
            if((char)buffer[i]=='{') {
              start=i;
            } if((char)buffer[i]=='}') {
              end=i;
            }
        }
        //Serial.print(F("]\r\n"));
        int index=0;
        for(uint32_t i = start; i <= end; i++) {
            infoData[index]=(char)buffer[i];
            index++;
        }
        infoData[index]='\n';
        Serial.print(F("Received:["));
        Serial.println(infoData);
        Serial.print(F("]\r\n"));

        if(strstr(infoData,"\"Status\":\"true\"")!=NULL) {
          Serial.println(F("ON"));
        } else {
          Serial.println(F("OFF"));
        }

    }


    if (wifi.releaseTCP()) {
        Serial.print(F("release tcp ok\r\n"));
    } else {
        Serial.print(F("release tcp err\r\n"));
    }

    postArray=NULL;

}

