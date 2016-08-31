#define DEBUG       0       //Serial monitor debugging ON/OFF.
#define PIN_KEY     6       //Crash Sensor pin.
#define PIN_LED     12      //Buzzer pin.
#define PIN_LIGHT   A2      //Light Sensor pin.
#define PIN_MIC     A6      //Microphone Sensor pin.
#define VOICE_MAX   1023    //Max noise value.
#define LED_NUM     2       //Number of LEDs.
#define LIGHT_MIN   0       

//=================================DO IT YOURSELF: CHANGE THE CODE BELOW!========================================//
//Example: Change "#define BRIGHT_MAX 128" to "#define BRIGHT_MAX 180".                                          //
//         That means the LED will now shine brighter than before.                                               //
//                                                                                                               //
//IMPORTANT: Remember to upload the code into the mBattery again after changing the values!                      //
//===============================================================================================================//
#define BRIGHT_MAX  128     //Max LED brightness. Max brightness is 255. Minimum brightness is 0.

#define VOICE_MIN   400     //Minimum noise level to activate the Nightlight. Increasing this means you
                            //will have to be louder to activate the Nightlight.

#define LIGHT_MAX   200     //Lower this value if you want to activate the Nightlight when it's still bright outside.

#define TIME_ON     10 * 1000   //How long the LED will remain on once it lights up. Changing the number "10" to "20" 
                                //means it will remain on for 20 seconds. Do not change the "1000".
