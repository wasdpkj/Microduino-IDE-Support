/***************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  cuiwenjing@microduino.cc

  Sensor_Ultrasonic维基网址:
  https://wiki.microduino.cn/index.php/Sensor-Ultrasonic/zh

  本示例给出了使用超声波传感器测距离的方法
  并将测得的距离通过串口打印出来
  距离单位毫米（mm）
***************************************/

#include <Microduino_Ultrasonic.h>         //引用超声波库文件

Ultrasonic Ultrasonic1(ULTRASONIC_ADDR_1);//将Ultrasonic实例化

uint16_t Distance;   //定义变量

void setup() {
  Serial.begin(9600);                       // 串口初始化
  if (Ultrasonic1.begin()) {                //如果超声波初始化成功
    Serial.println("Ultrasonic is online");//串口打印信息
  }
  else {                                   //如果超声波初始化不成功
    Serial.println("error");              //串口打印信息
    while (1);                             //程序在此循环运行，即不再向下运行
  }

  if (Ultrasonic1.requstVersion() >= ULTRA_VERSION) { //固件版本为3及以上版本,以下版本无设置获取盲区功能
    Serial.print("Blind:");
    Serial.println(Ultrasonic1.requstBlind()); //将已设置的盲区值打印出来
    //    Ultrasonic1.setBlind(15);            //设置盲区值为15cm,一般情况下不使用此函数
  }
}

void loop() {
  Distance = Ultrasonic1.requstDistance(); //获取超声波测得的距离
  Serial.println(Distance);               //串口打印距离值
  delay(30);                              //延时30ms
}