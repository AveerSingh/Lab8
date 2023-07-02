/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/aveersingh/Lab8/src/Lab8.ino"
#include <Wire.h>
#include "SparkFun_VCNL4040_Arduino_Library.h"
#include "oled-wing-adafruit.h"
#include "blynk.h"


void displayProximityValue(unsigned int value);
void displayLightLevel(unsigned int value);
void sendToBlynk(unsigned int value);
void setup();
void loop();
#line 7 "/Users/aveersingh/Lab8/src/Lab8.ino"
OledWingAdafruit display;
VCNL4040 proximitySensor;

const int LED1 = D7;
const int LED2 = D6;
const int LED3 = D5;

bool useBlynk = false;
BlynkTimer timer;

unsigned int threshold1 = 60;  
unsigned int threshold2 = 13000;  

void displayProximityValue(unsigned int value) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Proximity Value:");
  display.println(value);
  display.display();
}

void displayLightLevel(unsigned int value) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Light Level:");
  display.println(value);
  display.display();
}

void sendToBlynk(unsigned int value) {
  Blynk.virtualWrite(V1, value);
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  if (!proximitySensor.begin()) {
    Serial.println("Device not found. Please check wiring.");
    while (1);
  }
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  
  display.setup();
  display.clearDisplay();
  display.display();
  
  Blynk.begin("YOUR_AUTH_TOKEN"); // Replace "YOUR_AUTH_TOKEN" with your actual Blynk authentication token
  timer.setInterval(1000L, []() {
    Blynk.run();
  });
}

void loop() {
  display.loop();
  
  unsigned int proximityValue = proximitySensor.getProximity();
  unsigned int lightLevel = proximitySensor.getAmbient();
  
  if (display.pressedA()) {
    displayProximityValue(proximityValue);
  }
  
  if (display.pressedB()) {
    displayLightLevel(lightLevel);
  }
  
  if (display.pressedC()) {
    useBlynk = !useBlynk;
    delay(500);  // debounce delay to avoid unintended button press
    display.clearDisplay();
    display.display();
  }
  
  if (proximityValue <= threshold1) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
  } else if (proximityValue <= threshold2) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
  } else {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
  }
  
  if (useBlynk) {
    sendToBlynk(proximityValue);
  }
}
