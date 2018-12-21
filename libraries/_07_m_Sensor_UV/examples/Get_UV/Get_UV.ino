/*********************************************************
  //  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  // 支持Microduino_Sensor_UV

  // Microduino wiki:
  // http://wiki.microduino.cn
  //
  // E-mail:
  // jinqipeng@mircoduino.cc
  //USE MICRODUINO - OLED TO SHOW.
  //OR COM PORT IS OK TOO.
  //THE OUTPUT IS VOLTAGE(FROM 0.98~3.3V power)
  //日期：2018.2
*********************************************************/
#include <Microduino_Sensor-UV.h>
//will use a oled screen to show the voltage.
//convenient while testing outside
#include <U8glib.h>

//CHECK THE +3.3V VOLTAGE ON YOUR BOARD AND FIX VREF TO GET A CORRECT FEED IF NEEDED.
#define VREF 3.34

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

SensorUV UV(UV_ADDRESS);

float voltage=0;

void draw(void) {
  u8g.setFont(u8g_font_7x13);
  u8g.setPrintPos(0, 20);
  // call procedure from base class, http://arduino.cc/en/Serial/Print
  u8g.print(voltage);
}
void setup()
{
  Serial.begin(9600); //串口初始化
  Serial.println("Microduino_Sensor UV!");

  while (!UV.begin())
  {
    Serial.println("Something Wrong while Initialing");   //紫外线传感器初始化
  }
  Serial.print("Version : ");
  Serial.println(UV.getVersion());
}

void loop()
{
  if(Serial.available())
  {
    char c = Serial.read();
    UV.setReset();
  }

  voltage = UV.getUV();
  voltage = (VREF * voltage) / 1023;
  Serial.print("OUTPUT : ");
  Serial.println(voltage);
  u8g.firstPage();
  do {
    draw();
  } while ( u8g.nextPage() );
}
