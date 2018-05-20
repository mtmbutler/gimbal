#include <Stepper.h>

const int STEPS = 513;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(STEPS, 4, 5, 6, 7);

int stepCount = 0;         // number of steps the motor has taken

void setup() {
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  myStepper.setSpeed(20);
  myStepper.step(-1*STEPS);
  delay(1000);

  myStepper.setSpeed(30);
  myStepper.step(STEPS);
  delay(1000);

  myStepper.setSpeed(40);
  myStepper.step(-1*STEPS);
  delay(1000);

  myStepper.setSpeed(50);
  myStepper.step(STEPS);
  delay(1000);

  myStepper.setSpeed(60);
  myStepper.step(-1*STEPS);
  delay(1000);
}
