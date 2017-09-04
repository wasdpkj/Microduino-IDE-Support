#define DEBUG        0       //Serial monitor debugging ON/OFF.
#define PIN_KEY      6       //Crash Sensor pin.
#define PIN_BUZZER   10      //Buzzer pin.
#define PIN_LED      12      //ColorLED pin.
#define PIN_LINEA    A0      //Line Finder A pin.
#define PIN_LINEB    A2      //Line Finder B pin.
#define LED_NUM      2

//=================================DO IT YOURSELF: CHANGE THE CODE BELOW!========================================//
//Example: Change "#define BRIGHT_MAX 128" to "#define BRIGHT_MAX 180".                                          //
//         That means the LED will now shine brighter than before.                                               //
//                                                                                                               //
//IMPORTANT: Remember to upload the code into the mBattery again after changing the values!                      //
//===============================================================================================================//
#define BRIGHT_MAX   128     //Max LED brightness. Max brightness is 255. Minimum brightness is 0.

#define DOCK_MAX     500     //Line Finder minimum activation value. Increasing this means you will have to use a
//darker-colored ball/item to activate the Line Finder.

#define SCORE_MAX    5       //Number of points you have to be ahead by to win! The default is 5 points, meaning 
//there will only be a winner if that person is beating the other person by 5 points.
//Lower this number to have shorter games.

#define leftColor   COLOR_GREEN     //左灯颜色
#define rightColor   COLOR_PURPLE    //右灯颜色
