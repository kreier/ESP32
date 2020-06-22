/* Blink */
// T-Koala:    5
// Wemos lite: 4
// LORA915:    2
// T8 V1.7:    21

int ledPin = 5;
bool light = HIGH; // LORA915, T-Koala, T8, 
bool dark  = LOW;
// bool light = LOW; // WEMOS Lite
// bool dark  = HIGH;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Let's start");
}

void loop() {
  digitalWrite(ledPin, light);
  Serial.println("LED on");
  delay(50);
  digitalWrite(ledPin, dark);
  delay(100);
  digitalWrite(ledPin, light);
  delay(50);
  digitalWrite(ledPin, dark);
  Serial.println("LED off");
  delay(2000);
}
