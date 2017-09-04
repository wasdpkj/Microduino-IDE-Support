// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
// ==============

/*
*多线程例程
*
*本例程采用了3种方法定义了3个线程
	
	线程myTask1：每隔1秒钟打印"hello from task1"
	
	线程myTask2：每隔500毫秒切换LED灯的状态，同时打印出LED灯的状态
	
	线程Timer1： 每隔100ms查询串口输入内容，控制线程myTask1
	
注意：
    1.调用mySCoop.start()方法，启动多线程
	2.避免使用delay(), delayMicroseconds()方法，采用sleep()替换delay()
	
*/

#include <SCoop.h>
#define led1 6

/*
======= 创建线程1(myTaks1) ==========/
**线程创建方法
	defineTaskLoop(线程名)
	{
		线程循环内容
	}	
*/
defineTaskLoop(myTask1)
{
  Serial.println("hello from task1");
  sleep(1000);
}


/*
======= 创建线程2(myTaks2) ==========/
**线程创建方法
	defineTask(线程名)
	void 线程名::setup()
	{
		线程初始化内容
	}
	
	void 线程名::loop()  
	{
		线程循环内容
	}	
*/
defineTask(myTask2)
void myTask2::setup()
{
  trace("task2setup");
  pinMode(led1, OUTPUT);
}

void myTask2::loop()  {
  Serial.println("led1 HIGH");
  digitalWrite(led1, HIGH);
  sleepSync(500);
  Serial.println("led1 LOW");
  digitalWrite(led1, LOW);
  sleepSync(500);
}


/*
======= 创建定时线程(Timer1) ==========/
**定时线程创建方法
	defineTimerRun(定时线程名，定时周期)
	{
		定时工作内容
	}
		
*/
defineTimerRun(Timer1, 100)
{
  //根据串口输入内容，控制线程myTask1
  if (Serial.available())
  {
    char c = Serial.read();
    Serial.print(c);
    Serial.println(" key pressed");
    if (c == 'a')
      myTask1.pause();		//线程myTask1暂停
    if (c == 'b')
      myTask1.resume();		//线程myTask1恢复
  }
}



void setup() {
  Serial.begin(9600);
  while (!Serial);
  mySCoop.start();		//多线程启动
}
void loop()  {
  Serial.println("do whatever you want here also");
  sleep(500);		   //释放500ms
}
