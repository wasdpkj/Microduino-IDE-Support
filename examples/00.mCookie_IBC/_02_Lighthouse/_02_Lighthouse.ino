/*
  =================================================================================                         
                             | What is: Lighthouse? |
  --------------------------------------------------------------------------------- 
  Use a Remote Controller to create your own lightshow! 
  You are in control of multiple modes: 
  A. Full Color + Breathing Effect
  B. Warm Colors + Breathing Effect
  C. Cool Colors + Breathing Effect
  D. Single Color + Solid Effect
  E. Single Color + Breathing Effect
  
  While in Mode D or E, you can use the left/right arrow keys to change the color:
  Red, Orange, Yellow, Green, Aqua, Blue, and Violet.
  
  =================================================================================
                                   | You will |
  ---------------------------------------------------------------------------------
  Use the Infrared Receiver and Remote Control together to control things from afar!  
  =================================================================================
                           | Change the code yourself! |
  ---------------------------------------------------------------------------------
  Click the userDef.h tab to change the below value to your own liking. :)
  
  BRIGHT_MAX: How bright the LEDs will shine.

  Remember to upload the code into the mBattery again after changing the value!
  =================================================================================
                              | Brainstorming Ideas |
  ---------------------------------------------------------------------------------
  1. What else can you use to control the Lighthouse instead of a Remote Controller? 
     A button? Light Sensor? Microphone sensor?
  2. Can you attach an LED to a Motor, turning it into a crazy, spinning lightshow?!
     You could maybe also shine it onto a disco ball!
  3. Can you build a Lighthouse design of your own using cardboard?
     You could make multiple holes for the LEDs to shine out of.
     
    Note: Some of the above may require significant modification of the code. 
  =================================================================================
                               | Port Connections |
  ---------------------------------------------------------------------------------
  Infrared Receiver: D4
  ColorLED：D12
  =================================================================================
                                  | Questions? |
  ---------------------------------------------------------------------------------
  Visit us at http://www.microduino.cc/ and click the chatbox in the lower right to submit
  a question. We'll get back to you as soon as possible.
*/
#include "IRControl.h"
#include "colorLed.h"
#include "userDef.h"


void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);
  irrecv.enableIRIn();//Initialize the Infrared Receiver and begin receiving data.
#if DEBUG
  Serial.println("**************START************");
#endif
  //Shine 10 colors for 0.5 seconds each.
  for (int i = 0; i < 10; i++)
  {
    setAllLed(i);
    delay(500);//每个颜色持续时间
  }
}

void loop() {
  if (irrecv.decode(&results)) //If there is an available Infrared signal, then:
  {
#if DEBUG
    Serial.print("Recv Code:");
    Serial.println(results.value, HEX);
#endif
    recvParse(results.value);
    //irrecv.enableIRIn();
    irrecv.resume(); //Receive next value.
  }

  switch (mode)
  {
    case MODE_OFF:
      setAllLed(LOW);
      break;
    case MODE_A:
      ledRainbow(20);       //Rainbow effect, 20 ms for every color.
      break;
    case MODE_B:
      ledBreath(COLOR_WARM, 10);
      break;
    case MODE_C:
      ledBreath(COLOR_COLD, 10);
      break;
    case MODE_D:
      setAllLed(colorNum);
      break;
    case MODE_E:
      ledBreath(Color[colorNum], 10);
      break;
    default:
      break;
  }
  delay(20);
}
