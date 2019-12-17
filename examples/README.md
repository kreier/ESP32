# Code snipplets

The internet has a variety of code for the ESP32. Here I just collect some code that worked.

## Temperature sensor

Found this post from [circuits4you](https://circuits4you.com/) from January 2019 about a way to access the temperature sensor of the ESP32. Unfortunately on most boards this sensor is not present. My board returns 53.33 'C as well.

https://circuits4you.com/2019/01/01/esp32-internal-temperature-sensor-example/

``` c
/* 
 *  https://circuits4you.com
 *  ESP32 Internal Temperature Sensor Example
 */

 #ifdef __cplusplus
  extern "C" {
 #endif

  uint8_t temprature_sens_read();

#ifdef __cplusplus
}
#endif

uint8_t temprature_sens_read();
//====================================================
//         Setup
//====================================================
void setup() {
  Serial.begin(115200);
}

//====================================================
//         Loop
//====================================================
void loop() {
  Serial.print("Temperature: ");
  
  // Convert raw temperature in F to Celsius degrees
  Serial.print((temprature_sens_read() - 32) / 1.8);
  Serial.println(" C");
  delay(1000);
}
```
