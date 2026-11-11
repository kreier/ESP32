#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  // address, columns, rows

void setup()
{
  Wire.begin(); // default ESP32 pins: SDA = 21, SCL = 22
  lcd.init();
  lcd.backlight();  

  lcd.setCursor(0, 0);
  lcd.print("Hello, ESP32!");
  lcd.setCursor(0, 1);
  lcd.print("2004 on 0x27");
  lcd.setCursor(0, 2);
  lcd.print("20 columns, 4 rows");
  lcd.setCursor(0, 3);
  lcd.print("Uptime: ");

  Serial.begin(115200);
  while (!Serial);
  Serial.println("\nI connected to an 1602 display");
}
 
void loop() {
  lcd.setCursor(8, 3);
  lcd.print(millis() / 1000);
  lcd.print(" s    ");        // trailing spaces clear leftover digits
  delay(1000);
}
