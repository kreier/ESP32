#include <Wire.h>
 
void setup() {
  //Wire.begin(14, 12); // SDA, SCL
  Wire.begin(); // ESP32 default SDA GPIO21 and SCL GPIO22 
  Serial.begin(115200);
  Serial.println("\ni2cdetect");
}

void loop() {
  byte error, address, address_l, address_h;
  int nDevices;
  Serial.println("Scanning address range 0x00-0x7F");
  Serial.println(" ");
  Serial.println("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f");
  nDevices = 0;
  for(address_h = 0; address_h <= 7; address_h++ ) {
    Serial.print(address_h);
    Serial.print("0: ");
    for(address_l = 0; address_l < 16; address_l++) {
      address = address_h * 16 +  address_l;
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
      if (error == 0) {
        Serial.print(address_h);
        Serial.print(address_l,HEX);
        Serial.print(" ");
        nDevices++;
      }
      else if (error==4) {
        Serial.print("xx ");
      } else {
        Serial.print("-- ");   
      }
    }
    Serial.println(" ");
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  delay(10000);
}
