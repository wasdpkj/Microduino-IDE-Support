#include <SCoop.h>
#define led1 6

defineTaskLoop(myTask1)
{
  Serial.println("hello from task1");
  sleep(1000);
}

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

defineTimerRun(Timer1, 100)
{
  if (Serial.available())
  {
    char c = Serial.read();
    Serial.print(c);
    Serial.println(" key pressed");
    if (c == 'a')
      myTask1.pause();
    if (c == 'b')
      myTask1.resume();
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  mySCoop.start();
}
void loop()  {
  Serial.println("do whatever you want here also");
  sleep(500);
}
