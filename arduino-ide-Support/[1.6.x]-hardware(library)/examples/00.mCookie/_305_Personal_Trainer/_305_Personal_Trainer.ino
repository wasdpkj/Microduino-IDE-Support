#include "Wire.h"
#include "U8glib.h"
#include <Rtc_Pcf8563.h>
#include "I2Cdev.h"
#include "MPU6050.h"

#define buzzer_pin 6

Rtc_Pcf8563 rtc;//初始化实时时钟
/* set a time to start with.
   year, month, weekday, day, hour, minute, second */
DateTime dateTime = {2017, 6, 3, 1, 15, 30, 40};

#define set_time_hour 22
#define set_time_min 20

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//定义OLED连接方式
#define setFont_L u8g.setFont(u8g_font_7x13)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_fixed_v0r)
#define setFont_SS u8g.setFont(u8g_font_fub25n)

#define init_draw 10  //主界面刷新时间

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t change_gx;

boolean sport_en, sport_change, sport_num;
boolean time_en, time_change;

int Sport_num = 8;

unsigned long timer_draw, timer_0;

void setup() {
  pinMode(buzzer_pin, OUTPUT);

  Wire.begin();

  Serial.begin(115200);
  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  rtc.begin();
  rtc.clearAll();
  rtc.setDateTime(dateTime);
}

void loop() {
  rtc.formatDate();
  rtc.formatTime();

  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // these methods (and a few others) are also available
  //accelgyro.getAcceleration(&ax, &ay, &az);
  //accelgyro.getRotation(&gx, &gy, &gz);

  // display tab-separated accel/gyro x/y/z values
  ax = ax / 180;
  ay = ay / 180;
  az = az / 180;
  gx = gx / 180;
  gy = gy / 180;
  gz = gz / 180;

  Serial.print("a/g:\t");
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.println(gz);

  change_gx = (abs(gx) + abs(gy) + abs(gz)) / 3;
  //Serial.println(change_gx);
  if (change_gx > 80 && !sport_en)
    sport_en = true;
  else if (change_gx <= 10 && !sport_num)
  {
    timer_0 = millis();
    sport_num = true;
  }
  if (sport_num)
  {
    if (millis() - timer_0 > 1000)
    {
      change_gx = (abs(gx) + abs(gy) + abs(gz)) / 3;
      if (change_gx <= 10)
      {
        sport_num = false;
        sport_en = false;
        timer_0 = millis();
      }
    }
  }
  /*
    if (gy > 80 && ax > -50)
      sport_en = true;
    else if (gy < -80 && ax < -50)
      sport_en = false;
  */
  if (rtc.getHour() == set_time_hour && rtc.getMinute() == set_time_min && !time_change)
  {
    time_en = true;
    time_change = true;
    tone(buzzer_pin, 300);
  }
  else if (rtc.getHour() != set_time_hour || rtc.getMinute() != set_time_min)
    time_change = false;

  if (sport_change != sport_en && time_en)
  {
    if (sport_en)
    {
      Sport_num -= 1;
      tone(buzzer_pin, 300);
      if (Sport_num == 0)
      {
        time_en = false;
        noTone(buzzer_pin);
      }
    }
    //else
    //  noTone(buzzer_pin);
    //Serial.println(sport_en);
    sport_change = sport_en;
  }

  if (millis() - timer_draw > init_draw)
  {
    u8g.firstPage();
    do {
      draw();
    }
    while ( u8g.nextPage() );
    timer_draw = millis();
  }
}

void draw()
{
  if (!time_en)
  {
    setFont_L;
    u8g.setPrintPos(4, 16);
    u8g.print(rtc.formatDate());
    u8g.print("    ");
    switch (rtc.getWeekday()) {
      case 1:
        u8g.print("Mon");
        break;
      case 2:
        u8g.print("Tue");
        break;
      case 3:
        u8g.print("Wed");
        break;
      case 4:
        u8g.print("Thu");
        break;
      case 5:
        u8g.print("Fri");
        break;
      case 6:
        u8g.print("Sat");
        break;
      case 7:
        u8g.print("Sun");
        break;
    }
    u8g.setPrintPos(4, 64);
    u8g.print("Mcookie..");
    u8g.setPrintPos(92, 64);
    u8g.print(set_time_hour);
    u8g.print(":");
    if (set_time_min < 10)
    {
      u8g.print("0");
      u8g.print(set_time_min);
    }
    else
      u8g.print(set_time_min);

    setFont_SS;
    u8g.setPrintPos(18, 49);
    u8g.print(rtc.getHour());
    u8g.setPrintPos(55, 46);
    if (rtc.getSecond() % 2 == 0)
      u8g.print(":");
    else
      u8g.print(" ");
    u8g.setPrintPos(68, 48);
    if (rtc.getMinute() < 10)
    {
      u8g.print("0");
      u8g.print(rtc.getMinute());
    }
    else
      u8g.print(rtc.getMinute());
  }
  else
  {
    setFont_SS;
    u8g.setPrintPos(50, 49);
    u8g.print(Sport_num);
  }
}
