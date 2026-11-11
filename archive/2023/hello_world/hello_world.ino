int column = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("\n");
  Serial.println("Hello World!");
}

void loop() {
  Serial.print(".");
  delay(2000);
  column += 1;
  if(column > 80)
  {
    column = 0;
    Serial.println(".");
  }
}
