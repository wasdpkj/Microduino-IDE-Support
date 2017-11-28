/*
 * linerCCD.cpp
 *
 *  Created on: 2017年1月12日
 *      Author: ray
 */



#include "Microduino_LinerCCD.h"

LinerCCD::LinerCCD(uint8_t pixsCommand, uint8_t lightPin) :
		_pixsCommand(pixsCommand),_lightPin(lightPin) {
	pinMode(_lightPin,INPUT);
	Wire.begin();
}

void LinerCCD::setCCDPixs(uint8_t pixsCommand) {
	_pixsCommand = pixsCommand;
	_pixsNum=pow(2, _pixsCommand+1);

	_times=128/_pixsNum+1;

	Wire.beginTransmission(_CCDAddress);
	Wire.write(_pixsCommand);
	Wire.endTransmission();
}

//void LinerCCD::setLight(uint16_t light) {
//	Wire.beginTransmission(_CCDAddress);
//	Wire.write(map(light,0,1023,7,255));
//	Wire.endTransmission();
//}

void LinerCCD::setExposed(uint8_t exposed) {
	Wire.beginTransmission(_CCDAddress);
	Wire.write(constrain(exposed, 7, 255));
	Wire.endTransmission();
}

void LinerCCD::caculateMaxMinPix() {
	for (uint8_t i = 0; i < getPixsNum(); i++) {
		if (i == 0) {
			_MaxPix = pixlArray[i];
			_MinPix = pixlArray[i];
		} else {
			if (pixlArray[i] > _MaxPix) {
				_MaxPix = pixlArray[i];
			}
			if (pixlArray[i] < _MinPix) {
				_MinPix = pixlArray[i];
			}
		}
	}
	_Min2Max = _MaxPix - _MinPix;
}

void LinerCCD::adjustContrastAlgorithm() {
	//Out = Average + (In – Average) * ( 1 + percent)   而percent即调整范围[-1,1]
	uint8_t light = getLight();
	uint8_t middleValue = (_MaxPix + _MinPix) / 2;
	for (uint8_t i = 0; i < 128; i++) {
		if ((i + 1) % getTimes() == 0) {	  //如果这个点是满足算平均数
			pixlArray[i] = middleValue
					+ (pixlArray[i] - middleValue)
							* (1 + (255 - light) / 255);
		}
	}
}

void LinerCCD::adConAlgorithm() {
  caculateMaxMinPix();
  for (uint8_t i = 0; i < getPixsNum(); i++) {
    pixlArray[i] = map(pixlArray[i], _MinPix, _MaxPix, 0, 255);
  }
}

void LinerCCD::reqestPixls() {
	switch (_pixsCommand) {
	case PIXEL128: {
		getMoreBlocks(4);
	}
		break;
	case PIXEL64: {
		getMoreBlocks(2);
	}
		break;
	case PIXEL32: {
		getOneBlockOrLess();
		//getOneBlock(0);
	}
		break;
	case PIXEL16: {
		getOneBlockOrLess();
	}
		break;
	case PIXEL8: {
		getOneBlockOrLess();
	}
		break;
	case PIXEL4: {
		getOneBlockOrLess();
	}
		break;
	case PIXEL2: {
		getOneBlockOrLess();
	}
		break;
	default:
		;
	}
}

void LinerCCD::getOneBlockOrLess() {

	Wire.requestFrom(_CCDAddress, _pixsNum);
	uint8_t index = 0;
	while (Wire.available()) {

		if (index == _pixsNum) {
			index = 0;
		}
		uint8_t c = Wire.read();
		pixlArray[index]=c;
		index++;
	}
}

void LinerCCD::getOneBlock(uint8_t curBlock) {
	Wire.requestFrom(_CCDAddress, uint8_t(32));
	uint8_t index = curBlock*32;
	while (Wire.available()) {
		uint8_t c = Wire.read();
		pixlArray[index]=c;
		index++;
	}
}

void LinerCCD::getMoreBlocks(uint8_t blockNum) {
	for (uint8_t i = 0; i < blockNum; i++) {
		getOneBlock(i);
	}
}

LinerCCD::~LinerCCD() {
}


