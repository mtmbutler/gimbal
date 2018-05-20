#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Made it through setup");
}

void loop() {
  Wire.beginTransmission(8);
  Serial.println("Began transmission");  
  Wire.write(100);
  Serial.println("Wrote 100");
  Wire.write(1);
  Serial.println("Wrote 1");
  Wire.endTransmission();
  Serial.println("Sent");
}
