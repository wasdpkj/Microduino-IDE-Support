// U8g2_for_TFT library
#include <U8g2_for_Adafruit_GFX.h>
#define FONT u8g2_font_wqy16_t_gb2312
extern U8G2_FOR_ADAFRUIT_GFX u8g2;

#include <Adafruit_GFX.h>
#include <Adafruit_TFT.h>
extern Adafruit_ST7789 tft;
extern GFXcanvas16 FULLLCD;

const char *NAME_ROBOT[8] = {       //八通道静态界面显示名称
  "CH0", "CH1", "CH2", "CH3", "CH4", "CH5", "CH6", "CH7"
};

#define PWM_CH_BL 2          //PWM通道 背光

#define TFT_HIGH 240
#define TFT_WIDTH 240

#define FONT_HIGH 12

#define TOPBAR_HIGH 32  //顶部bar高度

#define CHANNEL_BAR_WIDTH 100
#define CHANNEL_BAR_HEIGHT 14

#define BACKGROUNT_COLOR tft.color565(255, 255, 255)
#define TEXT_COLOR    ST7789_BLACK

#define CH_TEXT_COLOR ST7789_WHITE  //nRF.CH 字体颜色
#define CH_DATA_COLOR tft.color565(20, 200, 240)  //通道蓝色
#define CH_FILL_COLOR tft.color565(124, 215, 132) //通道绿色


void drawChannelOne(int _startX, int _startY, int _width, int _height, int _data, int _per, int _i) {
  int _dataCache;
  if (_data != 0) {
    _dataCache = map((constrain(_data, -100, 100)), -100, 100, -((_width / 2 - 2) * _per / 100) - 1, ((_width / 2 - 2) * _per / 100));
  } else {
    _dataCache = 0;
  }

  FULLLCD.drawRect(_startX, _startY + _i * (FONT_HIGH + 6), _width, _height, ST7789_BLACK);
  if (_dataCache > 0) {
    FULLLCD.fillRect(_startX + 1, _startY + _i * (FONT_HIGH + 6) + 1,  _width / 2 - 2, _height - 2, BACKGROUNT_COLOR);
    FULLLCD.fillRect(_startX + _width / 2 + 1, _startY + _i * (FONT_HIGH + 6) + 1,  _dataCache, _height - 2, CH_FILL_COLOR);
    FULLLCD.fillRect(_startX + _width / 2 + _dataCache + 1, _startY + _i * (FONT_HIGH + 6) + 1, _width / 2 - _dataCache - 2, _height - 2, BACKGROUNT_COLOR);
    FULLLCD.drawLine(_startX + _width / 2, _startY + _i * (FONT_HIGH + 6), _startX + _width / 2, _startY + _height + _i * (FONT_HIGH + 6) - 2, CH_DATA_COLOR);
  } else {
    FULLLCD.fillRect(_startX + 1, _startY + _i * (FONT_HIGH + 6) + 1, _width / 2 + _dataCache, _height - 2, BACKGROUNT_COLOR);
    FULLLCD.fillRect(_startX + _width / 2 + _dataCache, _startY + _i * (FONT_HIGH + 6) + 1,  -_dataCache, _height - 2, CH_FILL_COLOR);
    FULLLCD.fillRect(_startX + _width / 2 + 1, _startY + _i * (FONT_HIGH + 6) + 1, _width / 2 - 2, _height - 2, BACKGROUNT_COLOR);
    FULLLCD.drawLine(_startX + _width / 2, _startY + _i * (FONT_HIGH + 6) + 1, _startX + _width / 2, _startY + _height + _i * (FONT_HIGH + 6) - 2, CH_DATA_COLOR);
  }
}

//动态界面更新函数
void DrawOffline_AtMain_Loop(String _name) {
  FULLLCD.fillScreen(BACKGROUNT_COLOR);

  u8g2.setForegroundColor(TEXT_COLOR);      // apply Adafruit GFX color
  u8g2.setCursor(0, FONT_HIGH + 5);
  u8g2.print(F("  电池电压: "));
  u8g2.print(pmu.getVoltage(5.0), 1);
  u8g2.print(F(" V"));
  
  u8g2.setForegroundColor(TEXT_COLOR);      // apply Adafruit GFX color
  u8g2.setCursor(0, 55);
  u8g2.print(F("\
  BLE离线\n\r\
  \n\r\
  "));
  u8g2.print(F("请连设备:\""));
  u8g2.print(_name);
  u8g2.print(F("\""));

  tft.drawRGBBitmap((TFT_WIDTH - FULLLCD.width()) / 2, (TFT_HIGH - FULLLCD.height()) / 2, FULLLCD.getBuffer(), FULLLCD.width(), FULLLCD.height());
}

void DrawChannel_AtMain_Loop(uint16_t * _channel) {
  FULLLCD.fillScreen(BACKGROUNT_COLOR);

  u8g2.setForegroundColor(TEXT_COLOR);      // apply Adafruit GFX color
  u8g2.setCursor(0, FONT_HIGH + 5);
  u8g2.print(F("  电池电压: "));
  u8g2.print(pmu.getVoltage(5.0), 1);
  u8g2.print(F(" V"));

  for (uint8_t a = 0; a < 8; a++) {
    u8g2.setForegroundColor(TEXT_COLOR);
    u8g2.setCursor(10, 5 + TOPBAR_HIGH + a * (FONT_HIGH + 6) + FONT_HIGH);
    u8g2.print(NAME_ROBOT[a]);
  }

  int16_t _chData[4];
  //前四通道数据图示
  for (int i = 0; i < 4; i++) {
    int16_t _chData = map(_channel[i], 1000, 2000 , -100, 100);
    drawChannelOne(FULLLCD.width() * 3 / 10, 5 + TOPBAR_HIGH, CHANNEL_BAR_WIDTH, CHANNEL_BAR_HEIGHT, _chData, 100, i);
  }

  //后四通道数据图示
  for (int i = 4; i < 8; i++) {
    if (_channel[i] < 1500) {
      drawChannelOne(FULLLCD.width() * 3 / 10, 5 + TOPBAR_HIGH, CHANNEL_BAR_WIDTH, CHANNEL_BAR_HEIGHT, -100, 100, i);
    } else {
      drawChannelOne(FULLLCD.width() * 3 / 10, 5 + TOPBAR_HIGH, CHANNEL_BAR_WIDTH, CHANNEL_BAR_HEIGHT, 100, 100, i);
    }
  }

  //8通道数据显示
  for (uint8_t a = 0; a < 8; a++) {
    u8g2.setForegroundColor(TEXT_COLOR);      // apply Adafruit GFX color
    u8g2.setCursor(FULLLCD.width() * 8 / 10, 5 + TOPBAR_HIGH + a * (FONT_HIGH + 6) + FONT_HIGH);
    u8g2.print(_channel[a]);
  }

  tft.drawRGBBitmap((TFT_WIDTH - FULLLCD.width()) / 2, (TFT_HIGH - FULLLCD.height()) / 2, FULLLCD.getBuffer(), FULLLCD.width(), FULLLCD.height());
}

