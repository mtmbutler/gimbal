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
    //if (Serial.read() == '\n') {
      step1 = (new_phi)*stepsPerRev/2/pi;
      step2 = (new_theta)*stepsPerRev/2/pi;
      step3 = (new_psi)*smallStepsPerRev/2/pi;
      Serial.println(String(step1) + " " + String(step2) + " " + String(step3));

    // SMALLEST
      // Drive motors
      int count1 = step1;
      int count2 = step2;
      int count3 = step3;
      int smallest = abs(step1);
      if (abs(step2) < smallest) smallest = abs(step2);
      if (abs(step3) < smallest) smallest = abs(step3);
      if (smallest == 0) smallest = 1;

      while (count1 < step_size_1 + 1 || count2 < step_size_2 + 1 || count3 < step_size_3 + 1) {
        int step_size_1 = count1/smallest;
        if (count1 > 0 && step_size_1 == 0) step_size_1 = 1;
        int step_size_2 = count2/smallest;
        if (count2 > 0 && step_size_2 == 0) step_size_2 = 1;
        int step_size_3 = count3/smallest;
        if (count3 > 0 && step_size_3 == 0) step_size_3 = 1;
  
        constrain(step_size_1, 0, 10);
        constrain(step_size_2, 0, 10);
        constrain(step_size_3, 0, 10);
  
        for (int i = 1; i <= smallest; i++) {
          m1.step(step_size_1);
          count1 = count1 - step_size_1;
          m2.step(step_size_2);
          count2 = count2 - step_size_2;
          m3.step(step_size_3);
          count3 = count3 - step_size_3;
        }
      }
      Serial.println("-----------------------------");
  }
}
