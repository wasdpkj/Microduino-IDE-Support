// I2Cdev library collection - BMP085 basic Arduino example sketch
// Based on register information stored in the I2Cdevlib internal database

//
// Changelog:
#include <BMP085.h>

// class default I2C address is 0x77
// specific I2C addresses may be passed as a parameter here
// (though the BMP085 supports only one address)
BMP085 barometer;

float temperature;
float pressure;
float altitude;

void setup() {
    Serial.begin(115200);

    // initialize device
    Serial.println("Initializing I2C devices...");
	
	//set BM085 mode,
    uint8_t bmpMode = BMP085_MODE_PRESSURE_3;       //BMP085_MODE_TEMPERATURE/BMP085_MODE_PRESSURE_0/BMP085_MODE_PRESSURE_1/BMP085_MODE_PRESSURE_2/BMP085_MODE_PRESSURE_3 

    Serial.println(barometer.begin(bmpMode) ? "BMP085 connection successful" : "BMP085 connection failed");
}

void loop() {
    // wait for available 
    while(!barometer.available());
 
    // read calibrated temperature value in degrees Celsius   
    temperature = barometer.getTemperature();

    // read calibrated pressure value in Pascals (Pa)
    pressure = barometer.getPressure();

    // calculate absolute altitude in meters based on known pressure
    // (may pass a second "sea level pressure" parameter here,
    // otherwise uses the standard value of 101325 Pa)
    altitude = barometer.getAltitude();

    // display measured values if appropriate
    Serial.print("T/P/A\t");
    Serial.print(temperature); Serial.print("\t");
    Serial.print(pressure); Serial.print("\t");
    Serial.print(altitude);
    Serial.println("");
    
    // delay 100 msec to allow visually parsing blink and any serial output
    delay(100);
}