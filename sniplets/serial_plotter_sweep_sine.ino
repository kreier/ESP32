/* Serial Sweep */

int ledPin = 13;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  Serial.println("Sinus Cosinus");
  delay(1000);
}

void loop() {
  for(int i = 0; i < 628; i++) {
    Serial.print((sin(i/100.0)+1)*10);
    Serial.print(" ");
    Serial.println((cos(i/100.0)+1)*10);
    delay(3);
  }
}