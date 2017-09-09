// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============


/**
 * Example: nRF24网络协议
 *
 * 在这里例程中，每个节点会每隔一段时间以此轮询其他节点 
 * 使用RF24Network库，可以引导nRF24网络中的数据发送到指定的节点
 *
 *
 * 如果想了解RF24Network库底层的运行机制，可以在RF24Network_config.h文件中使用
 * #define SERIAL_DEBUG.
 *
 * RF24Network组网的机制如下.
 * 地址 00 是主节点. 地址 02，05 表示 网络中的第二层节点， 是主节点的子节点.
 * 地址 012 是 02节点的子节点.  地址 015 是 05节点的子节点.
 * 
 * 下面的树状图解释了该网络中节点之间的关系。
 *
 *        地址/层级          节点号  (To simplify address assignment in this demonstration)
 *             00                  - 主节点        ( 0 )
 *           02  05                - 第一层子节点 ( 1,2 )
 *    32 22 12    15 25 35 45      - 第二层子节点 (7,5,3-4,6,8)
 *
 * 比如 015节点想要和 02节点通讯, 信号需要依次通过 05节点，00主节点，最后到达 02节点。 
 *
 * 需要使用至少2个nRF24模块，并按照以下地址列表进行配置. 
 */
#if defined(__AVR__)
#include <avr/pgmspace.h>
#endif
#if defined(ESP32)
#include <pgmspace.h>
#endif

#include <RF24Network.h>

/***********************************************************************
************* Set the Node Address *************************************
/***********************************************************************/

// 预设的地址
const uint16_t node_address_set[10] = { 00, 02, 05, 012, 015, 022, 025, 032, 035, 045 };
 
// 0 = 主机
// 1-2 (02,05)   = '00'主机的子节点
// 3,5 (012,022) = '02'子机的子节点
// 4,6 (015,025) = '05'子机的子节点
// 7   (032)     = '02'子机的子节点
// 8,9 (035,045) = '05'子机的子节点

uint8_t NODE_ADDRESS = 0;  // 通过数字选择对应的节点地址

/***********************************************************************/
/***********************************************************************/

/* 硬件配置: nRF24模块使用SPI通讯外加9脚和10脚 */
RF24 radio(D9,D10);                         
RF24Network network(radio); 

uint16_t this_node;                           // 本机地址

const unsigned long interval = 1000; // ms       // 通讯间隔1000ms
unsigned long last_time_sent;


const short max_active_nodes = 10;            // 轮询通讯节点的数量
uint16_t active_nodes[max_active_nodes];
short num_active_nodes = 0;
short next_ping_node_index = 0;


bool send_T(uint16_t to);                      // 命令发送方法
bool send_N(uint16_t to);
void handle_T(RF24NetworkHeader& header);
void handle_N(RF24NetworkHeader& header);
void add_node(uint16_t node);


void setup(){
  
  Serial.begin(115200);
  printf_P(PSTR("\n\rRF24Network/examples/meshping/\n\r"));

  this_node = node_address_set[NODE_ADDRESS];            //设置本机地址
  
  radio.begin();
  // 设置无线射频的发射功率为高功率，设置范围RF24_PA_MAX > RF24_PA_HIGH> RF24_PA_LOW .
  radio.setPALevel(RF24_PA_HIGH);
  //nRF24网络初始化, 使用频道100，配置本机地址
  network.begin(/*channel*/ 100, /*node address*/ this_node );

}

void loop(){
    
	network.update();                                   // 网络更新

	while ( network.available() )  {                    // 接收到数据
     
    RF24NetworkHeader header;                           //读取数据
    network.peek(header);

    
      switch (header.type){                              // 根据收到的数据处理相应命令.
        case 'T': handle_T(header); break;
        case 'N': handle_N(header); break;
        default:  printf_P(PSTR("*** WARNING *** Unknown message type %c\n\r"),header.type);
                  network.read(header,0,0);
                  break;
      };
    }

  
  unsigned long now = millis();                         
  if ( now - last_time_sent >= interval ){		    //发送时间到
    last_time_sent = now;

    uint16_t to = 00;                               //设置发送地址为00
        
    if ( num_active_nodes ){                            // 或者有轮询地址
        to = active_nodes[next_ping_node_index++];      // 设置轮询地址
        if ( next_ping_node_index > num_active_nodes ){ 
	    next_ping_node_index = 0;                  
	    to = 00;                                    
        }
    }
    bool ok;

    
    if ( this_node > 00 || to == 00 ){                    // 子节点给主节点发送'T'命令
        ok = send_T(to);   
    }else{                                                // 主节点给子节点发送'N'命令
        ok = send_N(to);
    }
    
    if (ok){                                              // 是否发送成功
        printf_P(PSTR("%lu: APP Send ok\n\r"),millis());
    }else{
        printf_P(PSTR("%lu: APP Send failed\n\r"),millis());
        last_time_sent -= 100;                            
    }
  }


//  delay(50);                          // Delay to allow completion of any serial printing
//  if(!network.available()){
//      network.sleepNode(2,0);         // Sleep this node for 2 seconds or a payload is received (interrupt 0 triggered), whichever comes first
//  }
}

/**
 * 发送'T'命令
 */
bool send_T(uint16_t to)
{
  RF24NetworkHeader header(/*to node*/ to, /*type*/ 'T' /*Time*/);
  
  // The 'T' message that we send is just a ulong, containing the time
  unsigned long message = millis();
  printf_P(PSTR("---------------------------------\n\r"));
  printf_P(PSTR("%lu: APP Sending %lu to 0%o...\n\r"),millis(),message,to);
  return network.write(header,&message,sizeof(unsigned long));
}

/**
 * 发送'N'命令
 */
bool send_N(uint16_t to)
{
  RF24NetworkHeader header(/*to node*/ to, /*type*/ 'N' /*Time*/);
  
  printf_P(PSTR("---------------------------------\n\r"));
  printf_P(PSTR("%lu: APP Sending active nodes to 0%o...\n\r"),millis(),to);
  return network.write(header,active_nodes,sizeof(active_nodes));
}

/**
 * 'T'命令处理方法
 * 添加活跃节点列表
 */
void handle_T(RF24NetworkHeader& header){

  unsigned long message;                                                                      // The 'T' message is just a ulong, containing the time
  network.read(header,&message,sizeof(unsigned long));
  printf_P(PSTR("%lu: APP Received %lu from 0%o\n\r"),millis(),message,header.from_node);


  if ( header.from_node != this_node || header.from_node > 00 )                                // If this message is from ourselves or the base, don't bother adding it to the active nodes.
    add_node(header.from_node);
}

/**
 * 'N'命令处理方法
 */
void handle_N(RF24NetworkHeader& header)
{
  static uint16_t incoming_nodes[max_active_nodes];

  network.read(header,&incoming_nodes,sizeof(incoming_nodes));
  printf_P(PSTR("%lu: APP Received nodes from 0%o\n\r"),millis(),header.from_node);

  int i = 0;
  while ( i < max_active_nodes && incoming_nodes[i] > 00 )
    add_node(incoming_nodes[i++]);
}

/**
 * 将节点加入活跃节点列表中
 */
void add_node(uint16_t node){
  
  short i = num_active_nodes;                                    // Do we already know about this node?
  while (i--)  {
    if ( active_nodes[i] == node )
        break;
  }
  
  if ( i == -1 && num_active_nodes < max_active_nodes ){         // If not, add it to the table
      active_nodes[num_active_nodes++] = node; 
      printf_P(PSTR("%lu: APP Added 0%o to list of active nodes.\n\r"),millis(),node);
  }
}


