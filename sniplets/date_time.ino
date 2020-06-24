// date_time v0.1 2020/06/23
// Get and synchronise the time on an ESP32
//
// go to light sleep 
//
// ESP32 uses the time.h from lewlib: https://sourceware.org/newlib/libc.html#Timefns
//
// OTA update; https://blog.classycode.com/secure-over-the-air-updates-for-esp32-ec25ae00db43
//

 
#include <WiFi.h>
#include <credentials.h>  // WiFi credentials in separate file
#include <time.h>

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 0;
const int   ledPin = 5;  // T-Koala: 5   LoLin32 lite 22  DevKit V1 2

uint64_t mS_TO_S_FACTOR = 1000;  // Conversion factor for micro seconds to seconds
uint64_t TIME_TO_SLEEP  =  999;  // in milliseconds
int zeit  = 0;               // to determine the light sleep time correctly
int light = 0;               // times in light sleep

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  Serial.begin(115200);
  Serial.print(" ");
  delay(50);
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, HIGH);  
  syncTime();
  digitalWrite(ledPin, LOW); 
}

void loop() {
  zeit = millis();
  digitalWrite(ledPin, HIGH);
  printLocalTime();
  Serial.print("This took me now ");
  Serial.print(millis() - zeit);
  Serial.print(" milliseconds. Total millis: ");
  Serial.print(millis());
  Serial.print(" and times in light sleep: ");
  Serial.println(light);
  light++;
  digitalWrite(ledPin, LOW);
  delay(50);
  TIME_TO_SLEEP = 10000 + zeit - millis();
  // enable timer light sleep
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * mS_TO_S_FACTOR);    
  esp_light_sleep_start();
}

void syncTime() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  zeit = millis();
  WiFi.begin(ssid, password);
  int timeout = 10 * 4; // 10 seconds
  while(WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if(WiFi.status() != WL_CONNECTED) {
     Serial.println("Failed to connect, going back to sleep");
  }

  Serial.print("WiFi connected in: "); 
  Serial.print(millis() - zeit);
  Serial.print(", IP address: "); 
  Serial.println(WiFi.localIP());
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);  
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");
  
  int slot = timeinfo.tm_hour * 12 + int(timeinfo.tm_min / 5);
  Serial.print("The 5 minute slot to measure data and calculate averages is: ");
  Serial.println(slot);

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
}
