#define DEBUG       0       //Serial monitor debugging ON/OFF.
#define PIN_LED     12      //ColorLED pin.
#define PIN_MIC     A6      //MIC Sensor pin.
#define VOICE_MAX   1023    //Maximum noise level to activate the windmill and LED.
#define LED_NUM     2

//=================================DO IT YOURSELF: CHANGE THE CODE BELOW!========================================//
//Example: Change "#define BRIGHT_MAX 128" to "#define BRIGHT_MAX 180".                                          //
//         That means the LED will now shine brighter than before.                                               //
//                                                                                                               //
//IMPORTANT: Remember to upload the code into the mBattery again after changing the values!                      //
//===============================================================================================================//
#define VOICE_MIN 400           //Minimum noise level to activate the windmill and LED. Increasing this means you
//will have to be louder to activate the windmill.

#define MOTOR_SPEED_MAX 100     //How fast the windmill spins. Fastest speed is 255. A negative value will make
//the Motor spin in the other direction. Note: 255 is very fast. Be careful!

#define TIME_RUN  10*1000       //How long the windmill and LED will be on. Changing the number "10" to "20" means
//it will spin for 20 seconds. Do not change the "1000".

#define BRIGHT_MAX  128         //Max LED brightness. Max brightness is 255. Minimum brightness is 0. 

