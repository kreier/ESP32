/* Blink */
// T-Koala:    5
// Wemos lite: 4
// LORA915:    2
// T8 V1.7:    21
// T-Dislay:   
// NodeMCU:    2
// ATMega2560: 13


// int ledPin = LED_BUILTIN;
int ledPin = 13;
bool light = HIGH; // LORA915, T-Koala, T8, Arduino
bool dark  = LOW;
//bool light = LOW; // WEMOS Lite, ESP8266 NODEMCU
//bool dark  = HIGH;
int count = 0; 

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Let's start!");
}

void loop() {
  digitalWrite(ledPin, light);
  Serial.print("LED on ");
  delay(50);
  digitalWrite(ledPin, dark);
  delay(100);
  digitalWrite(ledPin, light);
  delay(50);
  digitalWrite(ledPin, dark);
  Serial.print("LED off ");
  count++;
  if(count > 6) {
    count = 0;
    Serial.println(ledPin);
  }
  delay(2000);
}
