#include "Microduino_Ai.h"

// 参数说明:  
// in， 源字符串  
// src，要替换的字符串  
// dst，替换成什么字符串  
static void strrpl(char* pDstOut, char* pSrcIn, const char* pSrcRpl, const char* pDstRpl)
{ 
	char* pi = pSrcIn; 
	char* po = pDstOut; 

	int nSrcRplLen = strlen(pSrcRpl); 
	int nDstRplLen = strlen(pDstRpl); 

	char *p = NULL; 
	int nLen = 0; 

	while(1)
	{
		p = strstr(pi, pSrcRpl); 
		if(p == NULL) 
		{ 
		strcpy(po, pi); 
		break;
		} 
		nLen = p - pi; 
		memcpy(po, pi, nLen);
		memcpy(po + nLen, pDstRpl, nDstRplLen); 

		pi = p + nSrcRplLen; 
		po = po + nLen + nDstRplLen; 
	}
}

#if defined (ESP32)
MicroAi::MicroAi(HardwareSerial *ser, int _rx, int _tx) {
	common_init();
	aiHwSerial = ser;
	pinRX = _rx;
	pinTX = _tx;
}
#elif defined (__AVR__)
MicroAi::MicroAi(HardwareSerial *ser) {
	common_init();
	aiHwSerial = ser;
}

MicroAi::MicroAi(SoftwareSerial *ser) {
	common_init();
	aiSwSerial = ser;
}
#endif

void MicroAi::common_init(void) {
	aiHwSerial = NULL;
#if defined (__AVR__)
	aiSwSerial = NULL;
#endif
}

int MicroAi::available() {
	if (aiHwSerial) {
		return aiHwSerial->available();
	}
#if defined (__AVR__)
	else {
		return aiSwSerial->available();
	}
#endif
	return 0;
}

int MicroAi::read() {
	if (aiHwSerial) {
		return (char)(aiHwSerial->read());
	}
#if defined (__AVR__)
	else {
		return (char)(aiSwSerial->read());
	}
#endif
	return -1;
}

void MicroAi::write(char* str) {
//  Serial.println(str);
	if (aiHwSerial)
		aiHwSerial->print(str);
#if defined (__AVR__)
	else
		aiSwSerial->print(str);
#endif
}

void MicroAi::write(uint8_t c) {
  if (aiHwSerial)
    aiHwSerial->print(c);
#if defined (__AVR__)
  else
    aiSwSerial->print(c);
#endif
}

void MicroAi::clear() {
  if (aiHwSerial) {
    while(aiHwSerial->available())
      aiHwSerial->read();
  }
#if defined (__AVR__)
  else {
    aiSwSerial->stopListening();
    aiSwSerial->listen();
  }
#endif
}

char* MicroAi::recvWait(uint32_t timeout, char* str){
	static char buf_chk[128] = "";
	char* str_chk = buf_chk;
	uint8_t str_index = 0;
	char buffer;
	uint32_t endTime = millis() + timeout;
	while (millis() < endTime) {
		buffer = read();
		if (buffer == -1){
			continue;
		}else if (buffer != '\n') {
			str_chk[str_index++] = buffer;
			str_chk[str_index] = '\0';
			if (str_index >= 126)
				break;
		} else {
#ifdef DEBUG
      Serial.println(str_chk);
#endif
			str_chk[str_index - 1] = ' ';
			if (strstr(str_chk, str))
				return str_chk;
			else if (strstr(str_chk, "ERROR"))
				break;
		}
	}
	return NULL;
}

bool MicroAi::begin(uint8_t mode) {
#if defined (ESP32)
	aiHwSerial->begin(9600, SERIAL_8N1, pinRX, pinTX);
#elif defined (__AVR__)
	if (aiSwSerial)
		aiSwSerial->begin(9600);
	else
		aiHwSerial->begin(9600);
#endif
	bool flag = false;
	for(uint8_t i=0; i<10; i++){
		if(isOn()){
			flag = true;
			break;
		}
		delay(500);
	}
	if(!flag)
		return false;

  reset();
	delay(2000);
  clear();
	if(!setMode(mode))
    return false;

  if(mode = MODE_WIFI)
    delay(3000);
  return true;
}

