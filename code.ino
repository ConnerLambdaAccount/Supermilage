#include<Adafruit_GFX.h>
#include<SPI.h>
#include<Adafruit_ILI9341.h>

#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define LED_GREEN 33
#define LED_BLUE 37
#define LED_RED 35
#define LED_YELLOW 39

// Most of this is a mess cause I was the only guy on the team who knew how to code...

//int cutoff = 0;
//int cutoff_up = ;
//int cutoff_down = ;

// cutoff
// Avg. Speed
// Total time

double last_value = 0.00;
double ms;
double seconds;
double mph;
int mag_sensor = 3;
bool clear_line = false;
double last_mph;

void setup() {
  Serial.begin(9600);
  
  pinMode(mag_sensor, INPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  
  tft.begin();
  tft.setTextSize(3);
  tft.fillScreen(ILI9341_BLACK);
}

double getMPH(int sensor, double* last_value) {
  // Block till sensor reads a magnet
  while(digitalRead(sensor) != 0) {}
  ms = millis();
  seconds = ((double)(ms - *last_value)) / (double)1000;
  *last_value = ms;
  
  // Return miles per hour
  return ((double)1/seconds) * (double)60 * (double)60 * (double)1.57597 / (double)1609.34;
}

void loop() {
  // Get wheel MPH
  Serial.println("Blocking...");
  mph = getMPH(mag_sensor, &last_value);
  Serial.println("Got MPH");

  if(clear_line == true) {
    Serial.println("Clearing");
    tft.setTextColor(ILI9341_BLACK);
    tft.println(String(last_mph) + " mph");
  }
  
  // Print MPH
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(0, 0);
  if(mph > 40) {
    mph = 40;  
  }
  
  if(mph) {
    Serial.println(String(mph) + " mph");
    tft.println(String(mph) + " mph");
    last_mph = mph;
    clear_line = true;
  } else {
    clear_line = false;
  }

  // Delay() without affecting the timer, allowing driver to read the screen easily
  if(mph < 10) {
    delay(300);
    last_value = millis();
  } else if(mph < 20) {
    delay(200);
    last_value = millis();
  } else if(mph < 30) {
    delay(100);
    last_value = millis();
  } else {
    delay(100);
    last_value = millis();
  }

  // Cutoff
  //tft.setCursor(1, 1);
  tft.println("Cutoff: X");
  tft.print("\n");
  tft.println("Avg: X");
  tft.print("\n");
  tft.println("Time: X");

  tft.setCursor(0, 0);
  // Power on/off LEDs
  if(mph > 10) {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, HIGH);
  } else if(mph < 10) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_GREEN, LOW);
  } else {
    digitalWrite(LED_BLUE, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, LOW);
  }
}
