// Include Wire library
#include <Wire.h>

// Serial interface variables
int input;

// I2C locations
const byte ard1 = 8;
const byte ard2 = 9;

// Ratio for RPM calculation
const double ratio = 1250000/13;

// Ratio for angle conversion
const double angleRatio = 5/9; // multiply an angle variable by this constant to get the angle in degrees

// Drive variables
int drive1, drive2, drive3;
byte spd1, spd2, spd3, dir1, dir2, dir3;

// Sensor variables
volatile long theta, phi, psi; // measured in 1.8-degree increments

// PID variables
// to be tested
/*----------------*/
const float kP1 = 0.5;
const float kP2 = 0.5;
const float kP3 = 0.5;
const float kI1 = 0;
const float kI2 = 0;
const float kI3 = 0;
const float kD1 = 0;
const float kD2 = 0;
const float kD3 = 0;
/*----------------*/
float P1, P2, P3, I1, I2, I3, D1, D2, D3;
float setTheta, setPhi, setPsi;
float error1, errorPrev1, error2, errorPrev2, error3, errorPrev3;
float integral1, integral2, integral3, derivative1, derivative2, derivative3;
unsigned long t1 = 0;
unsigned long t2 = 0;
unsigned long t3 = 0;
unsigned long tPrev1 = 0;
unsigned long tPrev2 = 0;
unsigned long tPrev3 = 0;

// Input pins for encoders
// The Arduino Mega has 6 interrupt pins, but 2 are used for I2C.
// I chose to use both channels for the largest ring because it's the one
//   most likely to be going in a different direction than the drive value.
const byte CHA1pin = 2;
const byte CHA2pin = 18;
const byte CHA3pin = 19;

void setup() {
  // Join I2C bus as master
  Wire.begin();

  // Attach interrupts
  attachInterrupt(digitalPinToInterrupt(CHA1pin), updateStateA1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHA2pin), updateStateA2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHA3pin), updateStateA3, CHANGE);
  
  // Start serial for output
  Serial.begin(9600);
  
  // Initialize pins to input
  pinMode(CHA1pin, INPUT);
  pinMode(CHA2pin, INPUT);
  pinMode(CHA3pin, INPUT);

  // Initialize variables
  theta = 0;
  phi = 0;
  psi = 0;
  dir1 = 1;
  dir2 = 1;
  dir3 = 1;
  
  // Print instructions, "Setup complete!" and a divider
  printInstructions();
  Serial.println("Setup complete!");
  printDivider();
}

void loop() {
  if (Serial.available() > 0)
    input = Serial.parseInt();
  if (input != 0) {    
    // Define target angles
    setTheta = 90;
    setPhi = 0;
    setPsi = 0;

    // Use PID function to calculate drive values
    drive1 = calcDrive1();
    drive2 = calcDrive2();
    drive3 = calcDrive3();

    // Constrain drive values
    drive1 = constrain(drive1, -255, 255);
    drive2 = constrain(drive2, -255, 255);
    drive3 = constrain(drive3, -255, 255);

    // Convert drive values into bytes
    spd1 = abs(drive1);
    spd2 = abs(drive2);
    spd3 = abs(drive3);
    dir1 = calcDir(drive1);
    dir2 = calcDir(drive2);
    dir3 = calcDir(drive3);

    // Send drive values
    driveMotors(spd1, spd2, spd3, dir1, dir2, dir3);
  }
  else {
    driveMotors(0, 0, 0, 0, 0, 0);
    setTheta = 0;
    setPhi = 0;
    setPsi = 0;
    error1 = 0;
    errorPrev1 = 0;
    error2 = 0;
    errorPrev2 = 0;
    error3 = 0;
    errorPrev3 = 0;
    integral1 = 0;
    integral2 = 0;
    integral3 = 0;
    derivative1 = 0;
    derivative2 = 0;
    derivative3 = 0;
    t1 = 0;
    t2 = 0;
    t3 = 0;
    tPrev1 = 0;
    tPrev2 = 0;
    tPrev3 = 0;
    delay(1000);
  }
}

/*======================================================================================*
 *                                   PID FUNCTIONS                                      *
 *======================================================================================*/
 
