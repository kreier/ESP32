// Solar- and windmeter at AISVN2 v0.4
// 2020/06/23
//
// read state if pin25 and write to voltage[5]
// still with deep sleep, value therefore in RTC_DATA_ATTR pin25
//
// pin:          34,       35,     32,       33,       25,    12
// value:  currentA, currentB, solar3, battery2, load_OUT, LiPo2
//
// submit: solar, battery, currentA, currentB, LiPo2, load, bootCount
//             0,       1,        2,        3,     4,    5
 
#include <WiFi.h>
#include <Wire.h>
#include <credentials.h>  // WiFi credentials in separate file
#include <soc/sens_reg.h>
#include <driver/rtc_io.h>

RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR int pin25 = 0;
gpio_num_t pin_LOAD = GPIO_NUM_25;

// Replace with your SSID and Password + uncomment
// const char* ssid     = "REPLACE_WITH_YOUR_SSID";
// const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Replace with your unique IFTTT URL resource + uncomment
// const char* resource = "/trigger/data/with/key/value";

// Maker Webhooks IFTTT
const char* server = "maker.ifttt.com";

// Time to sleep
uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
uint64_t TIME_TO_SLEEP = 118; // need two seconds each time to upload ... sleep 2 minutes

// pin:          34,       35,     32,       33,    12,       25
// value:  currentA, currentB, solar3, battery2, LiPo2, load_OUT 

int voltage[6] = {0, 0, 0, 0, 0, 0};       // all voltages in millivolt
int pins[6] = {32, 33, 34, 35, 12, 25}; // solar, battery, curA, curB, LiPo, load(OUT)
int ledPin = 2;  // T-Koala: 5   LoLin32 lite 22  DevKit V1 2

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  pinMode(25, OUTPUT);
  digitalWrite(25, HIGH);   
  rtc_gpio_init(pin_LOAD);
  rtc_gpio_set_direction(pin_LOAD,RTC_GPIO_MODE_OUTPUT_ONLY);
  Serial.begin(115200);
  // determine cause of reset
  esp_reset_reason_t reason = esp_reset_reason();
  Serial.print("reason ");Serial.println(reason);
  delay(50);
  digitalWrite(ledPin, LOW);
  bootCount++;
  delay(1000);
  measureVoltages();

  // battery too low? switch off load via pin_LOAD in rtc
  if(voltage[1] < 12200) {
    pin25 = 0; // in RTC slow mem - survives deep sleep
  } 
  if(voltage[1] > 12800) { // voltage high enough to power load on
    pin25 = 1;
  }
  rtc_gpio_set_level(pin_LOAD, pin25); // GPIO HIGH
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
  voltage[5] = pin25;

  digitalWrite(ledPin, HIGH);  
  initWifi();
  makeIFTTTRequest();
  digitalWrite(ledPin, LOW); 

  // enable timer deep sleep
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);    
  Serial.println("Going to sleep now");
  // start deep sleep
  esp_deep_sleep_start();
}

void loop() {
  // sleeping so wont get here 
}

// Establish a Wi-Fi connection with your router
void initWifi() {
  Serial.print("Connecting to: "); 
  Serial.print(ssid);
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
  Serial.print(millis());
  Serial.print(", IP address: "); 
  Serial.println(WiFi.localIP());
}

// Make an HTTP request to the IFTTT web service
void makeIFTTTRequest() {
  Serial.print("Connecting to "); 
  Serial.print(server);
  
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()) {
    Serial.println("Failed to connect...");
  }
  
  Serial.print("Request resource: "); 
  Serial.println(resource);

  String jsonObject = String("{\"value1\":\"") + voltage[0] 
                                       + "|||" + voltage[1] 
                          + "\",\"value2\":\"" + voltage[2] 
                                       + "|||" + voltage[3]
                          + "\",\"value3\":\"" + voltage[4] 
                                       + "|||" + voltage[5]
                          + "|||" + bootCount + "\"}";
                      
  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server); 
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);
        
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  if(!!!client.available()) {
    Serial.println("No response...");
  }
  while(client.available()){
    Serial.write(client.read());
  }
  
  Serial.println("\nclosing connection");
  client.stop(); 
}

void measureVoltages() {
  Serial.print(" ** Voltages measured: ");
  for(int i = 0; i < 5; i++) {
    // multisample 100x to reduce noise - 9.5 microseconds x 100 = 1ms per voltage
    voltage[i] = 0;
    for(int multi = 0; multi < 100; multi++) {
      voltage[i] += analogRead( pins[i] );
    }
    voltage[i] = voltage[i] / 100;
    Serial.print(voltage[i]);
    Serial.print("  ");
  }
  // conversion to voltage prior to voltage divider
  // 34 currentA 1.5 Ohm diode 100k
  // 35 currentB 0.1 Ohm diode 100k
  // 32 solar3 10k:10k -> 47k:10k maps 0-20V to 2-12V to 0.35-2.1V
  // 33 battery2 47k:10k 5.7:1
  // 25 output switch connects D-E or 6-9 to 2SD613 npn transistor
  // 26 
  // 27 
  // 14
  // 12 LiPo 100k : 100k 2:1
  // 
  // pin:          34,       35,     32,       33,       25,    12
  // value:  currentA, currentB, solar3, battery2, load_OUT, LiPo2
  //
  // submit: solar, battery, currentA, currentB, LiPo2, load, bootCount
  //             0,       1,        2,        3,     4,    5

  // solar3
  // pin32 solar3 double voltage divider 10:10 & 47:10
  voltage[0] = int(25300 - voltage[0] * 11.7);  
  if(voltage[0] < 0) voltage[0] = 0;
  //
  // battery2
  // pin33 battery  voltage divider 47k:10k 5.7:1
  voltage[1] = int((voltage[1] * 0.83 + 150) * 5.7);
  //
  // currentA
  // voltage[2} is difference pin34 to 1.580 Volt - over 1.6 Ohm - current is 1/1.6 voltage
  voltage[2] = int(((voltage[2] * 0.83 + 150) - 1564) / 1.6);
  //
  // currentB
  // voltage[3} is difference pin35 to 1.580 Volt - over 0.1 Ohm - current is 1/0.1 voltage
  voltage[3] = int(((voltage[3] * 0.83 + 150) - 1660) * 10); // fixed 2020/06/24 was 1627
  //
  // LiPo2
  // pin12 LiPo2     voltage divider 100k:100k 2:1
  voltage[4] = int((voltage[4] * 0.83 + 150) * 2.0);  
  Serial.print("Boot number: ");
  Serial.println(bootCount);
}
