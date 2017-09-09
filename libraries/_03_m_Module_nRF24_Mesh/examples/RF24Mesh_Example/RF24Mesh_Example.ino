// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

/*
*nRF24 mesh网络例程
*
*这个例程说明网络中的子节点怎么给主节点发送数据

*/


#include <RF24Mesh.h>


/* 硬件配置: nRF24模块使用SPI通讯外加9脚和10脚 */
RF24 radio(D9, D10);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

/**
   用户配置: nodeID - 每个节点在网络中拥有的唯一ID，
	0 代表主节点
	1-255 代表子节点.

 **/
#define nodeID 1


uint32_t displayTimer = 0;

struct payload_t {
  unsigned long ms;
  unsigned long counter;
};

void setup() {

  Serial.begin(115200);
  //printf_begin();
  // 设置节点ID
  mesh.setNodeID(nodeID);
  // 连接到mesh网络
  Serial.println(F("Connecting to the mesh..."));
  mesh.begin();
}



void loop() {

  mesh.update();		//网络更新， 必须要使用

  // Send to the master node every second
  if (millis() - displayTimer >= 1000) {
    displayTimer = millis();

    // 发送 'M' 命令（包括发送时间） 到主节点
    if (!mesh.write(&displayTimer, 'M', sizeof(displayTimer))) {

      // 如果发送失败，检查是否断开网络连接
      if ( ! mesh.checkConnection() ) {
        //如果是断开连接，尝试重连
        Serial.println("Renewing Address");
        mesh.renewAddress();
      } else {
        Serial.println("Send fail, Test OK");
      }
    } else {
      Serial.print("Send OK: "); Serial.println(displayTimer);
    }
  }

  while (network.available()) {		//接收到数据
    RF24NetworkHeader header;
    payload_t payload;
    network.read(header, &payload, sizeof(payload));	//读取数据
    Serial.print("Received packet #");
    Serial.print(payload.counter);
    Serial.print(" at ");
    Serial.println(payload.ms);
  }
}






