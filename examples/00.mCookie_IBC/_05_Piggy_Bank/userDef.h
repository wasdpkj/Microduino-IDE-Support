#define DEBUG       1       //Serial monitor debugging ON/OFF.
#define PIN_BUZZER  10      //Buzzer Sensor pin.
#define PIN_LED     12      //ColorLED pin.
#define PIN_LINEA   A0      //Line Finder pin.
#define LED_NUM     2

//=================================DO IT YOURSELF: CHANGE THE CODE BELOW!========================================//
//Example: Change "#define BRIGHT_MAX 128" to "#define BRIGHT_MAX 180".                                          //
//         That means the LED will now shine brighter than before.                                               //
//                                                                                                               //
//IMPORTANT: Remember to upload the code into the mBattery again after changing the values!                      //
//===============================================================================================================//
#define BRIGHT_MAX  128     //Max LED brightness. Max brightness is 255. Minimum brightness is 0.

#define LINE_MAX    600     //Line Finder minimum activation value. Increasing this means you will have to use a
//darker-colored coin/item to activate the Line Finder.
