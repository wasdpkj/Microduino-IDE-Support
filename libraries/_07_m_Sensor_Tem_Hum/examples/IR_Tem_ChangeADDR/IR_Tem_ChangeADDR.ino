/*********************************************************
  //  LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)

  // 支持Microduino_Sensor_Infrared_TEM

  // Microduino wiki:
  // http://wiki.microduino.cn

  // E-mail:
  // jinqipeng@mircoduino.cc

  //日期：2018.2
*********************************************************/
#include <Microduino_IR_TEM.h>

IR_TEM termo;
const byte oldAddress = 0x5A;
const byte newAddress = 0x5B;

void setup()
{
  Serial.begin(9600); // Initialize Serial to log output
  Serial.println("Press a key to begin");
  while (!Serial.available()) ;
  
  termo.begin(oldAddress); // Try using the old address first
  
  byte address;
  if (!termo.readID()) // Try reading the ID registers
  {
    // If the read fails, try re-initializing with the
    // new address. Maybe we've already set it.
    termo.begin(newAddress);
    
    if (termo.readID()) // Read from the ID registers
    { // If the read succeeded, print the ID:
      Serial.println("Communicated with new address.");
      Serial.println("ID: 0x" + 
                     String(termo.getIDH(), HEX) +
                     String(termo.getIDL(), HEX));
    }
    else
    {
      Serial.println("Failed to communicate with either address.");      
    }
  }
  else
  {
    // If the read suceeds, change the address to something
    // new.
    if (!termo.setAddress(newAddress))
    {
      Serial.println("Failed to set new address.");
    }
    else
    {
      Serial.println("Set the address to 0x" + String(newAddress, HEX));
      Serial.println("Reset the sensor Physically.");
    }
  }
}
//leave the sensor about 2~3cm away
void loop()
{

}
