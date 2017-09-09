// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

/*
*nRF24 无线网络发送例程
*
*向接收器发送数据

*/


#include <RF24Network.h>

/* 硬件配置: nRF24模块使用SPI通讯外加9脚和10脚 */
RF24 radio(D9,D10);                 

RF24Network network(radio);          // Network uses that radio

const uint16_t this_node = 01;       // 本机地址
const uint16_t other_node = 00;      // 接收器的地址

const unsigned long interval = 2000; //ms  发送间隔2000ms

unsigned long last_sent;             // 记录发送时刻
unsigned long packets_sent;          // 记录发送次数


struct payload_t {                  // 数据的结构体
  unsigned long ms;
  unsigned long counter;
};

void setup(void)
{
  Serial.begin(115200);
  Serial.println("RF24Network/examples/helloworld_tx/");
  radio.begin();
  //nRF24网络初始化, 使用频道90，本机地址01
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop() {
  
  network.update();                          // 网络更新

  
  unsigned long now = millis();             
  if ( now - last_sent >= interval  )
  {
    last_sent = now;

    Serial.print("Sending...");
    payload_t payload = { millis(), packets_sent++ };
    RF24NetworkHeader header(/*to node*/ other_node);
    bool ok = network.write(header,&payload,sizeof(payload));	//发送数据
    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");
  }
}


