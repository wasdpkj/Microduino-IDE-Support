#include <Arduino.h>
#define IIC_ADDRESS 0x7E
#define DATA_LENGTH 12
#define THE_RED 0
#define THE_GREEN 1
#define THE_BLUE 2
class MicroMV{
	public:
		MicroMV();
		bool Init();
    bool getData();
    bool getTarget(byte color);
    int getX(byte color);
    int getY(byte color);
    bool isFull(byte color);
  private:
    byte buff[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
};
