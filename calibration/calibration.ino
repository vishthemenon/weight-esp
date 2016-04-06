#include <ESP8266WiFi.h>
#include <Q2HX711.h>

const byte hx711_data_pin = 4;
const byte hx711_clock_pin = 5;
float tareFactor = 0;
float calibFactor = -1;
float calibWeight = 1.25;
Q2HX711 hx711(hx711_data_pin, hx711_clock_pin);

int waitButtonPin = 2;
bool currentState = HIGH;


void tare() {
  tareFactor = hx711.read();
}

float readLikeABoss() {
  float cool = (hx711.read() - tareFactor) / calibFactor;
  return (cool / 1000);
}

float readLikeASuperBoss(int repeat) {
  float cool = 0;
  for (int i = 0; i < repeat; i++) {
    cool += (hx711.read() - tareFactor) / calibFactor;
    delay(10);
  }
  return (cool / repeat);
}

float printLikeASuperBoss(int repeat) {
  Serial.println(String(readLikeASuperBoss(repeat)) + " | TareFactor: " + String(tareFactor) + " | CalibFactor: " + String(calibFactor));
}

void buttonWait() {
  int buttonState;
  int lastButtonState = HIGH;
  long lastDebounceTime = 0;
  long debounceDelay = 100;
  while (true) {
    int reading = digitalRead(waitButtonPin);
    if (reading != lastButtonState) {
      lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (reading != buttonState) {
        buttonState = reading;
        if (buttonState == HIGH) {
          break;
        }
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  delay(100);
  pinMode(waitButtonPin, INPUT);
}



void loop() {
  Serial.println("Howdy Folks!");
  delay(50);
  Serial.println("Looks like a good day for some calibration!");
  delay(50);
  Serial.println("Remove them dum-doo-dab-dooly from the scale and STAY CLEAR!");
  Serial.println("Yall jolly well press that USER button when yall ready!");
  buttonWait();
  delay(100);
  tare();
  delay(100);
  printLikeASuperBoss(20);
  Serial.println("Place a dum-doo-dab-dooly" + String(calibWeight) + "weight on the scale and STAY CLEAR!");
  Serial.println("Yall jolly well press that USER button when yall ready!");
  buttonWait();
  calibFactor *= readLikeASuperBoss(20) / calibWeight;
  Serial.println("Beep..Bop..Beep..Bop..Calibrating..");
  printLikeASuperBoss(20);
  Serial.println("Mmmhmm..thats some hardcore southern calibrationisationing");
  delay(100);
  buttonWait();
}


