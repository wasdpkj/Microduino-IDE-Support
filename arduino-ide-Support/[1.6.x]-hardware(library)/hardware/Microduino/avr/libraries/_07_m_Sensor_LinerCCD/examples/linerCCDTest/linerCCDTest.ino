// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============
/*
 *  线性CCD测试示例
 *  设置128个像素时可用processing上位机sketch___CCD看到效果
 *
 *  Created on: 2017年1月12日
 *      Author: ray
 *
 *  线性CCD
 *  可设置返回的像素数128,64,32,16,8,4,2
 *  可设置曝光时间
 *  调整对比度算法
 *  计算最大,最小像素值
 */


#include <Microduino_LinerCCD.h>

LinerCCD linerCCD;//default light pin on A0
//LinerCCD linerCCD(linerCCD.PIXEL128, A1); //default light pin on A1

uint8_t MaxPix = 0;
uint8_t MinPix = 0;

void setup() {
  Serial.begin(38400);

  linerCCD.setCCDPixs(linerCCD.PIXEL128);
  //  linerCCD.setCCDPixs(linerCCD.PIXEL64);// 64 pixel of liner CCD
  //  linerCCD.setCCDPixs(linerCCD.PIXEL32);// 32 pixel of liner CCD
  //  linerCCD.setCCDPixs(linerCCD.PIXEL16);// 16 pixel of liner CCD

  linerCCD.setExposed(50); //exposed during 50ms
}

void loop() {

  //获取原始像素值
  linerCCD.reqestPixls();

  //计算像素最大值，最小值
  linerCCD.caculateMaxMinPix();

  //调整对比度算法
  //Out = Average + (In – Average) * ( 1 + percent)   而percent即调整范围[-1,1]
  //linerCCD.adjustContrastAlgorithm();

  linerCCD.adConAlgorithm();

  //趋近两极算法//////////////////////////////////////////////////////
  /*
    if (MaxPix - MinPix > 10) {
      uint8_t middleValue = (MaxPix + MinPix) / 2;
      for (uint8_t i = 0; i < 128; i++) {
        if ((i + 1) % linerCCD.getTimes() == 0) {    //如果这个点是满足算平均数
          if (linerCCD.pixlArray[i] < middleValue - 5) {
            linerCCD.pixlArray[i] = 0;
          }
          if (linerCCD.pixlArray[i] > middleValue + 5) {
            linerCCD.pixlArray[i] = 255;
          }
        }
      }
    }
  */
  ///////////////////////////////////////////////////////////////////
  uint8_t pixNum = linerCCD.getPixsNum();
  for (uint8_t i = 0; i < pixNum; i++) {
    Serial.print(linerCCD.pixlArray[i]);
    Serial.print(",");
  }
  Serial.println();

  delay(50);
}
