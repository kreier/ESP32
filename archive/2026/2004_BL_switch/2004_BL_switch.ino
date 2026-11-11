#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  // address, columns, rows

const int BUTTON_PIN = 0;            // BOOT button on ESP32 DevKit V1
bool backlightOn = true;

bool lastReading = HIGH;             // last raw reading
bool buttonState = HIGH;             // debounced state
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

unsigned long lastUptimeUpdate = 0;

void setup()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);

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
  Serial.println("\nI connected to a 2004 display");
}

void loop() {
  // --- Button handling (toggle backlight on press) ---
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {          // pressed
        backlightOn = !backlightOn;
        if (backlightOn) lcd.backlight();
        else             lcd.noBacklight();
      }
    }
  }
  lastReading = reading;

  // --- Uptime update once per second ---
  if (millis() - lastUptimeUpdate >= 1000) {
    lastUptimeUpdate = millis();
    lcd.setCursor(8, 3);
    lcd.print(millis() / 1000);
    lcd.print(" s    ");   // trailing spaces clear leftover digits
  }
}