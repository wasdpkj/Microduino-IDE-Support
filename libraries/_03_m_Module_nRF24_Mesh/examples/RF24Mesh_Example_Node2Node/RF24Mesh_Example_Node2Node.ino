
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

/*
*nRF24 mesh网络例程
*
*这个例程说明网络中的子节点如何相互通讯

*/

 /** RF24Mesh_Example_Node2NodeExtra.ino by TMRh20
  * 
  * This example sketch shows how to communicate between two (non-master) nodes using
  * RF24Mesh & RF24Network
  */


#include <RF24Mesh.h>
//#include <printf.h>


/**** Configure the nrf24l01 CE and CS pins ****/
RF24 radio(D9,D10);
RF24Network network(radio);
RF24Mesh mesh(radio,network);

/** 
   用户配置: nodeID - 每个节点在网络中拥有的唯一ID，
	0 代表主节点
	1-255 代表子节点.
	
 * otherNodeID - 另一个节点的唯一ID
 * 
 **/
#define nodeID 1
#define otherNodeID 2    


uint32_t millisTimer=0;

void setup() {

  Serial.begin(115200);
  // 设置节点ID
  mesh.setNodeID(nodeID);  
  // 连接到mesh网络
  Serial.println(F("Connecting to the mesh..."));
  mesh.begin();
}


void loop() {
  
  mesh.update();			//网络更新， 必须要使用

  if(network.available()){			//接收到数据
        RF24NetworkHeader header;
        uint32_t mills;
        network.read(header,&mills,sizeof(mills));		//读取数据
        Serial.print("Rcv "); Serial.print(mills);
        Serial.print(" from nodeID ");
        int _ID = mesh.getNodeID(header.from_node);
        if( _ID > 0){
           Serial.println(_ID);
        }else{
           Serial.println("Mesh ID Lookup Failed"); 
        }
  }
  
  
  // 每一秒钟给另外一个节点发送数据
  if(millis() - millisTimer >= 1000){
    millisTimer = millis();
    
    // 发送 'M' 命令（包括发送时间） 到另外一个节点
    if(!mesh.write(&millisTimer,'M',sizeof(millisTimer),otherNodeID)){
            // 如果发送失败，检查是否断开网络连接
			if( ! mesh.checkConnection() ){
              //如果是断开连接，尝试重连
			  Serial.println("Renewing Address");
              mesh.renewAddress(); 
            }else{
              Serial.println("Send fail, Test OK"); 
            }
    }
  }
  
}

