#include <Stepper.h>
const int stepsPerRev = 200;
const int smallStepsPerRev = 513;
const int rpm = 30;
const float pi = 3.141592654;

// Drive variables
float phi = 0;
float theta = 0;
float psi = 0;
float new_phi, new_theta, new_psi;
int step1, step2, step3;

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
}

void loop() {
  while (Serial.available() > 0) {
    // Parse serial input for floats to set as new angles
    new_phi = Serial.parseFloat();
    new_theta = Serial.parseFloat();
    new_psi = Serial.parseFloat();
    Serial.println(String(new_phi) + " " + String(new_theta) + " " + String(new_psi));
    
    // Executes if there's a line break (serial monitor must be set to 'Newline')
    if (Serial.read() == '\n') {
      step1 = (new_phi - phi)*stepsPerRev/2/pi;
      step2 = (new_theta - theta)*stepsPerRev/2/pi;
      step3 = (new_psi - psi)*smallStepsPerRev/2/pi;
      Serial.println(String(step1) + " " + String(step2) + " " + String(step3));

    // SMALLEST
      // Drive motors
      int smallest = abs(step1);
      if (abs(step2) < smallest) smallest = abs(step2);
      if (abs(step3) < smallest) smallest = abs(step3);

      int step_size_1 = (int) step1/smallest;
      int step_size_2 = (int) step2/smallest;
      int step_size_3 = (int) step3/smallest;

      Serial.println(String(step_size_1) + " " + String(step_size_2) + " " + String(step_size_3));

      for (int i = 1; i <= smallest; i++) {
        m1.step(step_size_1);
        m2.step(step_size_2);
        m3.step(step_size_3);

        phi = phi + 2*pi*step_size_1/stepsPerRev;
        theta = theta + 2*pi*step_size_2/stepsPerRev;
        psi = psi + 2*pi*step_size_3/smallStepsPerRev;
      }

//      // LARGEST (doesn't work as well as SMALLEST)
//      // Drive motors
//      int largest = abs(step1);
//      if (abs(step2) > largest) largest = abs(step2);
//      if (abs(step3) > largest) largest = abs(step3);
//
//      int step_size_1 = (int) step1/largest;
//      int step_size_2 = (int) step2/largest;
//      int step_size_3 = (int) step3/largest;
//
//      Serial.println(String(step_size_1) + " " + String(step_size_2) + " " + String(step_size_3));
//
//      for (int i = 1; i <= largest; i++) {
//        m1.step(step_size_1);
//        m2.step(step_size_2);
//        m3.step(step_size_3);
//
//        phi = phi + 2*pi*step_size_1/stepsPerRev;
//        theta = theta + 2*pi*step_size_2/stepsPerRev;
//        psi = psi + 2*pi*step_size_3/smallStepsPerRev;
//      }

      Serial.println(String(phi) + " " + String(theta) + " " + String(psi));
      Serial.println("-----------------------------");
    }
  }
}
