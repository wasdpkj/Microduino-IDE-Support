#include <ESP8266.h>

#define SSID        "yourSSID"
#define PASSWORD    "yourPass"
#define HOST_NAME   "api.heclouds.com"
#define DEVICEID   "your divice id"
#define PROJECTID "your project id"
#define HOST_PORT   (80)
String apiKey="your api key";
static const uint8_t stateLEDPin=4;
char buf[10];
int start=0;
int end=0;

String mCottenData;
String jsonToSend;
String postString;

#define INTERVAL_sensor 2000
unsigned long sensorlastTime = millis();

float tempOLED, humiOLED, lightnessOLED, gasOLED;

ESP8266 wifi(&Serial1);

void setup(void)
{
	Serial1.begin(115200);
    Serial.begin(9600);
    while(!Serial)
    Serial.print("setup begin\r\n");

    Serial.print("FW Version:");
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

void loop(void)
{

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

            tempOLED = map(analogRead(A1), 0, 1023, 0, 255);
            humiOLED = map(analogRead(A2), 0, 1023, 0, 255);
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

    // uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);

    // if (len > 0) {
    //     //Serial.print("Received:[");
    //     for(uint32_t i = 0; i < len; i++) {
    //         //Serial.print((char)buffer[i]);
    //         if((char)buffer[i]=='{') {
    //           start=i;
    //         } if((char)buffer[i]=='}') {
    //           end=i;
    //         }
    //     }
    //     //Serial.print("]\r\n");
    //     int index=0;
    //     for(uint32_t i = start; i <= end; i++) {
    //         infoData[index]=(char)buffer[i];
    //         index++;
    //     }
    //     infoData[index]='\n';
    //     Serial.print("Received:[");
    //     Serial.println(infoData);
    //     Serial.print("]\r\n");

    //     if(strstr(infoData,"\"Status\":\"true\"")!=NULL) {
    //       Serial.println("ON");
    //     } else {
    //       Serial.println("OFF");
    //     }

    // }


    if (wifi.releaseTCP()) {
        Serial.print("release tcp ok\r\n");
    } else {
        Serial.print("release tcp err\r\n");
    }

    postArray=NULL;

}
