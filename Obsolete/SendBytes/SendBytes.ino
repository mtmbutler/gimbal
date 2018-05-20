/* This sketch is used on a master Arduino to test listener sketches.
 * It sends two random bytes every five seconds to three different listeners.
 */

// Include Wire library
#include <Wire.h>

// Drive variables
int drive, dir;

// I2C location variables
const int motor1 = 8;
const int motor2 = 9;
const int motor3 = 10;

void setup() {
  // Join I2C bus as master
  Wire.begin();
}

void loop() {
  // Randomize drive variables
  drive = random(0, 256);
  dir = random(0, 2);
  
  // Write to motor 1
  Wire.beginTransmission(motor1);
  Wire.write(drive);
  Wire.write(dir);
  Wire.endTransmission();

  // Write to motor 2
  Wire.beginTransmission(motor2);
  Wire.write(drive);
  Wire.write(dir);
  Wire.endTransmission();

  // Write to motor 3
  Wire.beginTransmission(motor3);
  Wire.write(drive);
  Wire.write(dir);
  Wire.endTransmission();

  // Wait 5 seconds
  delay(5000);
}
