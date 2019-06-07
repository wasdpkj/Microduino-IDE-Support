/***************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  wasdpkj@hotmail.com

  本示例基于MicroRobot+ESP32指间板，使用BLE协议作为受控端
  可用Joypad+遥控器，选择BLE小车模式进行遥控

 ****************************************/

#include "ESP32Pin.h"
#include "ESP32Tone.h"
#include "userBLE.h"
#include "userGUI.h"

#include <Microduino_MicroRobot.h>
MicroRobot microRobot;
#define MAX_THROTTLE 255
#define MAX_STEERING 200

int16_t throttle, steering;

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO); //TFT屏的类的实例化
GFXcanvas16 FULLLCD = GFXcanvas16(200, 200);
U8G2_FOR_ADAFRUIT_GFX u8g2;

#define TIME_GUI    100   //GUI刷新时间ms 建议大于100 不可调的太低 否则会影响BLE服务稳定性
#define TIME_ROBOT  25

uint16_t rxChannel[8];
bool rxSta = false;

uint32_t timeGUI;
uint32_t timeRobot;

void setup() {
  Serial.begin(NORMAL_BAUD);

  //******BLE的初始化
  initBLEName();
  Serial.print(F("bleName:"));
  Serial.println(getBLEName());
  initBLEServer();

  setupCharService();
  setupBatteryService();
  setupInfoService();

  startBLEAdvertising();
  Serial.println("Waiting a client connection to notify...");

  //******TFT初始化******
  ledcAttachPin(TFT_BL, PWM_CH_BL); //将屏幕背光引脚关联到PWM通道
  ledcSetup(PWM_CH_BL, 10000, 8); // 10 kHz PWM, 8-bit resolution
  tft.begin(8000000L, SPI_PORT);
  //tft.softReset();
  tft.setRotation(0);
  tft.fillScreen(BACKGROUNT_COLOR);
  ledcWrite(PWM_CH_BL, 255);  //0-255
  //******开机画面******
  FULLLCD.fillScreen(BACKGROUNT_COLOR);
  tft.drawRGBBitmap((TFT_WIDTH - FULLLCD.width()) / 2, (TFT_HIGH - FULLLCD.height()) / 2, FULLLCD.getBuffer(), FULLLCD.width(), FULLLCD.height());
  u8g2.begin(FULLLCD);
  u8g2.setFont(FONT);
  u8g2.setFontMode(1);

  //******microRobot初始化
  microRobot.begin();
  for (int a = 1; a <= 4; a++) {
    microRobot.motorInit(a, BIT_8); //速度分辨率设为8位，8~14位可设
  }
  for (int a = 1; a <= 4; a++) {
    microRobot.setMode(a, MODE_OPEN);//电机设置为开环模式
    microRobot.setSpeed(a, 0);       //初始速度设为0
  }


  for (int a = 0; a < 3; a++) {
    tone(PIN_BUZZER, NOTE_E6);
    delay(100);
    noTone(PIN_BUZZER);
    delay(100);
  }
}

void loop() {
  //刷新蓝牙服务
  refreshBLEConnection();


  //Robot
  if (timeRobot > millis()) timeRobot = millis();
  if (millis() - timeRobot > TIME_ROBOT) {
    timeRobot = millis();

    //如果蓝牙连接正常
    if (getBLEConnection()) {
      rxSta = getChannel(rxChannel);

      //安全模式
      if (!rxSta) {
        microRobot.setSpeed(3, FREE);
        microRobot.setSpeed(4, FREE);
      }
      else {
        throttle = map(rxChannel[3], 1000, 2000, -MAX_THROTTLE, MAX_THROTTLE);
        steering = map(rxChannel[0], 1000, 2000, MAX_STEERING, -MAX_STEERING);
        if (throttle < 0) {
          steering = -steering;
        }
        //计算电机差速
        int16_t _motor_vol[2];
        for (int a = 0; a < 2; a++) {
          _motor_vol[a] = throttle;
          _motor_vol[a] += (a == 0 ? -1 : 1) * (steering / 2);
          if (_motor_vol[a] > 255)
            _motor_vol[a] = 255;
          else if (_motor_vol[a] < -255)
            _motor_vol[a] = -255;
        }
        Serial.print(rxChannel[3]);
        Serial.print(",");
        Serial.print(rxChannel[0]);
        Serial.print(" | ");
        Serial.print(throttle);
        Serial.print(",");
        Serial.print(steering);
        Serial.print(" | ");
        Serial.print(_motor_vol[0]);
        Serial.print(",");
        Serial.println(_motor_vol[1]);


        microRobot.setSpeed(3, -_motor_vol[1]);
        microRobot.setSpeed(4, _motor_vol[0]);
      }

      //发送蓝牙数据到主机
      //    pCharacteristic->setValue(&txValue, 1);
      //    pCharacteristic->notify();
      //    txValue++;
      //    delay(10); // bluetooth stack will go into congestion, if too many packets are sent
    }
  }


  //GUI
  if (timeGUI > millis()) timeGUI = millis();
  if (millis() - timeGUI > TIME_GUI) {
    timeGUI = millis();

    //如果蓝牙连接正常
    if (getBLEConnection()) {
      DrawChannel_AtMain_Loop(rxChannel);
    }
    else {
      DrawOffline_AtMain_Loop(getBLEName());
    }
  }

  delay(5);
}
