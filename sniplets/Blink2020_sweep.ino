/* Blink */
// T-Koala:    5
// Wemos lite: 4
// LORA915:    2
// T8 V1.7:    21
// Devkit V1:  2

RTC_DATA_ATTR int ledPin = 0;
bool light = HIGH; // LORA915, T-Koala, T8, 
bool dark  = LOW;
// bool light = LOW; // WEMOS Lite
// bool dark  = HIGH;

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
