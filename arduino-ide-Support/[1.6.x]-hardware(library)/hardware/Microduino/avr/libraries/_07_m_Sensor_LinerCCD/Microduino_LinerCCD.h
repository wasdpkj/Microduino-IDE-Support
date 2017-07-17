// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============
/*
 * linerCCD.h
 *
 *  Created on: 2017年1月12日
 *      Author: ray
 *
 *	线性CCD
 *	可设置返回的像素数128,64,32,16,8,4,2
 *	可设置曝光时间
 *	调整对比度算法
 *	计算最大,最小像素值
 */

#ifndef LINERCCD_H_
#define LINERCCD_H_

#include <Arduino.h>
#include <Wire.h>

class LinerCCD {

private:
	uint8_t _pixsCommand = PIXEL32;
	uint8_t _pixsNum=32;
	uint8_t _lightPin=A0;
	uint8_t _light=0;
	uint8_t _times=4;
	uint8_t _MaxPix=128;
	uint8_t _MinPix=128;
	uint8_t _Min2Max = 128;
	static const uint8_t _CCDAddress = 0x60;

	void getOneBlockOrLess();
	void getOneBlock(uint8_t curBlock);
	void getMoreBlocks(uint8_t blockNum);

public:
	enum {
		PIXEL2 = 0,// 2 pixel of liner CCD
		PIXEL4 = 1,// 4 pixel of liner CCD
		PIXEL8 = 2,// 8 pixel of liner CCD
		PIXEL16 = 3,// 16 pixel of liner CCD
		PIXEL32 = 4,// 32 pixel of liner CCD
		PIXEL64 = 5,// 64 pixel of liner CCD
		PIXEL128 = 6// 128 pixel of liner CCD
	};
	LinerCCD(uint8_t pixsCommand = PIXEL32, uint8_t lightPin=A0);
	void setCCDPixs(uint8_t pixsCommand);
	void setExposed(uint8_t exposed);
	void reqestPixls();
	void caculateMaxMinPix();//计算像素最大值，最小值
	void adjustContrastAlgorithm();//调整对比度算法
	void adConAlgorithm();//调整对比度算法2

	uint8_t pixlArray[128] = { 0 };

	~LinerCCD();

	uint8_t getPixsCommand() const {
		return _pixsCommand;
	}

	void setPixs_pixsCommand(uint8_t pixsCommand = 4) {
		_pixsCommand = pixsCommand;
	}

	uint8_t getLightPin() const {
		return _lightPin;
	}

	void setLightPin(uint8_t lightPin = A0) {
		_lightPin = lightPin;
	}

	uint8_t getPixsNum() const {
		return _pixsNum;
	}

	void setPixsNum(uint8_t pixsNum = 32) {
		_pixsNum = pixsNum;
	}


	uint8_t getTimes() const {
		return _times;
	}

	void setTimes(uint8_t times = 4) {
		_times = times;
	}

	uint8_t getMinPix() const {
		return _MinPix;
	}

	void setMinPix(uint8_t minPix = 128) {
		_MinPix = minPix;
	}

    uint8_t getMin2Max() const {
      return _Min2Max;
    }

	uint8_t getLight() const {
		return map(analogRead(getLightPin()), 0, 1023, 0, 255);
	}

	void setLight(uint8_t light = 0) {
		_light = light;
	}
};


#endif /* LINERCCD_H_ */
