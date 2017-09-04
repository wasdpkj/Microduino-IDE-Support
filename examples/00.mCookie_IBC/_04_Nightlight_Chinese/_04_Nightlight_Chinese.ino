/*
  =================================================================================
                              | 什么是路灯? |
  ---------------------------------------------------------------------------------
  怕黑? 不用再怕了。
  这是一个智能路灯!

  周围亮的时候:
  路灯不会亮。

  周围暗下来的时候:
  一有声音路灯就会亮。

  随时按碰撞传感器(不管周围是亮还是暗)也会点亮路灯。

  路灯亮一次会持续10秒钟。


  =================================================================================
                                    | 开始 |
  ---------------------------------------------------------------------------------
  用传感器让将路灯变成自动化，同时变得更加独特有趣。
  =================================================================================
                            | 自己动手修改代码! |
  ---------------------------------------------------------------------------------
  点开 userDef.h 文件，然后根据你自己的喜好更改下面的参数。 :)

  VOICE_MIN:  声音检测的触发值。
  LIGHT_MAX:  光照检测的触发值。大于该值，即认为是亮；小于该值，即认为是暗。
  TIME_RUN:   路灯亮的持续时间。
  BRIGHT_MAX: LED彩灯显示的最大亮度。

  修改完这些值后，记得再把代码上传到mBattery里。
  =================================================================================
                               | 头脑风暴 |
  ---------------------------------------------------------------------------------
  1. 还能用其他的什么来控制路灯?
     计时控制? 例如，它每天晚上6点开始亮。
  2. 可以加一个电机，把LED贴在上面吗？
     一个旋转灯可能不太适合做路灯了！
  3. 可以加一个蜂鸣器，把它变成一个警报监测装置吗？
     这个路灯有些不务正业了。:)

    注意: 以上功能上的变动会涉及到程序上的改动。
  =================================================================================
                                |端口连接|
  ---------------------------------------------------------------------------------
  光敏传感器:     A2
  声音检测传感器: A6
  ColorLED:       D12
  碰撞传感器:     D6
  =================================================================================
                                   | 问题 |
  ---------------------------------------------------------------------------------
  欢迎访问 http://forum.microduino.cn/，在我们的论坛提交您的问题， 我们会尽快回复您。
*/

#include <Microduino_Key.h>
#include"colorLed.h"
#include"userDef.h"

AnalogKey keyMic(PIN_MIC);
AnalogKey keyLight(PIN_LIGHT);
DigitalKey keyButton(PIN_KEY);

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);
  keyMic.begin(INPUT);
  keyLight.begin(INPUT);
  keyButton.begin(INPUT_PULLUP);
#if DEBUG
  Serial.println("**************START************");
#endif
}

void loop() {
#if DEBUG
  Serial.print("LIGHT Val:");
  Serial.print(analogRead(PIN_LIGHT));
  Serial.print("\tMIC Val:");
  Serial.println(analogRead(PIN_MIC));
#endif

  if (keyLight.readEvent(LIGHT_MIN, LIGHT_MAX) == LONG_PRESS && keyMic.readEvent(VOICE_MIN, VOICE_MAX) == SHORT_PRESS
      || keyButton.readEvent() == SHORT_PRESS)
  {
    uint32_t ledonTime = millis();
    while (millis() - ledonTime < TIME_ON)
      //rainbow(10);
      //ledBreath(COLOR_PURPLE, 10);
      setAllColor(COLOR_WARM);
  }
  else
    setAllColor(COLOR_NONE);
  delay(15);
}
