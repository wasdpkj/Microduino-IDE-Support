#define DEBUG       0       //Serial monitor debugging ON/OFF.
#define PIN_KEYA    6       //Crash Sensor A pin.
#define PIN_KEYB    8       //Crash Sensor B pin.
#define PIN_BUZZER  10      //Buzzer pin.
#define PIN_LED     12      //ColorLED pin.
#define LED_NUM     2

//=================================DO IT YOURSELF: CHANGE THE CODE BELOW!========================================//
//Example: Change "#define BRIGHT_MAX 88" to "#define BRIGHT_MAX 180".                                           //
//         That means the LED will now shine brighter than before.                                               //
//                                                                                                               //
//IMPORTANT: Remember to upload the code into the mBattery again after changing the values!                      //
//===============================================================================================================//
#define BRIGHT_MAX   88    //Max LED brightness. Max brightness is 255. Minimum brightness is 0.

#define SCORE_MAX 5        //Victory condition. If games are going on for too long, you can lower this number. 
//For example, "#define SCORE_MAX 3" would mean you only have to be ahead by 3 points.

#define LED_INTERVAL 1000  //Speed at which the colors change. 1000 = 1 second, 2000 = 2 seconds, etc...
//Decrease this number for a greater challenge! Minimum value is 200.Ser
//For example, "#define LED_INTERVAL 500" would mean the players only have 0.5 seconds
//to react.
