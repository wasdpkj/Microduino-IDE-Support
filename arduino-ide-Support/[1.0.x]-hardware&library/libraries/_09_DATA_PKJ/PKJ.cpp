#include "Arduino.h"
#include "PKJ.h"
 
int PKJ::VOLxianzhi(int _num,int _volL,int _volH)
{
	if (_num>_volH)
	_num=_volH;
	if(_num<_volL)
	_num=_volL;
	return _num;
}

int PKJ::VOLtiaobian(int _num,int _volL,int _volH)
{
	if (_num>_volH)
	_num=_volL;
	if(_num<_volL)
	_num=_volH;
	return _num;
}

void PKJ::Timer_Delay_Begin()
{
  for(int a=0;a<_RUNmax;a++)
    _RUNmillis[a]=millis();
}

bool PKJ::Timer_Delay(unsigned int _RUNid,  unsigned long _RUNDelay)
{	
	_RUNcache=millis();
	if(_RUNcache<_RUNmillis[_RUNid]) _RUNmillis[_RUNid]=_RUNcache;
    if(_RUNcache-_RUNmillis[_RUNid]>=_RUNDelay)
    {
      _RUNmillis[_RUNid]=_RUNcache;
      return true;
    }
    else
      return false;
}
