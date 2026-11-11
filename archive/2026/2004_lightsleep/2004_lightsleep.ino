#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "esp_sleep.h"
#include "driver/gpio.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);  // address, columns, rows

const int BUTTON_PIN = 0;                    // BOOT button
const int LED_PIN = 2;                       // blue LED on DevKit V1
const unsigned long DEBOUNCE_MS = 50;
const unsigned long LONG_PRESS_MS = 1500;    // hold this long to sleep
const uint64_t SLEEP_US = 20ULL * 1000000ULL;  // 20 s between heartbeats
const unsigned long HEARTBEAT_MS = 200;      // LED on time per heartbeat

bool backlightOn = true;

bool lastReading = HIGH;
bool buttonState = HIGH;                     // debounced state
unsigned long lastDebounceTime = 0;
unsigned long pressStart = 0;
bool longPressHandled = false;

unsigned long lastUptimeUpdate = 0;

void updateUptime() {
  lcd.setCursor(8, 3);
  lcd.print(millis() / 1000);
  lcd.print(" s    ");   // trailing spaces clear leftover digits
}

// Returns true if the button was pressed during the heartbeat
bool heartbeat() {
  digitalWrite(LED_PIN, HIGH);
  unsigned long start = millis();
  bool pressed = false;
  while (millis() - start < HEARTBEAT_MS) {
    if (digitalRead(BUTTON_PIN) == LOW) { pressed = true; break; }
    delay(5);
  }
  digitalWrite(LED_PIN, LOW);
  return pressed;
}

void enterLightSleep() {
  Serial.println("Going to light sleep...");
  Serial.flush();

  lcd.noBacklight();
  lcd.noDisplay();
  digitalWrite(LED_PIN, LOW);

  // Wait for release, otherwise the held (LOW) button wakes us immediately
  while (digitalRead(BUTTON_PIN) == LOW) delay(10);
  delay(DEBOUNCE_MS);

  gpio_wakeup_enable(GPIO_NUM_0, GPIO_INTR_LOW_LEVEL);
  esp_sleep_enable_gpio_wakeup();
  esp_sleep_enable_timer_wakeup(SLEEP_US);

  while (true) {
    esp_light_sleep_start();                 // execution pauses here

    if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
      // 20 s passed: short LED blink, then back to sleep
      if (heartbeat()) break;                // button pressed during blink
    } else {
      break;                                 // woke by button
    }
  }

  // --- woken by button press ---
  digitalWrite(LED_PIN, HIGH);
  Serial.println("Woke up");

  // Wait for release so the wake press isn't read as a new press
  while (digitalRead(BUTTON_PIN) == LOW) delay(10);
  delay(DEBOUNCE_MS);

  lcd.display();
  lcd.backlight();
  backlightOn = true;
}

void handleButton() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastReading) {
    lastDebounceTime = millis();
  }
  lastReading = reading;

  if ((millis() - lastDebounceTime) > DEBOUNCE_MS && reading != buttonState) {
    buttonState = reading;

    if (buttonState == LOW) {                // pressed
      pressStart = millis();
      longPressHandled = false;
    } else if (!longPressHandled) {          // released after a short press
      backlightOn = !backlightOn;
      if (backlightOn) lcd.backlight();
      else             lcd.noBacklight();
    }
  }

  // Long press: trigger while still held
  if (buttonState == LOW && !longPressHandled &&
      (millis() - pressStart) >= LONG_PRESS_MS) {
    longPressHandled = true;
    enterLightSleep();

    // Reset button state and refresh the display after waking
    buttonState = HIGH;
    lastReading = HIGH;
    updateUptime();
    lastUptimeUpdate = millis();
  }
}

void setup()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);               // LED on while running

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
  handleButton();

  if (millis() - lastUptimeUpdate >= 1000) {
    lastUptimeUpdate = millis();
    updateUptime();
  }
}