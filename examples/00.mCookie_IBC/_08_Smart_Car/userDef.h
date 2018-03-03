#define DEBUG        0       //Serial monitor debugging ON/OFF.
#define PIN_RECV     4       //Infrared Receiver pin.
#define PIN_BUZZER   10      //Buzzer pin.
#define PIN_LED      12      //ColorLED pin.
#define PIN_LINEA    A0      //Line Finder A pin.
#define PIN_LINEB    A2      //Line Finder B pin.
#define PIN_MIC      A6      //Microphone Sensor pin.
#define LED_NUM      2
#define DOCK_MAX     500
#define TRACK_THRESHOLD  500

//=================================DO IT YOURSELF: CHANGE THE CODE BELOW!========================================//
//Example: Change "#define BRIGHT_MAX 128" to "#define BRIGHT_MAX 180".                                          //
//         That means the LED will now shine brighter than before.                                               //
//                                                                                                               //
//IMPORTANT: Remember to upload the code into the mBattery again after changing the values!                      //
//===============================================================================================================//
#define BRIGHT_MAX    128   //Max LED brightness. Max brightness is 255. Minimum brightness is 0.

#define MAX_THROTTLE  100   //How quickly the Smart Car moves. Fastest speed is 255. A negative value will make
//the Motor spin in the other direction. Note: 255 is very fast. Be careful!

#define STEER          40   //How quickly the Smart Car turns. Max value is 255. 

#define tackforward 40
#define tacksteer 30
