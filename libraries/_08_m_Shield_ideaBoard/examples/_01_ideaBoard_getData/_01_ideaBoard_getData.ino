/*************************************************
  本示例给出了获取ideaBoard上输入传感器的数值的方法
  包含：左侧三路输入模拟信号值（0-255）
        摇杆X轴模拟值（0-255）
        摇杆Y轴模拟值（0-255）
        光照值（0-255）
        声音值（0-255）

        按键A(0/1)
        按键B(0/1)
        摇杆上(0/1)
        摇杆下(0/1)
        摇杆左(0/1)
        摇杆右(0/1)
****************************************
   2017-08-10  by CWJ
****************************************/

#include <Microduino_ideaBoard.h>

#if !defined (__AVR_ATmega328P__)
#error "Board selection error,you should choose Core(328p)@16M,5V"
#endif

uint8_t adcData[7];
bool button_a, button_b, button_u, button_d, button_l, button_r;

void setup() {
  Serial.begin(9600);
  ideaBoard.begin();
  Serial.println("TEST START");
}

void loop() {
  //*****************get********获取数据
  adcData[0] = ideaBoard.getIN(0);
  adcData[1] = ideaBoard.getIN(1);
  adcData[2] = ideaBoard.getIN(2);
  adcData[3] = ideaBoard.getJoystickX();
  adcData[4] = ideaBoard.getJoystickY();
  adcData[5] = ideaBoard.getLight();
  adcData[6] = ideaBoard.getMic();

  button_a = ideaBoard.getButtonA();
  button_b = ideaBoard.getButtonB();
  button_u = ideaBoard.getButtonUp();
  button_d = ideaBoard.getButtonDown();
  button_l = ideaBoard.getButtonLeft();
  button_r = ideaBoard.getButtonRight();

  //*****************print*****数据监视
  Serial.print("IN[3]: ");
  Serial.print(adcData[0]);         // print the character
  Serial.print(", ");
  Serial.print(adcData[1]);         // print the character
  Serial.print(", ");
  Serial.print(adcData[2]);         // print the character
  Serial.print(", \t");
  Serial.print("Joystick_X_Y: ");
  Serial.print(adcData[3]);         // print the character
  Serial.print(", ");
  Serial.print(adcData[4]);         // print the character
  Serial.print(",\t");
  Serial.print("Light: ");
  Serial.print(adcData[5]);         // print the character
  Serial.print(", ");
  Serial.print("Mic: ");
  Serial.print(adcData[6]);         // print the character
  Serial.print(", \t");

  Serial.print("Button[6]: ");
  Serial.print(button_a);         // print the character
  Serial.print(", ");
  Serial.print(button_b);         // print the character
  Serial.print(", ");
  Serial.print(button_u);         // print the character
  Serial.print(", ");
  Serial.print(button_d);         // print the character
  Serial.print(", ");
  Serial.print(button_l);         // print the character
  Serial.print(", ");
  Serial.print(button_r);         // print the character
  Serial.println(" ");         // print the character

  delay(10);
}
