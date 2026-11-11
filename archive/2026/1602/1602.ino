#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // address, columns, rows

void setup()
{
  Wire.begin();
  lcd.init();
  lcd.backlight();  

  lcd.setCursor(0, 0);
  lcd.print("Hello, ESP32!");
  lcd.setCursor(0, 1);
  lcd.print("1602 on 0x27");

  Serial.begin(115200);
  while (!Serial);
  Serial.println("\nI connected to an 1602 display");
}
 
void loop() {
  lcd.setCursor(13, 1);
  lcd.print(millis() / 1000 % 1000);  // seconds counter in the corner
  delay(1000);
}
