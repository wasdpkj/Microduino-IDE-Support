#ifndef PKJ_h
#define PKJ_h
 
#include "Arduino.h"

 
class PKJ
{
  public:
    int VOLxianzhi(int,int,int);
    int VOLtiaobian(int,int,int);

//=========================
    bool Timer_Delay(unsigned int,unsigned long);
    void Timer_Delay_Begin();

  private:
    int _num;
    int _volL;
    int _volH;
    
//=========================
	#define _RUNmax 10

    unsigned int _RUNid;
    unsigned long _RUNdelay;
	unsigned long _RUNcache;
	unsigned long _RUNmillis[_RUNmax];
};

#endif