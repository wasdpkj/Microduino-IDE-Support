// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

// 版权所有：
// @Microduino_sy  shenyang@microduino.com
// ==============

// Microduino-IDE
// ==============
// Microduino Getting start:
// http://www.microduino.cc/download/

// ==============
// Microduino wiki:
// http://wiki.microduino.cc

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <Microduino_PID.h>

/*Constructor (...)*********************************************************
 *    The parameters specified here are those for for which we can't set up 
 *    reliable defaults, so we need to have the user set them.
 ***************************************************************************/
PID::PID(double _kp, double _ki, double _kd)		
{	
	PID::setOutputLimits(-255, 255);				//default output limit corresponds to 
    PID::setItermLimits(-255, 255);	
	//the arduino pwm limits

    sampleTime = 1000;							//default Controller Sample Time is 0.1 seconds

    PID::setTunings(_kp, _ki, _kd);

	iterm = 0;
	prevBias = 0;
	lastBias = 0;
    lastTime = micros() - sampleTime;				
}
 
 
/* Compute() **********************************************************************
 *     This, as they say, is where the magic happens.  this function should be called
 *   every time "void loop()" executes.  the function will decide for itself whether a new
 *   pid Output needs to be computed.  returns true when the output is computed,
 *   false when nothing has been done.
 **********************************************************************************/ 
double PID::PositionCompute(double _input, double _setpoint)
{
   double output = 0;
   uint32_t dt;
   uint32_t now = micros();
   if(now < lastTime)
	  dt = (now + 0xFFFFFFFF - lastTime);
   else
      dt = (now - lastTime);
   
   if(dt >= sampleTime)
   {
      /*Compute all the working error variables*/
      float bias = _setpoint - _input;
      iterm += bias;
	  iterm = constrain(iterm, itermMin, itermMax);
 
      /*Compute PID Output*/
      output = kp * bias + ki * iterm + kd * (bias - lastBias);
      
	  output = constrain(output, outMin, outMax);
	  
      /*Remember some variables for next time*/
      lastBias = bias;
      lastTime = now;
   }
   return output;
}


double PID::IncrementCompute(double _input, double _setpoint)
{
   double output = 0;
   uint32_t dt;
   uint32_t now = micros();
   if(now < lastTime)
	  dt = (now + 0xFFFFFFFF - lastTime);
   else
      dt = (now - lastTime);
   
   if(dt >= sampleTime)
   {
      /*Compute all the working error variables*/
      float bias = _setpoint - _input;
 
      /*Compute PID Output*/
      output = kp * (bias - lastBias) + ki * bias + kd * (bias - 2*lastBias + prevBias);
      
	  output = constrain(output, outMin, outMax);
	  
      /*Remember some variables for next time*/
      prevBias = lastBias;
	  lastBias = bias;
      lastTime = now;
   }
   return output;
}

/* SetTunings(...)*************************************************************
 * This function allows the controller's dynamic performance to be adjusted. 
 * it's called automatically from the constructor, but tunings can also
 * be adjusted on the fly during normal operation
 ******************************************************************************/ 
void PID::setTunings(double _kp, double _ki, double _kd)
{
   if (_kp<0 || _ki<0 || _kd<0) return;
   
   double sampleTimeInSec = ((double)sampleTime)/1000000;
   kp = _kp;
   ki = _ki * sampleTimeInSec;
   kd = _kd / sampleTimeInSec;

}
  
/* SetSampleTime(...) *********************************************************
 * sets the period, in Milliseconds, at which the calculation is performed	
 ******************************************************************************/
void PID::setSampleTime(uint16_t _newSampleTime)
{
   if (_newSampleTime > 0)
   {
      double ratio  = (double)_newSampleTime
                      / (double)sampleTime;
      ki *= ratio;
      kd /= ratio;
      sampleTime = (uint32_t)_newSampleTime;
   }
}
 
/* SetOutputLimits(...)****************************************************
 *     This function will be used far more often than SetInputLimits.  while
 *  the input to the controller will generally be in the 0-1023 range (which is
 *  the default already,)  the output will be a little different.  maybe they'll
 *  be doing a time window and will need 0-8000 or something.  or maybe they'll
 *  want to clamp it from 0-125.  who knows.  at any rate, that can all be done
 *  here.
 **************************************************************************/
void PID::setOutputLimits(double _min, double _max)
{
   if(_min >= _max) return;
   outMin = _min;
   outMax = _max;
}


void PID::setItermLimits(double _min, double _max)
{
   if(_min >= _max) return;
   itermMin = _min;
   itermMax = _max;
}


/* Status Funcions*************************************************************
 * Just because you set the Kp=-1 doesn't mean it actually happened.  these
 * functions query the internal state of the PID.  they're here for display 
 * purposes.  this are the functions the PID Front-end uses for example
 ******************************************************************************/
double PID::getKp(){ return  kp;}
double PID::getKi(){ return  ki;}
double PID::getKd(){ return  kd;}

