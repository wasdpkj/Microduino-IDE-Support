// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

/*
*nRF24 无线网络接收例程
*
*监听发送器发射的数据，然后打印出来

*/

#include <RF24Network.h>

/* 硬件配置: nRF24模块使用SPI通讯外加9脚和10脚 */
RF24 radio(D9,D10);     

RF24Network network(radio);      
const uint16_t this_node = 00;    // 本机地址
const uint16_t other_node = 01;   // 发送器的地址

struct payload_t {                 // 数据的结构体
  unsigned long ms;
  unsigned long counter;
};


void setup(void)
{
  Serial.begin(115200);
  Serial.println("RF24Network/examples/helloworld_rx/");
 
  radio.begin();
  //nRF24网络初始化, 使用频道90，本机地址00
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop(void){
  
  network.update();                  // 网络更新

  while ( network.available() ) {     // 接收到数据
    
    RF24NetworkHeader header;        
    payload_t payload;
    network.read(header,&payload,sizeof(payload));  //读取数据
    Serial.print("Received packet #");
    Serial.print(payload.counter);
    Serial.print(" at ");
    Serial.println(payload.ms);
  }
}
