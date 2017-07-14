/**************************************************************
 本示例给出了将设置返回到出厂默认设置的方法
 Return settings to Factory Defaults
 **************************************************************/

#include <TinyGsmClient.h>

#define SerialMon Serial

// Use Hardware Serial on Microduino/mCookie Core+
//#define SerialAT Serial1

// or Software Serial on Microduino/mCookie Core
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(2, 3); // RX, TX

TinyGsm modem(SerialAT);

void setup() {
  // Set console baud rate
  SerialMon.begin(9600);
  delay(10);

  // Set GSM module baud rate
  SerialAT.begin(9600);
  delay(3000);

  if (!modem.init()) {
    SerialMon.println(F("***********************************************************"));
    SerialMon.println(F(" Cannot initialize modem!"));
    SerialMon.println(F("   Use File -> Examples -> _03_M_Module_GSM_GPRS -> tools -> AT_Debug"));
    SerialMon.println(F("   to find correct configuration"));
    SerialMon.println(F("***********************************************************"));
    return;
  }

  bool ret = modem.factoryDefault();

  SerialMon.println(F("***********************************************************"));
  SerialMon.print  (F(" Return settings to Factory Defaults: "));
  SerialMon.println((ret) ? "OK" : "FAIL");
  SerialMon.println(F("***********************************************************"));
}

void loop() {

}
