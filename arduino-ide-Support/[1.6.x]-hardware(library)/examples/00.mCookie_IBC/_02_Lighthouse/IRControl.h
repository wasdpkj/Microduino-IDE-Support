#include <IRremote.h>//Import the library for the Infrared Receiver.
#include "userDef.h"

#define  BUTTON_POWER   0x1FE48B7
#define  BUTTON_A       0x1FE807F
#define  BUTTON_B       0x1FE40BF
#define  BUTTON_C       0x1FEC03F
#define  BUTTON_D       0x1FE20DF
#define  BUTTON_E       0x1FE609F
#define  BUTTON_LEFT    0x1FEE01F
#define  BUTTON_RIGHT   0x1FE906F
#define  BUTTON_UP      0x1FEA05F
#define  BUTTON_DOWN    0x1FED827
#define  BUTTON_OK      0x1FE10EF
#define  BUTTON_BACK    0x1FE50AF
#define  BUTTON_VOLUP   0x1FEF807
#define  BUTTON_VOLDOWN 0x1FE708F
#define  BUTTON_MUTE    0x1FEB04F
#define  BUTTON_PLAY    0x1FE30CF



#define  MODE_OFF     0
#define  MODE_A       1
#define  MODE_B       2
#define  MODE_C       3
#define  MODE_D       4
#define  MODE_E       5

IRrecv irrecv(PIN_RECV);
decode_results results;

uint8_t mode = MODE_OFF;
uint8_t colorNum = 1;


//------------Receiving Infrared Signals from Remote Control-------------//
void recvParse(uint32_t recvCode)
{
  switch (recvCode)
  {
    case BUTTON_A:
      mode = MODE_A;
      break;
    case BUTTON_B:
      mode = MODE_B;
      break;
    case BUTTON_C:
      mode = MODE_C;
      break;
    case BUTTON_D:
      mode = MODE_D;
      colorNum = 3;
      break;
    case BUTTON_E:
      mode = MODE_E;
      colorNum = 3;
      break;
    case BUTTON_LEFT:
      colorNum--;
      if (colorNum < 3)
        colorNum = 9;
      break;
    case BUTTON_RIGHT:
      colorNum++;
      if (colorNum > 9)
        colorNum = 3;
      break;
    case BUTTON_POWER:
      mode = MODE_OFF;
      break;
    default:
      break;
  }
}