// Motor 1
int calcDrive1() {
  // Assign time
  tPrev1 = t1;
  t1 = millis();
  Serial.println("Time: " + String(t1));

  // Calculate error
  Serial.println("Theta: " + String(theta*angleRatio));
  errorPrev1 = error1;
  error1 = setTheta - theta*angleRatio;

  // Add error to integral term
  integral1 += error1*(t1 - tPrev1);

  // Calculate derivative term
  derivative1 = (error1 - errorPrev1)/(t1 - tPrev1);

  // Calculate drive
  P1 = kP1*error1;
  I1 = kI1*integral1;
  D1 = kD1*derivative1;
  int drive = P1 + I1 + D1;

  // Print data and return drive value
  Serial.println("P1: " + String(P1) + ", I1: " + String(I1) + ", D1: " + String(D1));
  Serial.println("Drive1: " + String(drive));
  return drive;
}

// Motor 2
int calcDrive2() {
  // Assign time
  tPrev2 = t2;
  t2 = millis();
  Serial.println("Time: " + String(t2));

  // Calculate error
  errorPrev2 = error2;
  error2 = setPhi - phi*angleRatio;

  // Add error to integral term
  integral2 += error2*(t2 - tPrev2);

  // Calculate derivative term
  derivative2 = (error2 - errorPrev2)/(t2 - tPrev2);

  // Calculate drive
  P2 = kP2*error2;
  I2 = kI2*integral2;
  D2 = kD2*derivative2;
  int drive = P2 + I2 + D2;

  // Print data and return drive value
  Serial.println("P2: " + String(P2) + ", I2: " + String(I2) + ", D2: " + String(D2));
  Serial.println("Drive2: " + String(drive));
  return drive;
}

// Motor 3
int calcDrive3() {
  // Assign time
  tPrev3 = t3;
  t3 = millis();
  Serial.println("Time: " + String(t3));

  // Calculate error
  errorPrev3 = error3;
  error3 = setPsi - psi*angleRatio;

  // Add error to integral term
  integral3 += error3*(t3 - tPrev3);

  // Calculate derivative term
  derivative3 = (error3 - errorPrev3)/(t3 - tPrev3);

  // Calculate drive
  P3 = kP3*error3;
  I3 = kI3*integral3;
  D3 = kD3*derivative3;
  int drive = P3 + I3 + D3;

  // Print data and return drive value
  Serial.println("P3: " + String(P3) + ", I3: " + String(I3) + ", D3: " + String(D3));
  Serial.println("Drive3: " + String(drive));
  printDivider();
  return drive;
}

/*======================================================================================*
 *                                  END PID FUNCTIONS                                   *
 *======================================================================================*/

// Function that prints a divider
void printDivider() {
  Serial.println("---");
}

// Function that tells the user how to run the program
void printInstructions() {
  Serial.println();
  Serial.println("Send any character to run the program; send 0 to stop.");
}

// Function that sends 2 drive bytes to each motor
void driveMotors(byte s1, byte s2, byte s3, byte d1, byte d2, byte d3) {
  Wire.beginTransmission(ard1);
  Wire.write(s1);
  Wire.write(d1);
  Wire.endTransmission();
  Wire.beginTransmission(ard2);
  Wire.write(s2);
  Wire.write(s3);
  Wire.write(d2);
  Wire.write(d3);
  Wire.endTransmission();
}

// Function that returns 1 or 0 for direction given a drive value
int calcDir(int driveValue) {
  if (driveValue < 0) return 0;
  else return 1;
}

// UNUSED
// Function that calculates the current RPM of a given motor
int getRPM(int motorNum) {
  // Declare variables
  double pulseWidth;
  int rpm;
  int pinNum;

  // Determine which pin to read
  if (motorNum == 1)
    pinNum = CHA1pin;
  else if (motorNum == 2)
    pinNum = CHA2pin;
  else
    pinNum = CHA3pin;
  
  // Read pulse width
  pulseWidth = pulseIn(pinNum, HIGH);

  // Calculate RPM from pulse width
  if (pulseWidth != 0)
    rpm = ratio/pulseWidth;
  else
    rpm = 0;

  // Return value
  return rpm;
}

// Interrupt Service Routines

// Motor 1 ISR
void updateStateA1 () {
  if (dir1 == 1)
    theta += 1;
  else
    theta -= 1;
}

// Motor 2 ISR
void updateStateA2 () {
  if (dir2 == 1)
    phi += 1;
  else
    phi -= 1;
}

// Motor 3 ISR
void updateStateA3 () {
  if (dir3 == 1)
    psi += 1;
  else
    psi -= 1;
}
