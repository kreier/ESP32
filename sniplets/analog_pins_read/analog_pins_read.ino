// read analog pins on ESP32 v0.1 2020/11/06
//
// conversion digital value -> voltage: v = 0.826x + 150     in millivolt

const int ledPin = 2;
const int adc1[8] = {36, 37, 38, 39, 32, 33, 34, 35};
const int adc2[10] = {4, 0, 2, 15, 13, 12, 14, 27, 25, 26};

int raw, voltage;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Let's start!");
}  

void loop() {
  measureADC( 1 );
  measureADC( 2 );
  Serial.println("---- finished ------");
  digitalWrite(ledPin, 0);
  delay(10000);
  digitalWrite(ledPin, 1);
}

void measureADC(int adc) {
  int limitChannels = 8;
  int pinMeasure;
  char* string1 = "ADC1_CH";
  if(adc == 2) {
    limitChannels = 10;
    string1 = "ADC2_CH";
  }
  for(int i = 0; i < limitChannels; i++) {
    raw = 0;
    if(adc == 1) {
      pinMeasure = adc1[i];
    } else {
      pinMeasure = adc2[i];
    }
    for(int multisample = 0; multisample < 100; multisample++) {
      raw += analogRead( pinMeasure );
    }
    voltage = 0.00826 * raw + 150;
    Serial.print(string1);
    printSerial(i);
    Serial.print(" raw: ");
    Serial.print(raw / 100);
    Serial.print(" voltage: ");
    printSerial(voltage);
    Serial.println(" mV");
  }  
}

void printSerial(int value) {
  if(value < 10) Serial.print(" ");
  if(value < 100) Serial.print(" ");
  if(value < 1000) Serial.print(" ");
  Serial.print(value);   
}