bool MicroAi::isOn() {
	write("AT\r\n");
	return (recvWait(1000, "OK") != NULL);
}

char* MicroAi::getVersion() {
	write("AT+VERS?\r\n");
	char* str_out = recvWait(1000, "+VERS");
	if (str_out == NULL) {
		return NULL;
	}
	char *p = strdup(strchr(str_out, ':') + 1);
  recvWait(1000, "OK");
	return p;  
}

int32_t MicroAi::getFreeRam() {
  write("AT+FRAM?\r\n");
  char* str_out = recvWait(1000, "+FRAM");
  if (str_out == NULL) {
    return 0;
  }
  int32_t ram = atoi(strchr(str_out, ':') + 1);
  recvWait(1000, "OK");
  return ram;  
}

bool MicroAi::reset() {
	write("AT+RST\r\n");
	return (recvWait(1000, "OK") != NULL);
}

bool MicroAi::setMode(uint8_t mode) {
	write("AT+MODE=");
	write(mode);
	write("\r\n");
	return (recvWait(1000, "OK") != NULL);
}

int8_t MicroAi::getMode() {
	write("AT+MODE?\r\n");
  char* str_out = recvWait(1000, "+MODE");
	if (str_out == NULL) {
		return -1;
	}
	int8_t mode = atoi(strchr(str_out, ':') + 1);
  recvWait(1000, "OK");
	return mode;
}

bool MicroAi::setWifi(char* ssid, char* pwd) {
	write("AT+CWJAP=\"");
	write(ssid);
	write("\",\"");
	write(pwd);
	write("\"\r\n");
	return (recvWait(1000, "OK") != NULL);
}

int8_t MicroAi::getRssi() {
	write("AT+CWJAP?\r\n");
  char* str_out = recvWait(1000, "+CWJAP");
	if (str_out == NULL) {
		return 0;
	}
  int8_t rssi = 0;
	char *p1 = strchr(str_out, ',');
  if (p1){
    char *p2 = strchr(p1+1, ',');
    if(p2){
      rssi = atoi(++p2);  
    }
  }
  recvWait(1000, "OK"); 
	return rssi;
}

bool MicroAi::nameBT(char* name) {
	write("AT+BTA=0,\"");
	write(name);
	write("\"\r\n");
	return (recvWait(1000, "OK") != NULL);
}

bool MicroAi::cmdBT(uint8_t cmd) {
	if(cmd < 1 || cmd > 4)
		return false;
	
	write("AT+BTA=");
	write(cmd);
	write("\r\n");
	return (recvWait(1000, "OK") != NULL);
}

int8_t MicroAi::getBTstatus() {
	write("AT+BTA?\r\n");
  char* str_out = recvWait(1000, "+BTA"); 
	if (str_out == NULL) {
		return -1;
	}
  int8_t state = atoi(strchr(str_out, ':') + 1);
  recvWait(1000, "OK");
	return state;
}

bool MicroAi::playFile(char* file) {
	write("AT+PLAY=0,\"");
	write(file);
	write("\"\r\n");
	return (recvWait(10000, "OK") != NULL);
}

bool MicroAi::playCmd(uint8_t cmd) {
	if(cmd < 1 || cmd > 2)
		return false;
	
	write("AT+PLAY=");
	write(cmd);
	write("\r\n");
	return (recvWait(10000, "OK") != NULL);
}

char* MicroAi::getPlayFile() {
	write("AT+PLAY?\r\n");
  char* str_out = recvWait(1000, "+PLAY");
	if (str_out == NULL) {
		return NULL;
	}
  char *p = strchr(str_out, ',');
  char *out = NULL;
  if(p){
    out = strdup(p + 1);  
  }
  recvWait(1000, "OK");
	return out;  
}

bool MicroAi::startRecord(char* file) {
	write("AT+REC=0,\"");
	write(file);
	write("\"\r\n");
	return (recvWait(3000, "OK") != NULL);
}

