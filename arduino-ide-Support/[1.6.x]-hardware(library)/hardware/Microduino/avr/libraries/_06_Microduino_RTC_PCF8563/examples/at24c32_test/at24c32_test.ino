//此示例程序的功能是对RTC模块上的EEPROM进行读写操作

#include <AT24Cxx.h>      //头文件

AT24Cxx at24c32;          //将AT24Cxx类命名为at24c32
uint8_t buf;

void setup (void){
  Serial.begin(9600);    //串口初始化,波特率9600  
  at24c32.begin();        //初始化AT24Cxx
}

void loop() {
  Serial.println("please input a char:");
  while(!Serial.available());                 //等待打开串口监视器
  while (Serial.available() > 0) {            //串口接收到数据 
    char c = Serial.read();                   //把串口读到的数据赋给c
    Serial.print(c);                          //串口打印变量c的值
    Serial.println(" write into the at24c32");
    at24c32.writeMem(0, c);                    //将变量c写到地址为0的寄存器里
    delay(200);
    at24c32.readMem(0, &buf, 1);               //读取地址为0的寄存器里的值，赋给buf
    Serial.write(buf);                        //写二进制数据buf到串口
    Serial.println(" read from the at24c32");   
  }
}

