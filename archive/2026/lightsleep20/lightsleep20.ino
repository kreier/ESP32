#include "esp_sleep.h"

const int LED_PIN = 2;                         // blue LED on ESP32 DevKit V1

const unsigned long FIRST_RUN_MS = 2000;       // awake time after startup
const unsigned long AWAKE_MS     = 500;        // awake time after each wake-up
const uint64_t SLEEP_US          = 120ULL * 1000000ULL;  // 20 s in microseconds

void lightSleep20s() {
  Serial.println("Light sleep for 20 s...");
  Serial.flush();                              // let the serial output finish

  digitalWrite(LED_PIN, LOW);                  // LED off before sleeping

  esp_sleep_enable_timer_wakeup(SLEEP_US);
  esp_light_sleep_start();                     // execution pauses here

  // --- woke up after 20 s ---
  digitalWrite(LED_PIN, HIGH);                 // LED on while awake
  Serial.println("Awake");
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);                 // LED on at startup

  Serial.begin(115200);
  Serial.println("\nStarted, LED on");

  delay(FIRST_RUN_MS);                         // stay awake 2 s
  lightSleep20s();                             // first sleep
}

void loop() {
  delay(AWAKE_MS);                             // stay awake 0.5 s
  lightSleep20s();                             // then sleep 20 s again
}