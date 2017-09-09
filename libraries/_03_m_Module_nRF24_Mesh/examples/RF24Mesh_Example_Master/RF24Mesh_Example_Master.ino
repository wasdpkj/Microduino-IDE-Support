// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

/*
*nRF24 mesh网络例程
*
*这个例程说明网络中的主节点处理子节点发过来的数据

*/
  
  
#include <RF24Mesh.h>
//Include eeprom.h for AVR (Uno, Nano) etc. except ATTiny
#include <EEPROM.h>

/* 硬件配置: nRF24模块使用SPI通讯外加9脚和10脚 */
RF24 radio(D9,D10);
RF24Network network(radio);
RF24Mesh mesh(radio,network);

uint32_t displayTimer = 0;

void setup() {
  Serial.begin(115200);

  // 设置节点ID为0， 即为主节点
  mesh.setNodeID(0);
  Serial.println(mesh.getNodeID());
  // 连接到mesh网络
  mesh.begin();

}


void loop() {    

  //网络更新， 必须要使用
  mesh.update();
  
  // 保持'DHCP'服务 主节点必须要使用该方法
  mesh.DHCP();
    
  //检查是否有收到数据
  if(network.available()){
    RF24NetworkHeader header;
    network.peek(header);
    
    uint32_t dat=0;
    switch(header.type){
      // Display the incoming millis() values from the sensor nodes
      case 'M': network.read(header,&dat,sizeof(dat)); Serial.println(dat); break;
      default: network.read(header,0,0); Serial.println(header.type);break;
    }
  }
  
  //打印网络中的节点列表
  if(millis() - displayTimer > 5000){
    displayTimer = millis();
    Serial.println(" ");
    Serial.println(F("********Assigned Addresses********"));
     for(int i=0; i<mesh.addrListTop; i++){
       Serial.print("NodeID: ");
       Serial.print(mesh.addrList[i].nodeID);
       Serial.print(" RF24Network Address: 0");
       Serial.println(mesh.addrList[i].address,OCT);
     }
    Serial.println(F("**********************************"));
  }
}
