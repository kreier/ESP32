// prime numbers
#include "esp32-hal-cpu.h"

int led = 2; // for ESP32 it's 2
int prime = 1;
double number = 3;
double divider = 2;
int zahl = 0;
unsigned long zeit;

void setup() {
  Serial.begin(115200);
  while(!Serial); // Just for the first 10 seconds of the Leonardo
  zeit = millis();
  setCpuFrequencyMhz(40);
  Serial.println(String(getCpuFrequencyMhz()));
  Serial.println("Prime numbers until 10000:");
  Serial.print("2, 3, 5, 7, ");
  for(number = 11; number < 10000 ; number += 2) {
    prime = 1;
    for(divider = 2; divider < (sqrt(number) + 1); divider += 1) {
      if( number/divider == int(number/divider)) {
        prime = 0;
        divider = number;
      }
    }
    if(prime == 1) {
      zahl++;
      if(zahl > 17) {
        Serial.print("\n");
        zahl = 0;
      } 
      Serial.print(int(number));
      Serial.print(", ");
    }
  }
  pinMode(led, OUTPUT);
  Serial.println("\nDone. It took me ");
  Serial.print(millis() - zeit);
  Serial.print(" milliseconds.");
}

void loop() {
  digitalWrite(led, HIGH);
  delay(20);
  digitalWrite(led, LOW);
  delay(80);
  digitalWrite(led, HIGH);
  delay(20);
  digitalWrite(led, LOW);
  delay(1000);
}
