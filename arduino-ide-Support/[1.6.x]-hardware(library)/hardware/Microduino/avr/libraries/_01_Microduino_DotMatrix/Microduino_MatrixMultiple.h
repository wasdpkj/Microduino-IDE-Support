#ifndef Microduino_Multiple_h
#define Microduino_Multiple_h

#if (ARDUINO >= 100)
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>
#include "Microduino_Matrix.h"

#define LedMultiple_X 2
#define LedMultiple_Y 2

#define LedMultiple_Num (LedMultiple_X * LedMultiple_Y)

class LedMultiple{
public:
#if LedMultiple_Num>=1
	LedControl a1 = LedControl(64);
#endif
#if LedMultiple_Num>=2
	LedControl a2 = LedControl(63);
#endif
#if LedMultiple_Num>=3
	LedControl a3 = LedControl(62);
#endif
#if LedMultiple_Num>=4
	LedControl a4 = LedControl(61);
#endif
#if LedMultiple_Num>=5
	LedControl a5 = LedControl(60);
#endif
#if LedMultiple_Num>=6
	LedControl a6 = LedControl(59);
#endif
#if LedMultiple_Num>=7
	LedControl a7 = LedControl(58);
#endif
#if LedMultiple_Num>=8
	LedControl a8 = LedControl(57);
#endif
#if LedMultiple_Num>=9
	LedControl a9 = LedControl(56);
#endif
#if LedMultiple_Num>=10
	LedControl a10 = LedControl(55);
#endif
#if LedMultiple_Num>=11
	LedControl a11 = LedControl(54);
#endif
#if LedMultiple_Num>=12
	LedControl a12 = LedControl(53);
#endif
#if LedMultiple_Num>=13
	LedControl a13 = LedControl(52);
#endif
#if LedMultiple_Num>=14
	LedControl a14 = LedControl(51);
#endif
#if LedMultiple_Num>=15
	LedControl a15 = LedControl(50);
#endif
#if LedMultiple_Num>=16
	LedControl a16 = LedControl(49);
#endif
#if LedMultiple_Num>=17
#error "max error"
#endif

//	LedControl m_ledctl;
	//LedControl m(20);
//	virtual void clearDisplay();
	LedMultiple(int _x,int _y);
//	void clearDisplay();
	int16_t getWidth();
	int16_t getHeight();

    void setCursor(int16_t x, int16_t y);
    int getStringLeng( char* _String);
	
    void print(char* c);    
protected:
	int16_t _width, _height; // Display w/h as modified by current rotation
	int16_t cursor_x, cursor_y;
};


#endif