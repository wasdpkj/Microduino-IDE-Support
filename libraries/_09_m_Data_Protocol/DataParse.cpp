#include "DataParse.h"
#include <Arduino.h>

DataParse::DataParse(uint8_t _length){
	length = _length;
	step = STEP_WAIT_AA;

	if((data = (uint8_t *)malloc(length))) {
		memset(data, 0, length);
	}
}


uint8_t DataParse::getChecksum(uint8_t _cmd, uint8_t *_data, uint8_t _len) {
  //三个参数分别为：指令代码 ， 数据长度 ， 实际数据数组
  uint8_t checksum = 0;
  checksum ^= _cmd;
  checksum ^= _len;
  for (uint8_t i = 0; i < _len; i++) 
	checksum ^= _data[i];
  return checksum;
}


void DataParse::read(uint8_t *_cmd, uint8_t *_data, uint8_t _len){	
	*_cmd = cmd;
	if(_len > length){
		_len = length;
	}
	memcpy(_data, data, _len);
}	


//数据处理函数
bool DataParse::parse(uint8_t tempChar)		{
	if(step == STEP_WAIT_AA){	        //等待接收0xAA 
		if(tempChar == STXA){
			step = STEP_WAIT_BB;  
		}
	}else if(step == STEP_WAIT_BB){	   //等待接收0xBB
		if(tempChar == STXB){
		    step = STEP_WAIT_CMD;
		}else{
			step = STEP_WAIT_AA;  
		}
	}else if(step == STEP_WAIT_CMD){		//等待接收命令
		cmd = tempChar;
		step = STEP_WAIT_DATA;
		index = 0; 
	}else if(step == STEP_WAIT_DATA){    //等待接收数据
		data[index++] = tempChar;
		if(index >= length){
			step = STEP_WAIT_SUM;
		}
	}else if(step == STEP_WAIT_SUM){     //等待接收校验码
		step = STEP_WAIT_AA;
		if(getChecksum(cmd, data, length) == tempChar){
			return true;
		}
	}
	return false;
}

