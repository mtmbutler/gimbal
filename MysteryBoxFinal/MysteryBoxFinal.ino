/* This program operates the Mystery Box, a teaching tool for quantum
 * physics. The Arduino controls a stepper motor, which turns to move
 * a cart side to side, simulating a particle moving under the influence
 * of a one-dimensional potential. It also blinks a laser at random,
 * stopping the motion after a set number of blinks.
 * 
 * Weston Buchanan
 * Austin Simms
 * Miles Moser
 * 
 * PHYS 350 - Intermediate Mechanics
 * Dr. Gregory Ojakangas
 * Drury University
 * Fall 2016
*/

// Include Stepper library, which has functions to
// turn an arbitrary # of steps and set speed (rpm)
#include <Stepper.h>

const int STEPS = 513; // steps per revolution

// Instantiate a stepper, specifying STEPS and the
// pins it's attached to
Stepper stepper(STEPS, 4, 5, 6, 7);

// Constants
const double L = 1800; // usable length of track (mm/5)
const double dL = 1; // translation caused by one step (mm/5)
const int res = 30; // resolution: # of steps to go
                   // before updating speed
double E = 4900; // Total energy of the system
                 // Chosen to make max rpm 70

// Motion Variables
double U = 0; // Potential energy of the system
double x = L/2; // Current location of the platform
double rpm = 80; // Initial speed of the system
int dir = 1; // 1 = forward, -1 = backward

// Laser Variables
int freq = 5000; // Average interval between blinks in ms
int randNum = 0; // Used to offset frequency and make the blinking random
int interval = 0; // Time in ms until next blink
long lastTime = 0; // Used to remember when the laser last blinked
long currentTime = 0; // Used to check when to blink
int blinkCount = 0; // To keep track of how many times the laser blinks

byte RUN = 1; // The program only runs if this is 1: can terminate program
              // at any time by setting this to 0
int blinkMax = 20; // Number of blinks before program stops

// Setup
void setup() {
  // Start serial for output to computer
  Serial.begin(9600);

  // Set initial motor speed
  stepper.setSpeed(80);

  // Set pin 13 to output for laser
  pinMode(13, OUTPUT);

  // Seed the RNG with a noise reading from unused analog pin
  randomSeed(analogRead(A2));
}

// Loop
void loop() {
  if (RUN == 1){ // Execute only if RUN = 1
    // Move platform, update location and print new location to serial
    stepper.step(dir*res);
    x += dir*dL*res;
    Serial.print("x = ");
    Serial.print(x);

    U = getPotential(x); // Calculate potential at location
    rpm = getRPM(U); // Calculate rpm based on potential
    stepper.setSpeed(rpm); // Give stepper a new speed
    // Make sure we don't go past the bounds on either side:
    // tell the motor to change direction if we get close
    if (dir == 1 && x > L - 3) dir *= -1;
    if (dir == -1 && x < 3) dir *= -1;

    // Get current time and see if the interval has elapsed
    currentTime = millis();
    Serial.print(", Time = ");
    Serial.print(currentTime);
    if (currentTime > lastTime + interval) { // executes if interval has elapsed
      digitalWrite(13, HIGH); // turn laser on
      randNum = random(-4500,4500);
      interval = freq + randNum; // varies the average interval of 5 seconds by up to 4.5 seconds
      Serial.print(", Interval = ");
      Serial.print(interval);
      lastTime = currentTime;
      blinkCount++;
      Serial.print(", Blink count = ");
      Serial.println(blinkCount);
    }
    else {
      digitalWrite(13, LOW); // turn laser off
      Serial.println(); // line break
    }

    // Stop the program and turn off the laser after 20 blinks
    if (blinkCount == 20){
      RUN = 0;
      digitalWrite(13, LOW);
    }
  }
  else delay(5000); // Wait 5 seconds (indefinitely) if RUN = 0
}

// This function calculates potential given a location x - normalize to [0,4500]
double getPotential(double location) {
  //U = -1*location; // Uniform acceleration to right
  //U = location; // Uniform acceleration to left
  U = 4500*pow((location-(L/2)),2)/pow(L/2,2); // Simple harmonic oscillator
  //if (location >= L/3 && location <= 2*L/3) U = 1;
  //else U = 0; // Step potential
  //U = 1; // Uniform motion

  // Print and return U
  Serial.print(", U = ");
  Serial.print(U);
  return U;
}

// This function calculates rpm given a potential
double getRPM(double potential) {
  rpm = sqrt(E - potential); // There are constants in the real equation,
                             // but we're normalizing to the motor

  // Print and return rpm
  Serial.print(", rpm = ");
  Serial.print(rpm);
  if (rpm < 20) rpm = 20;
  else if (rpm > 70) rpm = 70; // Make sure rpm values are safe for the electronics
  return rpm;
}
