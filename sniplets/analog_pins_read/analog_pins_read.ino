// read analog pins on ESP32 v0.1 2020/11/06
//
// conversion digital value -> voltage: v = 0.826x + 150     in millivolt

const int ledPin = 2;
const int adc1[8] = {36, 37, 38, 39, 32, 33, 34, 35};
const int adc2[10] = {4, 0, 2, 15, 13, 12, 14, 27, 25, 26};

int raw, voltage;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Let's start!");
}  

void loop() {
  for(int i = 0; i < 8; i++) {
    raw = 0;
    for(int multisample = 0; multisample < 100; multisample++) {
      raw += analogRead( adc1[i] );
    }
    voltage = 0.826 * raw + 150;
    Serial.print("ADC1_CH");
    Serial.print(i);
    Serial.print(" raw: ");
    Serial.print(raw / 1000);
    Serial.print(" voltage: ");
    Serial.println(voltage);
  }
  digitalWrite(ledPin, 0);
  delay(1000);
  digitalWrite(ledPin, 1);
}
