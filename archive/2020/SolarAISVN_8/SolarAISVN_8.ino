// Solar- and windmeter at AISVN v0.8 
// 2020/06/15
//
// pin:       32,      33,       34,       35,   14,   26,   27,     12,   13
// value:  solar, battery, currentA, currentB, load, wind, temp, solar2, LiPo
// submit: solar, battery, current, load, wind, temp, solar2, LiPo, bootCount
//             0,       1,       2,    4,    5,    6,      7,    8,
 
#include <WiFi.h>
#include <Wire.h>
#include <soc/sens_reg.h>

RTC_DATA_ATTR int bootCount = 0;
static RTC_NOINIT_ATTR int reg_b; // place in RTC slow memory so available after deepsleep

// Replace with your SSID and Password
const char* ssid     = "AISVN@Staff";  // AISVN@Staff
const char* password = "/\\1SVN4St@ff"; //  /\\1SVN4St@ff

// Replace with your unique IFTTT URL resource
const char* resource = "/trigger/aisvn/with/key/bqgP3Ljipzw7tfHKj5bvwK";

// Maker Webhooks IFTTT
const char* server = "maker.ifttt.com";

// Time to sleep
uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
// sleep for 2 minutes = 120 seconds
uint64_t TIME_TO_SLEEP = 120;

//    32,      33,       34,       35,   14,   26,   27,     12,   13
// solar, battery, currentA, currentB, load, wind, temp, solar2, LiPo  

int voltage[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};       // all voltages in millivolt
int pins[9] = {32, 33, 34, 35, 14, 26, 27, 12, 13};   // solar, battery, curA, curB, load, wind, temp, solar2, LiPo
int ledPin = 5;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);  
  Serial.begin(115200);
  // determine cause of reset
  esp_reset_reason_t reason = esp_reset_reason();
  Serial.print("reason ");Serial.println(reason);
  // get reg_b if reset not from deep sleep
  if ((reason != ESP_RST_DEEPSLEEP)) {
    reg_b = READ_PERI_REG(SENS_SAR_READ_CTRL2_REG);
    Serial.println("Reading reg b.....");
  }
  Serial.print("reg b: ");
  printf("%" PRIu64 "\n", reg_b);
  delay(50);
  digitalWrite(ledPin, LOW);
  bootCount++;
  delay(1000);
  measureVoltages();

  digitalWrite(ledPin, HIGH);  
  initWifi();
  makeIFTTTRequest();
  digitalWrite(ledPin, LOW); 

  // enable timer deep sleep
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);    
  Serial.println("Going to sleep now");
  // start deep sleep for 120 seconds (2 minutes)
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

  String jsonObject = String("{\"value1\":\"") + voltage[0] + "|||" + voltage[1] + "|||" + voltage[2]
                          + "\",\"value2\":\"" + voltage[4] + "|||" + voltage[5] + "|||" + voltage[6]
                          + "\",\"value3\":\"" + voltage[7] + "|||" + voltage[8]
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
  WRITE_PERI_REG(SENS_SAR_READ_CTRL2_REG, reg_b); // only needed after deep sleep
  SET_PERI_REG_MASK(SENS_SAR_READ_CTRL2_REG, SENS_SAR2_DATA_INV);
  Serial.print(" ** Voltages measured: ");
  for(int i = 0; i < 9; i++) {
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
  //    32,      33,       34,       35,   14,   26,   27,     12,   13
  // solar, battery, currentA, currentB, load, wind, dump, solar2, LiPo  
  voltage[0] = int((4096 - voltage[0]) * 7.52 - 1000);  // pin32 solar    voltage divider 10k : 1.2 k Ohm 1:1
  if(voltage[0] < 0) voltage[0] = 0;
  voltage[1] = int((4096 - voltage[1]) * 7.52 - 1000);  // pin33 battery  voltage divider 10k : 1.2 k Ohm 1:1
  //voltage[2] = int((voltage[2]) * 0.804 + 129);         // pin34 voltage solar minus green LED
  //voltage[3] = int((voltage[3]) * 0.804 + 129);         // pin35 voltage solar minus green LED minus 0.1 Ohm serial
  voltage[2] = int((voltage[3] - voltage[2]) * 6.75);   // voltage difference pin35 - pin34 x 8.4 is corrent (x0.804)
  voltage[4] = int((4096 - voltage[4]) * 7.52 - 1000);  // pin14 load     voltage divider 10k : 1.2 k Ohm 1:1
  if(voltage[4] < 0) voltage[4] = 0;  
  voltage[5] = int((4096 - voltage[5]) * 7.52 - 1000);  // pin26 wind     voltage divider 10k : 1.2 k Ohm 1:1
  if(voltage[5] < 0) voltage[5] = 0;  
  voltage[6] = int((voltage[6]) * 0.804 + 129);         // pin27 temp     not connected yet
  voltage[7] = int((voltage[7]) * 4.583 + 735);         // pin12 Solar2   voltage divider 4.7k : 1k
  if(voltage[7] < 736) voltage[7] = 0;
  voltage[8] = int((voltage[8]) * 1.608 + 258);         // pin13 LiPo     voltage divider 100kOhm 2:1
  Serial.print("Boot number: ");
  Serial.println(bootCount);  
}
