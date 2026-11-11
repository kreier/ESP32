// Solar- and windmeter at AISVN - test edition
 
#include <WiFi.h>
#include <Wire.h>
#include <soc/sens_reg.h>

RTC_DATA_ATTR int bootCount = 0;
static RTC_NOINIT_ATTR int reg_b; // place in RTC slow memory so available after deepsleep

// Replace with your SSID and Password
const char* ssid     = "AISVN@Staff";  // AISVN@Staff
const char* password = "/\\1SVN4St@ff"; //  /\\1SVN4St@ff

// Replace with your unique IFTTT URL resource
const char* resource = "/trigger/test/with/key/bqgP3Ljipzw7tfHKj5bvwK";

// Maker Webhooks IFTTT
const char* server = "maker.ifttt.com";

// Time to sleep
uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
// sleep for 2 minutes = 120 seconds
uint64_t TIME_TO_SLEEP = 120;

int voltage[8] = {0, 0, 0, 0, 0, 0, 0, 0};       // all voltages in millivolt
int pins[8] = {32, 33, 34, 35, 25, 26, 27, 13};   // solar, battery, load_1, load_2, LiPo, wind, dump
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
                          + "\",\"value2\":\"" + voltage[3] + "|||" + voltage[4] + "|||" + voltage[5]
                          + "\",\"value3\":\"" + voltage[6] + "|||" + voltage[7] + "|||" +bootCount + "\"}";
                      
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
  for(int i = 0; i < 8; i++) {
    voltage[i] = analogRead( pins[i] );
    voltage[i] = int( voltage[i] * 0.826 + 150 );
    if( voltage[i] == 150 ) voltage[i] = 0;
    if( voltage[i] > 3300 ) voltage[i] = 3300;
    Serial.print(voltage[i]);
    Serial.print("  ");
  }
  voltage[0] = int((3300 - voltage[0]) * 9.4);  // pin34 solar  voltage divider 11kOhm 11:1
  voltage[1] = int((3300 - voltage[1]) * 9.4);  // pin34 solar  voltage divider 11kOhm 11:1
  voltage[7] = int(voltage[7] * 2);            // pin26 LiPo   voltage divider 100kOhm 2:1
  Serial.print("Boot number: ");
  Serial.println(bootCount);  
}
