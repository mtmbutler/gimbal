// Upload this sketch to a listener Arduino
// To be used with any speaker that sends a drive byte, then a direction byte via I2C

// Include Wire library
#include <Wire.h>

// I2C location | Motor 1 = 8, Motors 2&3 = 9
const int location = 8;

// Initialize drive variables
byte spd, dir;

// Name motor pins for clarity
const int PWMpin = 11; //3 or 11
const int DIRpin = 13; //12 or 13

void setup() {
  // Join I2C bus at determined location and register receipt event
  Wire.begin(location);
  Wire.onReceive(receiveEvent);

  // Set pins to output and initialize to low
  pinMode(PWMpin, OUTPUT);
  pinMode(DIRpin, OUTPUT);
  digitalWrite(PWMpin, LOW);
  digitalWrite(DIRpin, LOW);
}

void loop() {
  delay(10);
}

// Function that executes whenever data is received via I2C
void receiveEvent(int num) {
  // Read two bytes
  spd = Wire.read();
  dir = Wire.read();

  // Set direction
  if (dir == 0) digitalWrite(DIRpin,LOW);
  else digitalWrite(DIRpin,HIGH);

  // Drive motor
  if (spd < 80 && spd > 0) {
    analogWrite(PWMpin,255);
    delay(1500);
  }
  analogWrite(PWMpin,spd);
}
