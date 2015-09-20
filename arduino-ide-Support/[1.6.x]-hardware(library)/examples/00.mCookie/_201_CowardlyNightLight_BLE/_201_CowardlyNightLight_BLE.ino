#include <Adafruit_NeoPixel.h>

#include <SoftwareSerial.h>

//#define my_Serial mySerial
#define my_Serial Serial1

#define PIXEL_PIN    6    // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT  2

#define lightSensorPin  A0
#define lightSwitch  700

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

String currentInfo;

char buffer[100];

int sensorValue=0;

boolean buffer_sta = false;

int buffer_num = 0;

int sta[4];

boolean color_en;

long safe_ms = millis();

#define val_max 255
#define val_min 0

void setup() {

  // put your setup code here, to run once:
  Serial.begin(9600);
  my_Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  pinMode(lightSensorPin, INPUT);

}

void loop() {

  sensorValue = analogRead(lightSensorPin); // read the value of sensor 1
  
  if(sensorValue>lightSwitch) {
    Serial.println(sensorValue);
    strip.setPixelColor(0, strip.Color(0, 0, 0));
    strip.setPixelColor(1, strip.Color(0, 0, 0));
    strip.show();
  } else {
      ble();
      if (!color_en) {
        for (int i = 0; i < 1; i++)
          rainbow(10);
      }
  }
}

void ble()
{
  while (my_Serial.available())
  {
    char c = my_Serial.read();
    delay(2);

    if (c == 'C')
      buffer_sta = true;
    if (c == '\n')
    {
      color_en = true;
      safe_ms = millis();
    }
    if (buffer_sta)
    {
      buffer[buffer_num] = c;
      buffer_num++;
    }
    //  Serial.println(c);
    //Serial.println(color_en);
  }

  if (buffer_sta)
  {
    buffer_sta = false;

    sscanf((char *)strstr((char *)buffer, "C:"), "C:%d,%d,%d,%d", &sta[0], &sta[1], &sta[2], &sta[3]);

    for (int a = 0; a < buffer_num; a++)
      buffer[a] = NULL;
    buffer_num = 0;

    for (int i = 0; i < 4; i++)
    {
      Serial.print(sta[i]);
      Serial.print(",");
    }
    Serial.println(" ");

    if (-1 == sta[3]) {
      colorSet(strip.Color(sta[0], sta[1], sta[2]));
    }
    else if ((0 <= sta[3]) && (sta[3] < PIXEL_COUNT)) {
      colorSet(strip.Color(sta[0], sta[1], sta[2]), sta[3]);
    }
  }

  if (millis() - safe_ms > 1000)
  {
    safe_ms = millis();
    color_en = false;
  }
}

void colorSet(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

void colorSet(uint32_t c, int i) {
  strip.setPixelColor(i, c);
  strip.show();
}

void rainbowCycle(uint8_t wait, int r, int g, int b, int num) {
  if (num == 0)
  {
    for (int val = 0; val < 255; val++)
    {
      ble();
      if (color_en)
        return;
      colorSet(strip.Color(map(val, val_min, val_max, 0, r), map(val, val_min, val_max, 0, g), map(val, val_min, val_max, 0, b)));
      delay(wait);
    }
  }
  if (num == 1)
  {
    for (int val = 255; val >= 0; val--)
    {
      ble();
      if (color_en)
        return;
      colorSet(strip.Color(map(val, val_min, val_max, 0, r), map(val, val_min, val_max, 0, g), map(val, val_min, val_max, 0, b)));
      delay(wait);
    }
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      ble();
      if (color_en)
        return;
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
