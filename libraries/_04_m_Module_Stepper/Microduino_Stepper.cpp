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

#include <Microduino_Stepper.h>

static Stepper_t steppers[MAX_STEPPERS];

uint8_t StepperCount = 0;

// Some debugging assistance

void stepperAllEnable(){
	digitalWrite(PIN_EN, LOW);
}

void stepperAllDisable(){
	digitalWrite(PIN_EN, HIGH);
}


static inline void handle_interrupts(){
	for(uint8_t channel=0; channel < MAX_STEPPERS; channel++){
		if(steppers[channel].isActive)
			steppers[channel].stepper->computeStep();
	}
}

static void initISR(){
#if defined (__AVR__)
#if defined(_useTimer1)
    cli();
	TCCR1A = 0;
	TCCR1B = _BV(WGM12)|_BV(CS11);
	OCR1A = TIMER_COMP;
	TCNT1 = 0;
	TIMSK1 = _BV(OCIE1A);
	sei();  
#endif
#elif defined (ESP32)
	hw_timer_t *timer;
	timer = timerBegin(1, 80, 1); 
	timerAttachInterrupt(timer, &handle_interrupts, 1); 
	// every 50ns, autoreload = true 
	timerAlarmWrite(timer, TIMER_COMP, true); 
	timerAlarmEnable(timer);  
#endif
	pinMode(PIN_EN, OUTPUT);
	stepperAllEnable();
}

#if defined (__AVR__)
#if defined(_useTimer1)
ISR(TIMER1_COMPA_vect){
	
	handle_interrupts();
}
#endif
#endif


static bool isTimerActive(){
	for(uint8_t channel=0; channel <MAX_STEPPERS ; channel++){
		if(steppers[channel].isActive == true)
			return  true;
	}
	return false;
}


/****************** end of static functions ******************************/

/************************ Stepper ******************************/

Stepper::Stepper(uint8_t _dirPin, uint8_t _stepPin){
	if(StepperCount < MAX_STEPPERS){
		this->stepperIndex = StepperCount++;
	}else{
		this->stepperIndex = INVALID_STEPPER;
	}
	dirPin = _dirPin;
	stepPin = _stepPin;
	speed = 0;
	period = 0;
	counter = 0;
}  


uint8_t Stepper::begin(uint16_t _maxSpeed){
	if(this->stepperIndex < MAX_STEPPERS){
		pinMode(dirPin, OUTPUT);
		pinMode(stepPin, OUTPUT);
		setMaxAccel(DEFAULT_ACCEL);
		setMaxSpeed(_maxSpeed);
		if(isTimerActive() == false)
			initISR();
		steppers[this->stepperIndex].isActive = true;
		steppers[this->stepperIndex].stepper = this;
	}
	return this->stepperIndex;
}
  
 
bool Stepper::setSpeed(int16_t _speed){	
	speed += constrain((_speed-speed), -(int16_t)maxAccel, (int16_t)maxAccel);
    if(speed == 0)
		period = 0;
	else
		period = maxSpeed/abs(speed);
	(speed>0) ? PIN_CLR(dirPin) : PIN_SET(dirPin);
	return (speed == _speed);
}
 
 
void Stepper::setMaxAccel(uint16_t _accel){
	maxAccel = _accel;
}

void Stepper::setMaxSpeed(uint16_t _maxSpeed){
	maxSpeed = _maxSpeed;
}

int16_t Stepper::getSpeed() { return  speed; }
uint16_t Stepper::getMaxAccel() { return  maxAccel;}
uint16_t Stepper::getMaxSpeed() { return  maxSpeed;}


void Stepper::computeStep(){
	counter++;
	if(counter > period){
		counter = 0;
		if(period > 0){
			PIN_SET(stepPin);
			delayMicroseconds(1);
			PIN_CLR(stepPin);
		}
	}
}

/************************ StepServo ******************************/

StepServo::StepServo(uint8_t _dirPin, uint8_t _stepPin)
{
	if(StepperCount < MAX_STEPPERS){
		this->stepperIndex = StepperCount++;
	}else{
		this->stepperIndex = INVALID_STEPPER;
	}
	
	dirPin = _dirPin;
	stepPin = _stepPin;
    direction = DIRECTION_CCW;
}


uint8_t StepServo::begin(){
	if(this->stepperIndex < MAX_STEPPERS){
		pinMode(dirPin, OUTPUT);
		pinMode(stepPin, OUTPUT);
		setMaxSpeed(SERVO_MAX_SPEED);
		pinMode(PIN_EN, OUTPUT);
		stepperAllEnable();
	}
	return this->stepperIndex;
}

void StepServo::setMaxSpeed(float _speed){
    if (maxSpeed != _speed){
		maxSpeed = _speed;
		cmin = 1000000.0 / _speed;
    }
}

void StepServo::setAcceleration(float acceleration){
    if (acceleration == 0.0)
		return;
    if (accel != acceleration){
		// Recompute _n per Equation 17
		n = n * (accel / acceleration);
		// New c0 per Equation 7
		c0 = sqrt(2.0 / acceleration) * 1000000.0;
		accel = acceleration;
		computeNewSpeed();
    }
}

float StepServo::getAcceleration(){
    return accel;
}

