/***************************************
  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  版权所有：
  wasdpkj@hotmail.com

  本示例基于MicroRobot+Core+mCookie-nRF24无线模块，使用nRF透传协议作为受控端
  可用Joypad+遥控器，选择nRF小车模式进行遥控

 ****************************************/

#include "nRFAutoConfig.h"
#include <Microduino_MicroRobot.h>
MicroRobot microRobot;

#define THROTTLE_MAX   160    //<255	油门最大值限制
#define STEERING_MAX   250    //<255	转向最大值限制
#define SERVO_POS_MIN  40     //>0
#define SERVO_POS_MAX  140    //<180

int16_t throttle = 0;   //油门
int16_t steering = 0;   //转向
uint8_t servoPos = SERVO_POS_MIN;

uint16_t Data[8] = {1500, 1500, 1500, 1500, 1000, 1000, 1000, 1000};
uint32_t safeTimer;
bool nrfFlag = false;


void setup() {
  Serial.begin(115200);  //9600/19200/38400

  //LED初始化
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);

  //microRobot初始化
  microRobot.begin();
  for (int a = 1; a <= 4; a++) {
    microRobot.motorInit(a);        //速度分辨率默认为8位，即速度可设范围-255~+255
    microRobot.setMode(a, MODE_OPEN);//电机设置为开环模式
    microRobot.setSpeed(a, 0);       //初始速度设为0
  }

  for (int a = 0; a < 3; a++) {
    tone(D6, 1319);
    delay(100);
    noTone(D6);
    delay(100);
  }

  //定时器清零
  safeTimer = millis();
  Serial.println("Start!");
}

void loop() {
  //运行nRF任务
  runNRFTask(&nrfFlag , Data, &safeTimer);

  //如果nrf数据有更新
  if (nrfFlag) {
    Serial.print("Time at:");
    Serial.print(safeTimer);
    Serial.print(",Data:");
    for (int i = 0; i < 8; i++) {
      Serial.print(Data[i]);
      Serial.print(" ");
    }
    Serial.println();
  }

  //如果设备在线
  if ((millis() - safeTimer < 1000)) {
    digitalWrite(D4, LOW);
    digitalWrite(D5, HIGH);

    servoPos = map(Data[1], 1000, 2000, SERVO_POS_MIN, SERVO_POS_MAX);    //舵机映射角度
    throttle = map(Data[0], 1000, 2000, -THROTTLE_MAX, THROTTLE_MAX);
    steering = map(Data[3], 1000, 2000, -STEERING_MAX, STEERING_MAX);
    if (Data[4] > 1500) {
      microRobot.servoWrite(0, servoPos);
      microRobot.servoWrite(1, 180 - servoPos);
    }
    else if (Data[5] > 1500) {
      microRobot.servoWrite(0, SERVO_POS_MAX);
      microRobot.servoWrite(1, SERVO_POS_MIN);
    }
    else {
      microRobot.servoWrite(0, SERVO_POS_MIN);
      microRobot.servoWrite(1, SERVO_POS_MAX);
    }
    microRobot.setSpeed(1, constrain(throttle + steering, -255, 255));
    microRobot.setSpeed(2, constrain(throttle - steering, -255, 255));
  }
  //如果设备不在线
  else {
    digitalWrite(D4, HIGH);
    digitalWrite(D5, LOW);

    for (int a = 0; a < 4; a++) {
      Data[a] = 1500;
    }
    for (int a = 4; a < 8; a++) {
      Data[a] = 1000;
    }
    microRobot.servoWrite(0, SERVO_POS_MIN);
    microRobot.servoWrite(1, SERVO_POS_MAX);
    microRobot.setSpeed(1, 0);
    microRobot.setSpeed(2, 0);
  }
}

