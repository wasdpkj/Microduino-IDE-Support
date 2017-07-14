#ifndef _MICRODUINO_PID_H_
#define _MICRODUINO_PID_H_

#define LIBRARY_VERSION	2.1.1


class PID
{
  public:

  //commonly used functions **************************************************************************
    PID(double, double, double);     //   Setpoint.  Initial tuning parameters are also set here
	
    double PositionCompute(double, double);          // * performs the PID calculation.  it should be
                                          //   called every time loop() cycles. ON/OFF and
                                          //   calculation frequency can be set using SetMode
                                          //   SetSampleTime respectively
	double IncrementCompute(double, double);
										  
    void setTunings(double, double, double);         	  //   constructor, this function gives the user the option
                                          //   of changing tunings during runtime for Adaptive control

    void setSampleTime(uint16_t);              // * sets the frequency, in Milliseconds, with which 
                                          //   the PID calculation is performed.  default is 100
										  
    void setOutputLimits(double, double); //clamps the output to a specific range. 0-255 by default, but
										  //it's likely the user will want to change this depending on
										  //the application
    void setItermLimits(double, double);
										  										  
  //Display functions ****************************************************************
	double getKp();						  // These functions query the pid for interal values.
	double getKi();						  //  they were created mainly for the pid front-end,
	double getKd();						  // where it's important to know what is actually 			  //

  private:

	double kp;                  // * (P)roportional Tuning Parameter
    double ki;                  // * (I)ntegral Tuning Parameter
    double kd;                  // * (D)erivative Tuning Parameter

 	uint32_t sampleTime;
	uint32_t lastTime;
	double iterm;
	double prevBias;
	double lastBias;

	double outMin, outMax;
    double itermMin, itermMax;
};
#endif