void StepServo::setSpeed(float _speed)
{
    if (_speed == speed)
        return;
    _speed = constrain(_speed, -maxSpeed, maxSpeed);
    if (_speed == 0.0)
		stepInterval = 0;
    else{
		stepInterval = fabs(1000000.0 / _speed);
		direction = (_speed > 0.0) ? DIRECTION_CW : DIRECTION_CCW;
    }
    speed = _speed;
}


float StepServo::getSpeed(){
    return speed;
}


bool StepServo::runSpeedToPosition(){
    if (targetPos == currentPos)
		return false;
    if (targetPos >currentPos)
		direction = DIRECTION_CW;
    else
		direction = DIRECTION_CCW;
    return runSpeed();
}

void StepServo::setCurrentPosition(int32_t position){
    targetPos = currentPos = position;
    n = 0;
    stepInterval = 0;
}

int32_t StepServo::getCurrentPosition(){
    return currentPos;
}

int32_t StepServo::getTargetPosition(){
    return targetPos;
}

int32_t StepServo::distanceToGo(){
    return targetPos - currentPos;
}
  
void StepServo::moveTo(int32_t _absolute){
    if (targetPos != _absolute){
		targetPos = _absolute;
		computeNewSpeed();	// compute new n?
    }
}

void StepServo::move(int32_t _relative){
    moveTo(currentPos + _relative);
}

// Blocks until the target position is reached and stopped
void StepServo::runToPosition(){
    while (speed != 0 || distanceToGo() != 0)
		run();
}

// Blocks until the new target position is reached
void StepServo::runToNewPosition(int32_t position){
    moveTo(position);
    runToPosition();
}


void StepServo::runDistance(int32_t _relative){
    move(_relative);
    runToPosition();
}

void StepServo::stop(){
    if (speed != 0.0){    
		int32_t stepsToStop = (int32_t)((speed * speed) / (2.0 * accel)) + 1; // Equation 16 (+integer rounding)
	if (speed > 0)
	    move(stepsToStop);
	else
	    move(-stepsToStop);
    }
}

// Run the motor to implement speed and acceleration in order to proceed to the target position
// You must call this at least once per step, preferably in your main loop
// If the motor is in the desired position, the cost is very small
// returns true if we are still running to position
void StepServo::run(){
    if (runSpeed())
		computeNewSpeed();
}

void StepServo::computeNewSpeed(){
    int32_t distanceTo = distanceToGo(); // +ve is clockwise from curent location
    int32_t stepsToStop = (int32_t)((speed * speed) / (2.0 * accel)); // Equation 16

    if(distanceTo == 0 && stepsToStop <= 1){
		// We are at the target and its time to stop
		stepInterval = 0;
		speed = 0.0;
		n = 0;
		return;
    }

    if(distanceTo > 0){
	// We are anticlockwise from the target
	// Need to go clockwise from here, maybe decelerate now
		if(n > 0){
			// Currently accelerating, need to decel now? Or maybe going the wrong way?
			if((stepsToStop >= distanceTo) || direction == DIRECTION_CCW)
				n = -stepsToStop; // Start deceleration
		}else if(n < 0){
			// Currently decelerating, need to accel again?
			if ((stepsToStop < distanceTo) && direction == DIRECTION_CW)
				n = -n; // Start accceleration
		}
    }else if (distanceTo < 0){
	// We are clockwise from the target
	// Need to go anticlockwise from here, maybe decelerate
		if (n > 0){
			// Currently accelerating, need to decel now? Or maybe going the wrong way?
			if((stepsToStop >= -distanceTo) || direction == DIRECTION_CW)
				n = -stepsToStop; // Start deceleration
		}else if(n < 0){
			// Currently decelerating, need to accel again?
			if((stepsToStop < -distanceTo) && direction == DIRECTION_CCW)
				n = -n; // Start accceleration
		}
    }

    // Need to accelerate or decelerate
    if (n == 0){
	// First step from stopped
		cn = c0;
		direction = (distanceTo > 0) ? DIRECTION_CW : DIRECTION_CCW;
    }else{
		// Subsequent step. Works for accel (n is +_ve) and decel (n is -ve).
		cn = cn - ((2.0 * cn) / ((4.0 * n) + 1)); // Equation 13
		cn = max(cn, cmin); 
    }
    n++;
    stepInterval = cn;
    speed = 1000000.0 / cn;
    if (direction == DIRECTION_CCW)
		speed = -speed;
}

// Implements steps according to the current step interval
// You must call this at least once per step
// returns true if a step occurred
bool StepServo::runSpeed(){
    // Dont do anything unless we actually have a step interval
    if (!stepInterval)
		return false;

    uint32_t timeNow = micros();
    // Gymnastics to detect wrapping of either the nextStepTime and/or the current time
    uint32_t nextStepTime = lastStepTime + stepInterval;
    if (((nextStepTime >= lastStepTime) && ((timeNow >= nextStepTime) || (timeNow < lastStepTime)))
	|| ((nextStepTime < lastStepTime) && ((timeNow >= nextStepTime) && (timeNow < lastStepTime)))){
		if (direction == DIRECTION_CW){
	    // Clockwise
			currentPos += 1;
		}else{
	    // Anticlockwise  
			currentPos -= 1;
		}
		step();
		lastStepTime = timeNow;
		return true;
    }else{
		return false;
    }
}

// Subclasses can override
void StepServo::step(){
	direction ? PIN_CLR(dirPin) : PIN_SET(dirPin);
	PIN_SET(stepPin);
	delayMicroseconds(1);
	PIN_CLR(stepPin);
}
