#include "Microduino_Protocol.h"

/****************************** ProtocolSer ******************************/

#if defined (ESP32)
ProtocolSer::ProtocolSer(HardwareSerial *ser, uint8_t _len, int _rx, int _tx){
	commonInit();
	pHwSerial = ser;
	length = _len;
	pinRX = _rx;
	pinTX = _tx;
	dataParse = new DataParse(length);	
}
#elif defined (__AVR__)
ProtocolSer::ProtocolSer(HardwareSerial *ser, uint8_t _len){
	commonInit();
	pHwSerial = ser;
	length = _len;
	dataParse = new DataParse(length);	
}

ProtocolSer::ProtocolSer(SoftwareSerial *ser, uint8_t _len){
	commonInit();
	pSwSerial = ser;
	length = _len;
	dataParse = new DataParse(length);
}
#endif

void ProtocolSer::commonInit(void){
#if defined (__AVR__)
	pSwSerial = NULL;
#endif
	pHwSerial = NULL;
}

void ProtocolSer::begin(uint16_t _baud){
	baud = _baud;
#if defined (ESP32)
	if(pHwSerial){
		pHwSerial->begin(baud, SERIAL_8N1, pinRX, pinTX);
	}
#elif defined (__AVR__)
	if(pHwSerial){
		pHwSerial->begin(baud);
	}else{
		pSwSerial->begin(baud);
	}
#endif
}


bool ProtocolSer::available(){
	uint8_t inChar = 0x00;
	if(pHwSerial){
		if(pHwSerial->available() > 0){
			inChar = pHwSerial->read();
			if(dataParse->parse(inChar)){
				if(pHwSerial->available() > (SERIAL_RX_BUFFER_SIZE-length-10))
					while(pHwSerial->available() > 0)
						pHwSerial->read();
				return true;
			}
		}
	}
#if defined (__AVR__)
	else{
		if(pSwSerial->available() > 0){
			inChar = pSwSerial->read();
			if(dataParse->parse(inChar)){
//				pSwSerial->clear();		
				pSwSerial->stopListening();
				pSwSerial->listen();
				return true;
			}			
		}
	}
#endif
	return false;	
}


void ProtocolSer::readBytes(uint8_t *_cmd, uint8_t *_data, uint8_t _len){	
	dataParse->read(_cmd, _data, _len);
}

void ProtocolSer::readWords(uint8_t *_cmd, uint16_t *_data, uint8_t _len){
	readBytes(_cmd, (uint8_t *)_data, _len*2);
}

void ProtocolSer::write(uint8_t cmd, uint8_t *_data, uint8_t _len){	
	uint8_t buf[length];
	if(_len > length)
		_len = length;
	memcpy(buf, _data, _len);
	uint8_t checkSum = dataParse->getChecksum(cmd, buf, length);	
	if(pHwSerial){
		pHwSerial->write(STXA);
		pHwSerial->write(STXB);
		pHwSerial->write(cmd);
		pHwSerial->write(buf, length);
		pHwSerial->write(checkSum);			
	}
#if defined (__AVR__)
	else{
		pSwSerial->write(STXA);
		pSwSerial->write(STXB);
		pSwSerial->write(cmd);
		pSwSerial->write(buf, length);
		pSwSerial->write(checkSum);		
	}
#endif
}


/****************************** ProtocolnRF ******************************/

ProtocolnRF::ProtocolnRF(RF24 *_rf24, uint8_t _len){
	rf24 = _rf24;
	length = _len + 3;
	if((dataBuf = (uint8_t *)malloc(length))) {
		memset(dataBuf, 0, length);
	}
}


bool ProtocolnRF::begin(uint8_t _channel, const uint8_t *rxAddress, const uint8_t *txAddress){
	if(!rf24->begin())
		return false;
	rf24->setPALevel(RF24_PA_MAX);
	rf24->setChannel(_channel);
	rf24->openReadingPipe(1, rxAddress);
	rf24->openWritingPipe(txAddress);
	rf24->startListening();
	return true;
}


bool ProtocolnRF::available(){
	while (rf24->available() ) {
		memset(dataBuf, 0, length);
		rf24->read(dataBuf, length);
		if(dataBuf[0]==0xAA && dataBuf[1]==0xBB) {
			return true;
		}
	}
	return false;
}


void ProtocolnRF::readBytes(uint8_t *_cmd, uint8_t *_data, uint8_t _len){	
	*_cmd = dataBuf[2];
	if(_len > length-3){
		_len = length-3;
	}
	memcpy(_data, dataBuf+3, _len);
}

void ProtocolnRF::readWords(uint8_t *_cmd, uint16_t *_data, uint8_t _len){
	readBytes(_cmd, (uint8_t *)_data, _len*2);
}

bool ProtocolnRF::write(uint8_t cmd, uint8_t *_data, uint8_t _len){	
	memset(dataBuf, 0, length); 
	dataBuf[0] = 0xAA;
	dataBuf[1] = 0xBB;
	if(_len > length-3){
		_len = length-3;
	}
	dataBuf[2] = cmd;
	memcpy(dataBuf+3, _data, _len);
	rf24->stopListening();
	bool result = rf24->write(dataBuf, _len+3);
	rf24->startListening(); 
	return result;
}


/****************************** ProtocolZig ******************************/

#if defined (__AVR_ATmega128RFA1__)


ProtocolZig::ProtocolZig(uint8_t _len){
	length = _len;
	dataParse = new DataParse(length);	
}


void ProtocolZig::begin(uint8_t _channel) {  //param chan the channel number for the radio to use, 11 to 26
	PRO_PORT.begin(_channel);
}


bool ProtocolZig::available() {
	uint8_t inChar = 0x00;
	if (PRO_PORT.available() > 0) {
		inChar = PRO_PORT.read();
		if(dataParse->parse(inChar)){
			PRO_PORT.flush();
			return true;
		}
	}
  return false;
}


void ProtocolZig::readBytes(uint8_t *_cmd, uint8_t *_data, uint8_t _len){	
	dataParse->read(_cmd, _data, _len);
}

void ProtocolZig::readWords(uint8_t *_cmd, uint16_t *_data, uint8_t _len){
	readBytes(_cmd, (uint8_t *)_data, _len*2);
}

void ProtocolZig::write(uint8_t cmd, uint8_t *_data, uint8_t _len){	
	uint8_t buf[length];
	if(_len > length)
		_len = length;
	memcpy(buf, _data, _len);
	uint8_t checkSum = dataParse->getChecksum(cmd, buf, length);	
	PRO_PORT.beginTransmission();
	PRO_PORT.write(STXA);
	PRO_PORT.write(STXB);
	PRO_PORT.write(cmd);
	PRO_PORT.write(buf, length);
	PRO_PORT.write(checkSum);
	PRO_PORT.endTransmission();	
}

int8_t ProtocolZig::getRSSI() {
  return PRO_PORT.getLastRssi();
}

#endif
