#include <Stepper.h>
const int stepsPerRev = 200;
const int smallStepsPerRev = 513;
const int rpm = 40;

// Drive variables
int drive1, drive2, drive3;

// Motors
Stepper m1(stepsPerRev, 2, 3, 4, 5);
Stepper m2(stepsPerRev, 6, 7, 8, 9);
Stepper m3(smallStepsPerRev, 10, 11, 12, 13);

void setup() {
  // Start serial for output
  Serial.begin(9600);

  // Set motor speeds
  m1.setSpeed(rpm);
  m2.setSpeed(rpm);
  m3.setSpeed(rpm);
  
  // Print instructions, "Setup complete!" and a divider
  Serial.println("Setup complete!");
}

void loop() {
      m1.step(400);
      delay(1000);
}

