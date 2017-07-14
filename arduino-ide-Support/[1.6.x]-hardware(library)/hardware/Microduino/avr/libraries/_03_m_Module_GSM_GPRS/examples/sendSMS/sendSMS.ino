/**************************************************************
 本示例给出了使用GSM模块的一些方法：
  1、获取设备的信号强度（SignalQuality）
  2、获取设备的身份辨识码（IMEI）
  3、发送短信
 **************************************************************/

#include <TinyGsmClient.h>

// Use Hardware Serial on Microduino/mCookie Core+
#define SerialAT Serial1

// or Software Serial on Microduino/mCookie Core
//#include <SoftwareSerial.h>
//SoftwareSerial SerialAT(2, 3); // RX, TX

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

const char number[] = "+86xxxxxxxxxxx";
const char text[] = "Hello Microduino!";

void setup() {
  // Set console baud rate
  Serial.begin(9600);
  delay(10);

  // Set GSM module baud rate
  SerialAT.begin(9600);
  delay(3000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println("Initializing modem...");
  modem.restart();

  //Unlock your SIM card with a PIN
  //modem.simUnlock("1234");

  Serial.println("Search Network");
  if (!modem.waitForNetwork()) {
    Serial.println("Network Error!");
    while (1);
  }
  Serial.print("SignalQuality:");
  Serial.println(modem.getSignalQuality());
  Serial.print("getIMEI:");
  Serial.println(modem.getIMEI());

  modem.sendSMS(number, text);
}

void loop() {

}

