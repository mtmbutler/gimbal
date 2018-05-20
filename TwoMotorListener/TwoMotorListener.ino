// Upload this sketch to a listener Arduino
// To be used with any speaker that sends two drive bytes, then two direction bytes via I2C

// Include Wire library
#include <Wire.h>

// I2C location | Motor 1 = 8, Motors 2&3 = 9
const int location = 9;

// Initialize drive variables
byte spd1, dir1, spd2, dir2;

// Name motor pins for clarity
const int PWM1pin = 3;
const int PWM2pin = 11;
const int DIR1pin = 12;
const int DIR2pin = 13;

void setup() {
  // Join I2C bus at determined location and register receipt event
  Wire.begin(location);
  Wire.onReceive(receiveEvent);

  // Set pins to output and initialize to low
  pinMode(PWM1pin, OUTPUT);
  pinMode(PWM2pin, OUTPUT);
  pinMode(DIR1pin, OUTPUT);
  pinMode(DIR2pin, OUTPUT);
  digitalWrite(PWM1pin, LOW);
  digitalWrite(PWM2pin, LOW);
  digitalWrite(DIR1pin, LOW);
  digitalWrite(DIR2pin, LOW);
}

void loop() {
  delay(10);
}

// Function that executes whenever data is received via I2C
void receiveEvent(int num) {
  // Read four bytes
  spd1 = Wire.read();
  spd2 = Wire.read();
  dir1 = Wire.read();
  dir2 = Wire.read();

  // Set directions
  if (dir1 == 0) digitalWrite(DIR1pin,LOW);
  else digitalWrite(DIR1pin,HIGH);
  if (dir2 == 0) digitalWrite(DIR2pin,LOW);
  else digitalWrite(DIR2pin,HIGH);

  // Drive motors
  if (spd1 < 80 && spd1 > 0) {
    analogWrite(PWM1pin,255);
    delay(1500);
  }
  analogWrite(PWM1pin,spd1);
  analogWrite(PWM2pin,spd2);
}
