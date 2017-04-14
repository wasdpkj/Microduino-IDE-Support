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
uint8_t colorNum = 0;

//------------红外接收功能-------------//
uint8_t recvRemote(uint32_t recvCode)
{
  uint8_t cmd = CMD_OK;
  switch (recvCode) {
    case BUTTON_A:
      motorCoast();
      setAllLed(COLOR_RED);
      tone(PIN_BUZZER, TONE_DO);
      delay(300);
      setAllLed(COLOR_NONE);
      noTone(PIN_BUZZER);
      mode = MODE_A;
      break;
    case BUTTON_B:
      motorCoast();
      setAllLed(COLOR_RED);
      tone(PIN_BUZZER, TONE_DO);
      delay(300);
      setAllLed(COLOR_GREEN);
      tone(PIN_BUZZER, TONE_RE);
      delay(300);
      setAllLed(COLOR_NONE);
      noTone(PIN_BUZZER);
      mode = MODE_B;
      break;
    case BUTTON_C:
      motorCoast();
      setAllLed(COLOR_RED);
      tone(PIN_BUZZER, TONE_DO);
      delay(300);
      setAllLed(COLOR_GREEN);
      tone(PIN_BUZZER, TONE_RE);
      delay(300);
      setAllLed(COLOR_BLUE);
      tone(PIN_BUZZER, TONE_MI);
      delay(300);
      setAllLed(COLOR_NONE);
      noTone(PIN_BUZZER);
      mode = MODE_C;
      break;
    case BUTTON_POWER:
      tone(PIN_BUZZER, 523);
      setAllLed(COLOR_RED);
      delay(100);
      noTone(PIN_BUZZER);
      setAllLed(COLOR_NONE);
      delay(100);
      tone(PIN_BUZZER, 523);
      setAllLed(COLOR_RED);
      delay(100);
      noTone(PIN_BUZZER);
      setAllLed(COLOR_NONE);
      delay(100);
      tone(PIN_BUZZER, 523);
      setAllLed(COLOR_RED);
      delay(100);
      noTone(PIN_BUZZER);
      setAllLed(COLOR_NONE);
      delay(100);
      mode = MODE_OFF;
      break;
    case BUTTON_UP:
      cmd = CMD_UP;
      break;
    case BUTTON_DOWN:
      cmd = CMD_DOWN;
      break;
    case BUTTON_LEFT:
      cmd = CMD_LEFT;
      break;
    case BUTTON_RIGHT:
      cmd = CMD_RIGHT;
      break;
    case BUTTON_OK:
      cmd = CMD_OK;
      break;
    default:
      break;
  }
  return cmd;
}
