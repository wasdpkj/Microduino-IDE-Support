#include "Microduino_MatrixMultiple.h"

LedControl _led = 0;


LedMultiple::LedMultiple(int _x,int _y) {	
	this->_width=_x*8;
	this->_height=_y*8;

    this->cursor_y = 0;
    this->cursor_x = 0;

	
/* if(_x*_y>=1)
	LedControl a1 = LedControl(64);
if(_x*_y>=2)
	LedControl a2 = LedControl(63);
if(_x*_y>=3)
	LedControl a3 = LedControl(62);
if(_x*_y>=4)
	LedControl a4 = LedControl(61);
if(_x*_y>=5)
	LedControl a5 = LedControl(60);
if(_x*_y>=6)
	LedControl a6 = LedControl(59);
if(_x*_y>=7)
	LedControl a7 = LedControl(58);
if(_x*_y>=8)
	LedControl a8 = LedControl(57);
if(_x*_y>=9)
	LedControl a9 = LedControl(56); */

//for(int a=0;a<_x*_y;a++)

	//	LedControl m(_addr);

//	MatrixA.clearDisplay();
	
	//m_ledcl.LedControl(_addr);
	
//  _addr--;
  /*
  if ( MatrixCount < 64)
  {
    this->matrixIndex = MatrixCount++;                    // assign a key index to this instance
    if (_addr < 64) {
      matrixs[this->matrixIndex].Addr.nbr = _addr;
    }
  }
  else
    this->matrixIndex = 255 ;  // too many keys
	
  this->Fast_mode=false;
  clearColor();
  */
}

void LedMultiple::setCursor(int16_t _x, int16_t _y) {
  this->cursor_x = _x;
  this->cursor_y = _y;
}

void LedMultiple::print(char* _c) {
//  int a = 0;
  for (int a = getStringLeng(_c); a > -8 * LedMultiple_Num; a--)
  {
  if(cursor_y>=0 && cursor_y<=7)
  {
    for (int b = 0; b <2 ; b++)
    {
      switch (b)
      {
#if LedMultiple_Num>=1
        case 0:
          _led = a1;
          break;
#endif
#if LedMultiple_Num>=2
        case 1:
          _led = a2;
          break;
#endif
#if LedMultiple_Num>=3
        case 2:
          _led = a3;
          break;
#endif
#if LedMultiple_Num>=4
        case 3:
          _led = a4;
          break;
#endif
#if LedMultiple_Num>=5
        case 4:
          _led = a5;
          break;
#endif
#if LedMultiple_Num>=6
        case 5:
          _led = a6;
          break;
#endif
#if LedMultiple_Num>=7
        case 6:
          _led = a7;
          break;
#endif
#if LedMultiple_Num>=8
        case 7:
          _led = a8;
          break;
#endif
#if LedMultiple_Num>=9
        case 8:
          _led = a9;
          break;
#endif
#if LedMultiple_Num>=10
        case 9:
          _led = a10;
          break;
#endif
#if LedMultiple_Num>=11
        case 10:
          _led = a11;
          break;
#endif
#if LedMultiple_Num>=12
        case 11:
          _led = a12;
          break;
#endif
#if LedMultiple_Num>=13
        case 12:
          _led = a13;
          break;
#endif
#if LedMultiple_Num>=14
        case 13:
          _led = a14;
          break;
#endif
#if LedMultiple_Num>=15
        case 14:
          _led = a15;
          break;
#endif
#if LedMultiple_Num>=16
        case 15:
          _led = a16;
          break;
#endif
      }
	  //LedMultiple_X
	  //LedMultiple_Y
      //_led.setCursor(a-getStringLeng(_c)+getWidth()-(24-8*b), 0);	//4x1
      //_led.print(_c);
		
	{
      _led.setCursor(this->cursor_x+a-getStringLeng(_c)+getWidth()-(24-8*b), this->cursor_y);	//4x1
      _led.print(_c);
	}

	  //delay(50);
    }
  }
    else if(cursor_y>7 && cursor_y<=16)
	{
    for (int b = 2; b <4 ; b++)
    {
      switch (b)
      {
#if LedMultiple_Num>=1
        case 0:
          _led = a1;
          break;
#endif
#if LedMultiple_Num>=2
        case 1:
          _led = a2;
          break;
#endif
#if LedMultiple_Num>=3
        case 2:
          _led = a3;
          break;
#endif
#if LedMultiple_Num>=4
        case 3:
          _led = a4;
          break;
#endif
#if LedMultiple_Num>=5
        case 4:
          _led = a5;
          break;
#endif
#if LedMultiple_Num>=6
        case 5:
          _led = a6;
          break;
#endif
#if LedMultiple_Num>=7
        case 6:
          _led = a7;
          break;
#endif
#if LedMultiple_Num>=8
        case 7:
          _led = a8;
          break;
#endif
#if LedMultiple_Num>=9
        case 8:
          _led = a9;
          break;
#endif
#if LedMultiple_Num>=10
        case 9:
          _led = a10;
          break;
#endif
#if LedMultiple_Num>=11
        case 10:
          _led = a11;
          break;
#endif
#if LedMultiple_Num>=12
        case 11:
          _led = a12;
          break;
#endif
#if LedMultiple_Num>=13
        case 12:
          _led = a13;
          break;
#endif
#if LedMultiple_Num>=14
        case 13:
          _led = a14;
          break;
#endif
#if LedMultiple_Num>=15
        case 14:
          _led = a15;
          break;
#endif
#if LedMultiple_Num>=16
        case 15:
          _led = a16;
          break;
#endif
      }
	  //LedMultiple_X
	  //LedMultiple_Y
      //_led.setCursor(a-getStringLeng(_c)+getWidth()-(24-8*b), 0);	//4x1
      //_led.print(_c);
		
	{
      _led.setCursor(this->cursor_x+a-getStringLeng(_c)+getWidth()-(24-8*b), this->cursor_y-8);	//4x1
      _led.print(_c);
	}

	  //delay(50);
    
	}
	}
  }
}

int LedMultiple::getStringLeng(char* _String) {
//    return (uint32_t)(offset + millis() / 1000);
  int _leng = 0;
  int _wight = 0;
  while (_String[_leng] != NULL) _wight += 1 + pgm_read_byte(alphabetBitmap[((int)_String[_leng++] - 32)]+FONE_SIZE_X);
  //Serial.println(_wight);
  return _wight;
}

int LedMultiple::getWidth() {return this->_width;}
int LedMultiple::getHeight() {return this->_height;}
/* void LedControl::clearDisplay() {
} */