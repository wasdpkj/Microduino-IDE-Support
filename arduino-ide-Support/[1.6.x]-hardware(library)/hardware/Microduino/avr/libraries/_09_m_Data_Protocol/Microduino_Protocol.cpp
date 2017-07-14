#include <Microduino_Protocol.h>

/****************************** Protocol ******************************/

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

void ProtocolSer::commonInit(void){
	pSwSerial = NULL;
	pHwSerial = NULL;
}

void ProtocolSer::begin(uint16_t _baud){
	baud = _baud;
	if(pSwSerial){
		pSwSerial->begin(baud);
	}else{
		pHwSerial->begin(baud);
	}
}


bool ProtocolSer::available(){
	uint8_t inChar = 0x00;
	if(pSwSerial){
		if(pSwSerial->available() > 0){
			inChar = pSwSerial->read();
			if(dataParse->parse(inChar)){
//				pSwSerial->clear();		
				pSwSerial->stopListening();
				pSwSerial->listen();
				return true;
			}
		}
	}else{
		if(pHwSerial->available() > 0){
			inChar = pHwSerial->read();
			if(dataParse->parse(inChar)){
				pHwSerial->flush();		
				return true;
			}
		}
	}	
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
	if(pSwSerial){
		pSwSerial->write(STXA);
		pSwSerial->write(STXB);
		pSwSerial->write(cmd);
		pSwSerial->write(buf, length);
		pSwSerial->write(checkSum);		
	}else{
		pHwSerial->write(STXA);
		pHwSerial->write(STXB);
		pHwSerial->write(cmd);
		pHwSerial->write(buf, length);
		pHwSerial->write(checkSum);			
	}		
}


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
