#include <Microduino_Key.h>

DigitalKey keyA0(A0);
AnalogKey keyA2(A2);

void setup(){
  Serial.begin(9600);
  keyA0.begin();
  keyA2.begin();
}

void loop(){
	switch(keyA0.readVal()){
		case KEY_RELEASED:
		Serial.println("KEY A0(digital) RELEASED---1"); 	//按键释放状态
		break;
		case KEY_PRESSED:
		Serial.println("KEY A0(digital) PRESSED-------2"); 		//按键按下状态
		break;
		case KEY_PRESSING:
		Serial.println("KEY A0(digital) PRESSING--------3"); 	//按键按下动作
		break;
		case KEY_RELEASING:
		Serial.println("KEY A0(digital) RELEASING---------4");    //按键释放动作
		break;
	}

	switch(keyA2.readVal(0, 100)){
		case KEY_RELEASED:
		Serial.println("KEY A2(analog)  RELEASED---1"); 		//按键释放状态
		break;
		case KEY_PRESSED:
		Serial.println("KEY A2(analog)  PRESSED-------2"); 		//按键按下状态
		break;
		case KEY_PRESSING:
		Serial.println("KEY A2(analog)  PRESSING--------3"); 		//按键按下动作
		break;
		case KEY_RELEASING:
		Serial.println("KEY A2(analog)  RELEASING---------4");    //按键释放动作
		break;
	}

	delay(100);
}