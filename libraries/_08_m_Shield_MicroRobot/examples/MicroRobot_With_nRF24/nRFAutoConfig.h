#ifndef __NOC_H__
#define __NOC_H__

#include <stdlib.h>
#include <EEPROM.h>
#include <RF24Network.h>

#define EEPROM_ADDR       0
#define PUBLIC_CHANNEL    0
#define ADDR_NODE    00
#define ADDR_MASTER  01
#define LOST_TIM_MAX       500

#define CHANNEL_NUM        8


//nRF状态
enum STATE_T {
  STATE_INIT = 0,
  STATE_CHECK_MATCHED,
  STATE_PUBLIC_CHANNEL_INIT,
  STATE_PUBLIC_CHANNEL,
  STATE_PRIVIATE_CHANNEL_INIT,
  STATE_PRIVIATE_CHANNEL,
  STATE_ROTATE
};

struct payload_public_t {                  // 数据的结构体
  int priviate_chn;
};

struct receive_a {
  uint32_t ms;
  uint16_t rf_CH[CHANNEL_NUM];
};

struct send_a {
  uint32_t node_ms;
};


//返回1 正常配对
//返回0 未正常配对
bool runNRFTask(bool * _dataFlag , uint16_t * _data, uint32_t * _safeTimer) {
  *_dataFlag = false;   //是否收到数据 初始
  //network如何定义要待定
  static int state_NRF = STATE_INIT;
  static RF24 radio(D9, D10);
  static RF24Network network(radio);
  static int priviate_channel = 0;
  static int state_NRF_init = 0;
  static int rotate_flag = 0;
  static unsigned long now_1, now_2, now_3, now_4, tim_1, tim_2 = 0;

  switch (state_NRF) {
    case STATE_INIT: {
        state_NRF = STATE_CHECK_MATCHED;
        break;
      }

    case STATE_CHECK_MATCHED: { 
        byte _eepromData = EEPROM.read(EEPROM_ADDR);
        if (_eepromData == PUBLIC_CHANNEL) {
          state_NRF = STATE_PUBLIC_CHANNEL_INIT;
          rotate_flag = 0;
        }
        else
          state_NRF = STATE_ROTATE;
        break;
      }

    case STATE_PUBLIC_CHANNEL_INIT: { 
        radio.begin();
        radio.setDataRate(RF24_250KBPS);
        network.begin(PUBLIC_CHANNEL, ADDR_NODE);
        now_1 = millis();
        //Serial.println("STATE_PUBLIC_CHANNEL_INIT success");
        state_NRF = STATE_PUBLIC_CHANNEL;
        break;
      }

    //公共通道配对，并接收priviate_chn信息
    case STATE_PUBLIC_CHANNEL: {  
        network.update();
        while (network.available()) {
          now_1 = millis();
          RF24NetworkHeader header;
          payload_public_t payload_public;
          network.read(header, &payload_public, sizeof(payload_public));  //读取数据
          priviate_channel = payload_public.priviate_chn;
          EEPROM.write(EEPROM_ADDR, priviate_channel);  //收到了新的 就写进去
          //Serial.println("STATE_PUBLIC_CHANNEL match success");
          //Serial.print("The new channel is ");
          //Serial.println(priviate_channel);
          state_NRF = STATE_PRIVIATE_CHANNEL_INIT;
        }
        now_2 = millis();
        if (now_2 - now_1 > LOST_TIM_MAX && rotate_flag == 1)
          state_NRF = STATE_PRIVIATE_CHANNEL_INIT;
        else if (now_2 - now_1 > LOST_TIM_MAX)
          state_NRF = STATE_PUBLIC_CHANNEL_INIT;
        break;
      }

    //设置新的通道
    case STATE_PRIVIATE_CHANNEL_INIT: { 
        priviate_channel = EEPROM.read(EEPROM_ADDR);
        radio.begin();
        radio.setDataRate(RF24_250KBPS);
        network.begin(priviate_channel, ADDR_NODE);
        //Serial.println("STATE_PRIVIATE_CHANNEL_INIT success");
        //Serial.print("The priviate_channel is : ");
        //Serial.println(priviate_channel);
        state_NRF = STATE_PRIVIATE_CHANNEL ;
        now_3 = millis();
        break;
      }

    //新通道配对，并接收新通道的信息
    case STATE_PRIVIATE_CHANNEL : { 
        network.update();
        if (network.available()) {
          *_safeTimer = millis();
          now_3 = millis();
          RF24NetworkHeader header;
          receive_a rec;
          network.read(header, &rec, sizeof(rec));
          for (int a = 0; a < CHANNEL_NUM; a++) {
            *(_data + a) = rec.rf_CH[a];
            //Serial.print(*(_data + a));
            //Serial.print(" ");
          }

          //Serial.println(" ");

          *_dataFlag = true;    //收到数据
          RF24NetworkHeader talker(ADDR_MASTER);
          //          network.write(talker,  Data_to_Rc, sizeof(Data_to_Rc));
        }
        now_4 = millis();
        if (now_4 - now_3 > LOST_TIM_MAX)
          state_NRF = STATE_ROTATE;
        break;
      }

    case STATE_ROTATE: {
        rotate_flag = 1;
        state_NRF = STATE_PUBLIC_CHANNEL_INIT;
        break;
      }
    default:
      break;
  }

  return bool(state_NRF == STATE_PRIVIATE_CHANNEL);
}


uint16_t getChannelData(uint8_t _ch, uint16_t * _data) {
  return *(_data + _ch);
}

#endif
