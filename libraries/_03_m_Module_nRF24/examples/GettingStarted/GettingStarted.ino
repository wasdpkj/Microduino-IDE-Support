// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

/*
*nRF24 无线收发例程
*
*需要使用2个nRF24模块，2个模块配置好对应的收发地址，如下

   模块A                     模块B
  
  接收地址 ---------------- 发送地址

  发送地址 ---------------- 接收地址
  
2个nRF24模块即可实现点对点通讯

*/

#include <RF24.h>

/* 硬件配置: nRF24模块使用SPI通讯外加9脚和10脚 */
RF24 radio(D9,D10);
/**********************************************************/

/* 预先设置好两个通讯地址，总长度为6位   */
uint8_t addresses[][6] = {"1Node","2Node"};

// 0代表程序默认为应答角色，1为发射角色
bool role = 0;

void setup() {
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  
  radio.begin();

  // 设置无线射频的发射功率为低功率，设置范围RF24_PA_MAX > RF24_PA_HIGH> RF24_PA_LOW .
  radio.setPALevel(RF24_PA_LOW);
  
  //配置发射地址和接收地址
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  
  //开始监听无线数据
  radio.startListening();
  
  radio.printDetails();
}

void loop() {
  
/****************** 发射角色 ***************************/  
  if (role == 1)  {
    
    radio.stopListening();                         // 发射数据前需要关闭数据监听.
    
    Serial.println(F("Now sending"));

    unsigned long start_time = micros();                    // 记录下发射时刻
     if (!radio.write( &start_time, sizeof(unsigned long) )){  //发射数据
       Serial.println(F("failed"));						    
     }
        
    radio.startListening();                                 // 数据发射完后重新开始监听数据
    
    unsigned long started_waiting_at = micros();               // 记录下发射完的时刻
    boolean timeout = false;                                   
    
    while ( ! radio.available() ){                             // 等待对方返回数据
      if (micros() - started_waiting_at > 200000 ){            // 等待200ms后不再等待
          timeout = true;
          break;
      }      
    }
        
    if ( timeout ){                                             // 打印等待返回数据的结果
        Serial.println(F("Failed, response timed out."));
    }else{
        unsigned long got_time;                                 
        radio.read(&got_time, sizeof(unsigned long));			// 读出返回的数据
        unsigned long end_time = micros();
        
        // Spew it
        Serial.print(F("Sent "));
        Serial.print(start_time);
        Serial.print(F(", Got response "));
        Serial.print(got_time);
        Serial.print(F(", Round-trip delay "));
        Serial.print(end_time-start_time);
        Serial.println(F(" microseconds"));
    }
    // Try again 1s later
    delay(1000);
  }

/****************** 应答角色 ***************************/

  if ( role == 0 )
  {
    unsigned long got_time;
    
    if( radio.available()){									// 如果接收到数据
                                                            
      while (radio.available()) {                           
        radio.read( &got_time, sizeof(unsigned long) );     // 读出数据
      }
     
      radio.stopListening();                                // 停止监听，准备发送返回数据
      radio.write( &got_time, sizeof(unsigned long) );      // 发送返回数据    
      radio.startListening();                               // 数据发送完后，重新开始监听    
      Serial.print(F("Sent response "));
      Serial.println(got_time);  
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
    if ( c == 'T' && role == 0 ){     	 
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      role = 1;                  // Become the primary transmitter (ping out)
      //切换发射地址和接收地址
	  radio.openWritingPipe(addresses[1]);
      radio.openReadingPipe(1,addresses[0]);
    
   }else
    if ( c == 'R' && role == 1 ){
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));      
       role = 0;                // Become the primary receiver (pong back)
       //切换发射地址和接收地址
	   radio.openWritingPipe(addresses[0]);
       radio.openReadingPipe(1,addresses[1]);
       radio.startListening();       
    }
  }

} // Loop
