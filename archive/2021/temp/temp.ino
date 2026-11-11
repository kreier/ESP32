/* temp */
// LORA915:        2   and ESP8266 NodeMCU, D1 R32 (Arduino size), DOIT ESP32 DEVKIT V1
// Wemos LOLIN32:  4
// T-Koala:        5   and T-Energy, 
// ATMega2560:    13   and Arduino Uno, Leonardo
// T8 V1.7:       21
// WeMos lite:    22
// TTGO LoRa      25  (not populated)


// int ledPin = LED_BUILTIN;
int ledPin = 2;
bool light = HIGH;    // LORA915, T-Koala, T8, Arduino
//bool light = LOW;   // WEMOS Lite, ESP8266 NODEMCU
bool dark  = !light;
int count = 0; 

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Let's start!");
  Serial.println("This is version v0.1.20211122");
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
