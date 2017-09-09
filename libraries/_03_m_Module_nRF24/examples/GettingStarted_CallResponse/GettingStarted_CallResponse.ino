// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

/*
*nRF24 无线收发例程, 使用无线自动应答功能
*
*需要使用2个nRF24模块，2个模块配置好对应的收发地址，如下

   模块A                     模块B
  
  接收地址 ---------------- 发送地址

  发送地址 ---------------- 接收地址
  
2个nRF24模块即可实现点对点通讯

2个nRF24模块启动自动应答功能后，每次发射数据后，接收模块都会给发射模块一个应答信号
在应答信号里面可以添加自定义的数据

*/
 
#include <RF24.h>

/* 硬件配置: nRF24模块使用SPI通讯外加9脚和10脚 */
RF24 radio(D9,D10);
/**********************************************************/
    
/* 预先设置好两个通讯地址，总长度为6位   */
byte addresses[][6] = {"1Node","2Node"}; 

// Role management: Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.  
typedef enum { role_ping_out = 1, role_pong_back } role_e;                 
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};  // The debug-friendly names of those roles
role_e role = role_pong_back;                                              // 默认配置为应答角色

byte counter = 1;                                                          // 记录应答次数


void setup(){

  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted_CallResponse"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
 
  // Setup and configure radio

  radio.begin();

  radio.enableAckPayload();                 // 启动自动应答
  radio.enableDynamicPayloads();     

  radio.setPayloadSize(1); 					// 设置自动应答的数据长度      
  
  //配置发射地址和接收地址
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);

  //开始监听无线数据
  radio.startListening();                       // Start listening  
  
  radio.writeAckPayload(1,&counter,1);          // 预先往自动应答FIFO填入需要返回的数据
  
  radio.printDetails();
}

void loop(void) {

  
/****************** 发射角色 ***************************/
  if (role == role_ping_out){                               // Radio is in ping mode

    byte gotByte;                                         
    
    radio.stopListening();                                  // 发射数据前需要关闭数据监听     
    Serial.print(F("Now sending "));                        
    Serial.println(counter);
    
    unsigned long time = micros();                          // 记录下发射时刻   
                                                            
    if ( radio.write(&counter,1) ){                         // 发射数据 
        if(!radio.available()){                             // 发射成功后没有收到应答数据，说明收到一个空的应答，可能是接收方没有启动自动应答
            Serial.print(F("Got blank response. round-trip delay: "));
            Serial.print(micros()-time);
            Serial.println(F(" microseconds"));     
        }else{      
            while(radio.available() ){                      // 收到应答数据
                radio.read( &gotByte, 1 );                  // 读取应答数据
                unsigned long timer = micros();
                
                Serial.print(F("Got response "));
                Serial.print(gotByte);
                Serial.print(F(" round-trip delay: "));
                Serial.print(timer-time);
                Serial.println(F(" microseconds"));
                counter++;                                  // 应答次数+1
            }
        }
    
    }else{        
		Serial.println(F("Sending failed.")); 				 // If no ack response, sending failed
	}         
    
    delay(1000);  // Try again later
  }


/****************** 应答角色 ***************************/

  if ( role == role_pong_back ) {
    byte pipeNo, gotByte;                          
    while( radio.available(&pipeNo)){         // 如果接收到数据
      radio.read( &gotByte, 1 );              // 读出数据     
                                                   
      gotByte += 1;                                
      radio.writeAckPayload(pipeNo,&gotByte, 1 );  // 预先往自动应答FIFO更新需要返回的数据
      Serial.print(F("Loaded next response "));
      Serial.println(gotByte);  
   }
 }

/****************** 通过串口命令切换发射或接收程序 ***************************/
/*
	串口发命令'T', 程序切换成发射角色
	串口发命令'R', 程序切换成应答角色
*/
  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == role_pong_back ){      
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      role = role_ping_out;
	  //切换发射地址和接收地址
      radio.openWritingPipe(addresses[1]);        // Both radios listen on the same pipes by default, but opposite addresses
      radio.openReadingPipe(1,addresses[0]);      // Open a reading pipe on address 0, pipe 1
      counter = 1;
   }else
    if ( c == 'R' && role == role_ping_out ){
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));      
       role = role_pong_back; 
	   //切换发射地址和接收地址
       radio.openWritingPipe(addresses[0]);
       radio.openReadingPipe(1,addresses[1]);
       radio.startListening();
       counter = 1;
       radio.writeAckPayload(1,&counter,1);
       
    }
  }
  
}