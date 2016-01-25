#include "Microduino_Matrix.h"

static matrix_t matrixs[64];                          // static array of key structures

uint8_t MatrixCount = 0;                                     // the total number of attached keys

LedControl::LedControl(int _addr) {
  _addr--;
  if ( MatrixCount < 64)
  {
    this->matrixIndex = MatrixCount++;                    // assign a key index to this instance
    if (_addr < 64) {
      matrixs[this->matrixIndex].Addr.nbr = _addr;
    }
  }
  else
    this->matrixIndex = 255 ;  // too many keys

  this->Fast_mode = false;
  clearColor();
}

int LedControl::getDeviceAddr() {
  return this->Devices_addr;
}

void LedControl::clearFastMode()
{
  this->Fast_mode = false;
}

void LedControl::setFastMode()
{
  this->Fast_mode = true;
}

void LedControl::clearColor()
{
  this->value_color[0] = 255;
  this->value_color[1] = 255;
  this->value_color[2] = 255;
}

void LedControl::setColor(uint8_t value_r, uint8_t value_g, uint8_t value_b)
{
  this->value_color[0] = value_r;
  this->value_color[1] = value_g;
  this->value_color[2] = value_b;
}

void LedControl::clearDisplay() {
  //Wire.beginTransmission(64); // transmit to device #4
  Wire.beginTransmission(matrixs[this->matrixIndex].Addr.nbr + 1); // transmit to device #4
  Wire.write(0x60);       // sends five bytes
  Wire.endTransmission();    // stop transmitting
  /*   for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
        setLed(i, j, false);
    } */
}


void LedControl::setLedColor(int _row, int _col, uint8_t _value_r, uint8_t _value_g, uint8_t _value_b) {
  //Wire.beginTransmission(64); // transmit to device #4
  Wire.beginTransmission(matrixs[this->matrixIndex].Addr.nbr + 1); // transmit to device #4
  uint8_t temp[4];
  temp[0] = 0x80 | (_row << 3) | _col;
  temp[1] = _value_b / 8;
  temp[2] = 0x20 | _value_g / 8;
  temp[3] = 0x40 | _value_r / 8;
  Wire.write(temp, 4);       // sends five bytes
  Wire.endTransmission();    // stop transmitting
}

void LedControl::setLedColorFast(int _row, int _col, uint8_t _value_r, uint8_t _value_g, uint8_t _value_b) {
  //Wire.beginTransmission(64); // transmit to device #4
  Wire.beginTransmission(matrixs[this->matrixIndex].Addr.nbr + 1); // transmit to device #4
  uint8_t temp[2];
  temp[0] = 0xC0 | (_row << 3) | _col;
  temp[1] = ((_value_b / 64) << 4) | ((_value_g / 64) << 2) | (_value_r / 64);
  Wire.write(temp, 2);       // sends five bytes
  Wire.endTransmission();    // stop transmitting
}

void LedControl::setLed(int _row, int _col, boolean _state) {
  if (_row < 0 || _row > 7 || _col < 0 || _col > 7)
    return;

  if (_state)
  {
    if (this->Fast_mode)
      this->setLedColorFast(_row, _col, this->value_color[0], this->value_color[1], this->value_color[2]);
    else
      this->setLedColor(_row, _col, this->value_color[0], this->value_color[1], this->value_color[2]);
  }
  else
    this->setLedColorFast(_row, _col, 0, 0, 0);
}

void LedControl::setRow(int _row, byte _value) {
  byte val;

  if (_row < 0 || _row > 7)
    return;
  for (int _col = 0; _col < 8; _col++) {
    //val=value >> (7-col);
    val = _value >> (_col);
    val = val & 0x01;
    this->setLed(_row, _col, val);
  }
}

void LedControl::setColumn(int _col, byte _value) {
  byte val;

  if (_col < 0 || _col > 7)
    return;
  for (int _row = 0; _row < 8; _row++) {
    val = _value >> (7 - _row);
    //val = _value >> (_row);
    val = val & 0x01;
    this->setLed(_row, _col, val);
  }
}

void LedControl::writeString(int _time, char * _displayString) {
  int _leng = 0;
  int _wight = 0;
  while (_displayString[_leng] != NULL) _wight += 1 + int pgm_read_byte(alphabetBitmap[((int)_displayString[_leng++] - 32)]+FONE_SIZE_X);
  Serial.println(_wight);

  for (int a = 8; a > -_wight; a--) {
    int c = 0;
    for (int b = 0; b < _leng; b++) {
      this->displayChar(a + c, 0, _displayString[b]);
      c += 1 + int pgm_read_byte(alphabetBitmap[((int)_displayString[b] - 32)]+FONE_SIZE_X);
    }
    delay(_time);
    //    clearDisplay(addr);
  }
}

void LedControl::setCursor(int16_t _x, int16_t _y) {
  this->cursor_x = _x;
  this->cursor_y = _y;
}

size_t LedControl::write(uint8_t c) {
  if((int)c - 32 > 94 || (int)c - 32 < 0)
	return 0;
  if (c == '\n') {
//    this->cursor_y += 8;
//    this->cursor_x  = 0;
  } else if (c == '\r') {
    //   skip em
  } else {
    this->displayChar((this->cursor_x), (this->cursor_y), c);
    this->cursor_x += 1 + int pgm_read_byte(alphabetBitmap[((int)c - 32)]+FONE_SIZE_X);
    // if (cursor_x > (8 - k)) {
    // cursor_y += 8;
    // cursor_x = 0;
    // }
  }
  // displayChar(Devices_addr, cursor_x, cursor_y, c);
  return 1;
}

/* void LedControl::displayChar(int row, int col, char _charIndex) {
  if (row < 0-8 || row > 7+8)
    return;

  int n=((int)_charIndex - 32);
  int m = FONE_SIZE_X - alphabetBitmap[n][FONE_SIZE_X]; //i:1

  for (int i = m; i < FONE_SIZE_X; i++) { //2 3
    this->setRow(i - m + row, alphabetBitmap[n][i]);
  }
    this->setRow(FONE_SIZE_X - m + row, 0x00);
  } */

void LedControl::displayChar(int row, int col, char _charIndex) {
  if (row < 0 - 8 || row > 7 + 8)
    return;

  if (col < 0 - 8 || col > 7 + 8)
    return;

  if((int)_charIndex - 32 > 94 || (int)_charIndex - 32 < 0)
    return;

  int n = ((int)_charIndex - 32);
  int m = FONE_SIZE_X - int (pgm_read_byte(alphabetBitmap[n]+FONE_SIZE_X)); //i:1

  byte val;
  for (int i = m; i < FONE_SIZE_X+1; i++) { //2 3
    for (int _col = col; col < 0 ? _col < 8 + col : _col < 8; _col++) {
      if (i - m + row < 0 || i - m + row > 7)
        break;
      //val=value >> (7-col);
	  if(i!=FONE_SIZE_X)
		val = int(pgm_read_byte(alphabetBitmap[n]+i)) >> (_col - col);
	  else	  
		val = 0x00 >> (_col - col);	
      val = val & 0x01;
      this->setLed(i - m + row, _col, val);
    }
  }
/*   for (int _col = col; col < 0 ? _col < 8 + col : _col < 8; _col++) {
    if (FONE_SIZE_X - m + row < 0 || FONE_SIZE_X - m + row > 7)
      break;
    //val=value >> (7-col);
    val = 0x00 >> (_col - col);
    val = val & 0x01;
    this->setLed(FONE_SIZE_X - m + row, _col, val);
  } */
}

