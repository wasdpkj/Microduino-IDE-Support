//---------------------------------------
#define SSID        F("Makermodule")
#define PASSWORD    F("microduino")
// What page to grab!
#define HOST_PORT   (80)
#define WEBSITE      F("api.waqi.info")

static const byte  GETDATA[]  PROGMEM = {
  "GET /feed/beijing/?token=571402e73270dc01fbd8e423c0567dd790ed53fb HTTP/1.0\r\nHost: api.waqi.info\r\nConnection: close\r\n\r\n"
};
//从这申请你自己的token: http://aqicn.org/data-platform/token/#/ 替换上面的"demo"字段

//---------------------------------------
#define string_target F("iaqi")
#define string_body F("}")
#define string_data    F("\":{\"v\":")
#define string_time    F("\"time\":{\"s\":\"")

#define MSGNUM 12
static const char string_head[MSGNUM][5] = {
  "co",   //一氧化碳
  "d",    //结露
  "h",    //湿度
  "no2",  //二氧化氮
  "o3",   //臭氧
  "p",    //气压
  "pm10", //PM10
  "pm25", //PM2.5
  "so2",  //二氧化硫
  "t",    //温度
  "w",    //风力
  "wd"    //风向
};

