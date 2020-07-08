#include <Wire.h>
#include <DS3231.h>

DS3231 rtc;

bool century = false;
bool h12Flag;
bool pmFlag;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nRTC clock");
}

void loop() {
  Serial.print("20");
  Serial.print(rtc.getYear(), DEC);
  Serial.print("-");
  if(rtc.getMonth(century) < 10) Serial.print("0");
  Serial.print(rtc.getMonth(century), DEC);
  Serial.print("-");
  if(rtc.getDate() < 10) Serial.print("0");
  Serial.print(rtc.getDate(), DEC);
  Serial.print(" ");
  if(rtc.getHour(h12Flag, pmFlag) < 10) Serial.print("0");  
  Serial.print(rtc.getHour(h12Flag, pmFlag), DEC); //24-hr
  Serial.print(":");
  if(rtc.getMinute() < 10) Serial.print("0");
  Serial.print(rtc.getMinute(), DEC);
  Serial.print(":");
  if(rtc.getSecond() < 10) Serial.print("0");
  Serial.print(rtc.getSecond(), DEC);
  Serial.print("  temperature: ");
  Serial.println(rtc.getTemperature());
  delay(10000);
}
