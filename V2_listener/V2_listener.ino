// Upload this sketch to a listener Arduino
// To be used with any speaker that sends an rpm byte, then a steps byte via I2C

// Include Wire and Stepper libraries
#include <Wire.h>
#include <Stepper.h>

// I2C location | Motor 1 = 8, Motor 2 = 9, Motor 3 = 10
const int location = 8;

// Steps per rev | Motors 1, 2 = 200, Motor 3 = 513
const int stepsPerRev = 200;

// Initialize drive variables and stepper
byte spd, steps;
Stepper motor(stepsPerRev, 2, 3, 4, 5);

void setup() {
  // Join I2C bus at determined location and register receipt event
  Wire.begin(location);
  Wire.onReceive(receiveEvent);

  // Set motor speed
  motor.setSpeed(30);
}

void loop() {
  delay(10);
}

// Function that executes whenever data is received via I2C
void receiveEvent(int num) {
  // Read two bytes
  spd = Wire.read();
  steps = Wire.read();

  // Drive motor
  motor.setSpeed(constrain(spd, 0, 60));
  motor.step(steps);
}
