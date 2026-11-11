/* Prime numbers in Arduino C v5.1 2023/12/06 */
#include <time.h>
#include <math.h>

double start;
int column = 10;
int found = 4;   // we already know 2, 3, 5, 7
int divisors = found;
int primes[10000] = {3, 5, 7};

int is_prime(int number) {
  int prime = 1;
  for (int divider = 3; divider < (int)(sqrt(number)) + 1; divider += 2)
  {
    if (number % divider == 0)
    {
      prime = 0;
      break;
    }
  }
  return prime;
}

int find_primes(int limit) {
  for(int number=11; number < limit + 1; number += 2) { 
    if( is_prime(number) == 1) {
      primes[found - 1] = number;
      found++;
    }
  }
  primes[found - 1] = limit;
  divisors = found - 1;
  return 1;
}

int is_prime_fast(int number) {
  int largest_divider = (int)(sqrt(number));
  int flag_prime = 1;
  // Serial.print(number);
  // Serial.print(" - ");
  for(int i=0; i < divisors; i++)
  {
    // Serial.print(primes[i]);
    // Serial.print(" ");
    if(number % primes[i] == 0) 
    {
      flag_prime = 0;
      break;
    }
    if(primes[i] > largest_divider)
    {
      break;
    }
  }
  // Serial.print("\n");
  return flag_prime;
}

void elapsed_time(int seconds) {
  int hours = (int)seconds/3600;
  int minutes = (int)(seconds/60 - hours*60);
  int sec = (int)(seconds - minutes*60 - hours*3600);
  Serial.print(" ");
  Serial.print(hours);
  Serial.print("h ");
  Serial.print(minutes);
  Serial.print("min ");
  Serial.print(sec);
  Serial.print("s ");
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  for (int i = 0; i < 3; i++) {
    Serial.print(".");
    delay(1000);
  }
  const int scope[] = {100, 1000, 10000, 100000, 1000000, 10000000, 25000000, 100000000, 1000000000};
  const int reference[] = {25, 168, 1229, 9592, 78498, 664579, 1565927, 5761455, 50847534};
  for (int i = 0; i < 9; i++)
  {
    int last = scope[i];
    found = 4;   // we already know 2, 3, 5, 7
    Serial.println("\n\nPrime v5.0 in Arduino C 2023/12/06");
    Serial.print("Calculating prime numbers until ");
    Serial.println(last);
    start = micros();      // use micros() for more precision
    // Serial.println(start/1000000, 6);
    int largest_divider = (int)(sqrt(last)); 
    if(largest_divider % 2 == 0)
    {
      largest_divider += 1;
    }
    find_primes(largest_divider);
    Serial.print("Found ");
    Serial.print(found);
    Serial.print(" primes until ");
    Serial.print(largest_divider);
    Serial.print(" to use as divisors.\n");
    int dot = millis();
    int column = 0;
    for(int number = largest_divider + 2; number < last; number += 2)
    {
      found += is_prime_fast(number);
      if((millis() - dot) > 2000) {
        Serial.print(".");
        dot = millis();
        column += 1;
        if(column > 40) {
          column = 0;
          elapsed_time(dot/1000);
          Serial.print(" - ");
          Serial.print(number);
          Serial.print(" ");
          Serial.print((int)(number / (last / 100)));
          Serial.print("% \n");
        }
      }
    }
    float duration = (micros() - start)/1000000;
    if(duration > 2) {
      Serial.print("\n");
    }    
    Serial.print("Found ");
    Serial.print(found);
    Serial.print(" prime numbers. It should be ");
    Serial.print(reference[i]);
    Serial.print(".\nThis took ");
    Serial.print(duration, 6);
    Serial.print(" seconds.");
    elapsed_time(duration);    
  }
  // program finished, just keep printing some dots  
  for(int line = 0; line < 24; line++){
    for(int i = 0; i < 80; i++) {
      Serial.print(".");
      delay(2000);
    }
    Serial.print(".\n");
  }
}
