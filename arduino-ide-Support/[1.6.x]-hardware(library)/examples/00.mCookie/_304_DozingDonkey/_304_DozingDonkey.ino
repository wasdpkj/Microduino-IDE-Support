#define motor_pinB 6
#define motor_pinA 8

//#define servo_pinB 5
//#define servo_pinA 7

const int vibrationPin = 10;     // the number of the pushbutton pin

int vibrationState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(vibrationPin, INPUT);

  pinMode(motor_pinB, OUTPUT);
  pinMode(motor_pinA, OUTPUT);
}

void loop() {

  vibrationState = digitalRead(vibrationPin);
  if (vibrationState == HIGH) {
    Serial.println("ONONONON");
    fullForward();
  } else {
    stop();
  }
}

//stop
void stop() {
  digitalWrite(motor_pinB, LOW);
  digitalWrite(motor_pinA, LOW);
  //delay(2000);
}

//full forward
void fullForward() {
  //digitalWrite(motor_pinB, HIGH);
  analogWrite(motor_pinB,255);//0-255
  digitalWrite(motor_pinA, LOW);
  delay(1000);
}