bool MicroAi::stopRecord() {
	write("AT+REC=2\r\n");
	return (recvWait(3000, "OK") != NULL);
}

int32_t MicroAi::getRecordTime() {
	write("AT+REC?\r\n");
  char* str_out = recvWait(1000, "+REC");
	if (str_out == NULL) {
		return 0;
	}
  int32_t rec_time = atoi(strchr(str_out, ':') + 1);
  recvWait(1000, "OK");
	return rec_time;  
}

bool MicroAi::mqttSetServer(char* server) {
  write("AT+MQSER=\"");
  write(server);
  write("\"\r\n");
  return (recvWait(1000, "OK") != NULL);
}

bool MicroAi::mqttConnect(char* id, char* usr, char* pwd) {
  write("AT+MQCON=\"");
  write(id);
  write("\",\"");
  write(usr); 
  write("\",\"");
  write(pwd);    
  write("\"\r\n");
  return (recvWait(1000, "OK") != NULL);
}

int8_t MicroAi::mqttGetStatus() {
  write("AT+MQCON?\r\n");
  char* str_out = recvWait(1000, "+MQCON"); 
  if (str_out == NULL) {
    return -1;
  }
  int8_t state = atoi(strchr(str_out, ':') + 1);
  recvWait(1000, "OK");
  return state;
}

bool MicroAi::mqttSetSubscrib(char* sub) {
  write("AT+MQSUB=1,\"");
  write(sub);
  write("\"\r\n");
  return (recvWait(1000, "OK") != NULL);
}

bool MicroAi::mqttPublish(char* pub, char* msg) {
  write("AT+MQPUB=\"");
  write(pub);
  //write("\",\"");
  write("\",");
  write(msg);
  //write("\"\r\n");
  write("\r\n");
  return (recvWait(1000, "OK") != NULL);
}

int32_t MicroAi::mqttQuery(char* res) {
  write("AT+MQQER?\r\n");
  char* str_out = recvWait(2000, "+MQQER"); 
  if (str_out == NULL) {
    return 0;
  }  
  int32_t num = 0;
  char *p1 = strchr(str_out, ':');
  if (p1){
    num = atoi(p1+1);
    char *p2 = strchr(p1, ',');
    if(p2){
      strrpl(res, p2+1, "\"", ""); 
    }
  }
  recvWait(1000, "OK");
  return num;
}

bool MicroAi::volumeUp() {
	write("AT+VOL=+\r\n");
	return (recvWait(1000, "OK") != NULL);
}

bool MicroAi::volumeDown() {
	write("AT+VOL=-\r\n");
	return (recvWait(1000, "OK") != NULL);
}

bool MicroAi::setVolume(uint8_t vol) {
	if(vol > 63)
		vol = 63;
	
	write("AT+VOL=");
	write(vol);
	write("\r\n");
	return (recvWait(1000, "OK") != NULL);
}

int8_t MicroAi::getVolume() {
	write("AT+VOL?\r\n");
  char* str_out = recvWait(1000, "+VOL"); 
	if (str_out == NULL) {
		return -1;
	}
	int8_t vol = atoi(strchr(str_out, ':') + 1);
  recvWait(1000, "OK");
	return vol;  
}

bool MicroAi::startRest() {
	write("AT+REST\r\n");
	return (recvWait(1000, "OK") != NULL); 
}

int32_t MicroAi::waitResult(char* res) {
  char* str_out = recvWait(20000, "+RELT"); 
  if (str_out == NULL) {
    return 0;
  }  
  int32_t code = 0;
  char *p1 = strchr(str_out, ':');
  if (p1){
    code = atoi(p1+1);
    char *p2 = strchr(p1, ',');
    if(p2){
      strrpl(res, p2+1, "\"", ""); 
    }
  }
  return code;
}

uint32_t MicroAi::testQuery() {
  write("AT+TEST?\r\n");
  char* str_out = recvWait(1000, "+TEST"); 
  if (str_out == NULL) {
    return -1;
  }
  uint32_t state = atoi(strchr(str_out, ':') + 1);
  recvWait(1000, "OK");
  return state;
}

//
// END OF FILE
//
