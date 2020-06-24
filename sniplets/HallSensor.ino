// Simple sketch to access the internal hall effect detector on the esp32.
// values can be quite low. 

int val = 0;
void setup() {
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = 0;
  for(int i = 0; i < 200; i++) {
    val = val + hallRead();
  }
  val = val / 200 + 50;
  // print the results to the serial monitor:
  for(int i = 0; i < val; i++) {
    Serial.print(".");
  }
  // Serial.print("sensor = ");
  Serial.println(val); //to graph 
}
