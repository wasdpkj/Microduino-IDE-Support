// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

/*
*nRF24 无线射频强度扫描
*
nRF24模块工作在2.4G射频频段，实际有125个工作频道

这个例子可以测量125个频道的各自信号强度，如果测量到某个频道的信号强度较大
说明有某个无线设备正在使用该频道，信号强度越大，使用该频道出现信号冲突，数据丢包的概率越大
建议改用到其他信号强度较弱的频道

*/
/**
 * Channel scanner
 *
 * Example to detect interference on the various channels available.
 * This is a good diagnostic tool to check whether you're picking a
 * good channel for your application.
 *
 * Inspired by cpixip.
 * See http://arduino.cc/forum/index.php/topic,54795.0.html
 */

 
#include <RF24.h>

/* 硬件配置: nRF24模块使用SPI通讯外加9脚和10脚 */
RF24 radio(D9,D10);

//一共有126个射频通道
const uint8_t num_channels = 126;
uint8_t values[num_channels];


void setup(void)
{

  Serial.begin(115200);
  Serial.println(F("\n\rRF24/examples/scanner/"));


  radio.begin();
  radio.setAutoAck(false);			//关闭自动应答

  //开始监听无线数据
  radio.startListening();
 //关闭数据监听
  radio.stopListening();

  radio.printDetails();


  int i = 0;
  while ( i < num_channels )
  {
    Serial.print(i>>4, HEX);
    ++i;
  }
  Serial.println();
  i = 0;
  while ( i < num_channels )
  {
    Serial.print(i&0xf, HEX);
    ++i;
  }
  Serial.println();
}

//
// Loop
//
//每个射频通道的重复测量次数
const int num_reps = 100;

void loop(void)
{
  //清楚所有测量数据
  memset(values,0,sizeof(values));

  // 扫描所有射频通道
  int rep_counter = num_reps;
  while (rep_counter--)
  {
    int i = num_channels;
    while (i--)
    {
      // 设置射频通道
      radio.setChannel(i);

      // 监听一会
      radio.startListening();
      delayMicroseconds(128);
      radio.stopListening();

      // 是否监听到数据
      if ( radio.testCarrier() ){
        ++values[i];
      }
    }
  }

  //打印出所有通道的测量数据
  int i = 0;
  while ( i < num_channels )
  {
    Serial.print(min(0xf,values[i]), HEX);
    ++i;
  }
  Serial.println();
}

// vim:ai:cin:sts=2 sw=2 ft=cpp
