/* Blink sweep to find LED pin */
// created July 2020, last updated 2020-10-14
// LORA915:        2   and ESP8266 NodeMCU, D1 R32 (Arduino size), DOIT ESP32 DEVKIT V1
// Wemos LOLIN32:  4
// T-Koala:        5   and T-Energy, 
// ATMega2560:    13   and Arduino Uno, Leonardo
// T8 V1.7:       21
// WeMos lite:    22
// TTGO LoRa      25  (not populated)

RTC_DATA_ATTR int ledPin = 0;
bool light = HIGH;    // LORA915, T-Koala, T8, Arduino
//bool light = LOW;   // WEMOS Lite, ESP8266 NODEMCU
bool dark  = !light;

void setup() {
  ledPin++;
  if (ledPin == 5) ledPin = 12;
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Serial.print("Let's start with pin ");
  Serial.print(ledPin);
  digitalWrite(ledPin, light);
  Serial.print(": ON ");
  delay(50);
  digitalWrite(ledPin, dark);
  delay(100);
  digitalWrite(ledPin, light);
  delay(50);
  digitalWrite(ledPin, dark);
  Serial.print("off ");
  delay(2000);
  digitalWrite(ledPin, light);
  Serial.print("ON ");
  delay(50);
  digitalWrite(ledPin, dark);
  delay(100);
  digitalWrite(ledPin, light);
  delay(50);
  digitalWrite(ledPin, dark);
  Serial.println("off ");
  delay(2000);
  Serial.println("Going to sleep for 4 seconds");
  esp_sleep_enable_timer_wakeup(4 * 1000000);
  esp_deep_sleep_start();
}

void loop() {
  // never gets here
}
