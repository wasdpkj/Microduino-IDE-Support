// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

/*
*nRF24 mesh网络例程
*
*这个例程说明网络中的主节点处理子节点发过来的数据并依次给子节点发送数据

*/


#include <RF24Mesh.h>

/* 硬件配置: nRF24模块使用SPI通讯外加9脚和10脚 */
RF24 radio(D9, D10);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

struct payload_t {
  unsigned long ms;
  unsigned long counter;
};

uint32_t ctr = 0;


void setup() {
  Serial.begin(115200);

  // 设置节点ID为0， 即为主节点
  mesh.setNodeID(0);
  Serial.println(mesh.getNodeID());
  // 连接到mesh网络
  mesh.begin();

}

uint32_t displayTimer = 0;

void loop() {

  //网络更新， 必须要使用
  mesh.update();

  // 保持'DHCP'服务 主节点必须要使用该方法
  mesh.DHCP();


  //检查是否有收到数据
  if (network.available()) {
    RF24NetworkHeader header;
    network.peek(header);
    Serial.print("Got ");
    uint32_t dat = 0;
    switch (header.type) {
      // Display the incoming millis() values from the sensor nodes
      case 'M':
        network.read(header, &dat, sizeof(dat));
        Serial.print(dat);
        Serial.print(" from RF24Network address 0");
        Serial.println(header.from_node, OCT);
        break;
      default:
        network.read(header, 0, 0);
        Serial.println(header.type);
        break;
    }
  }


  // 每5秒钟给每个节点发送数据
  if (millis() - displayTimer > 5000) {
    ctr++;
    for (int i = 0; i < mesh.addrListTop; i++) {
      payload_t payload = {millis(), ctr};
      if (mesh.addrList[i].nodeID == 1) {  //Searching for node one from address list
        payload = {ctr % 3, ctr};
      }
      RF24NetworkHeader header(mesh.addrList[i].address, OCT); 
      Serial.println( network.write(header, &payload, sizeof(payload)) == 1 ? F("Send OK") : F("Send Fail")); 
    }
    displayTimer = millis();
  }
}
