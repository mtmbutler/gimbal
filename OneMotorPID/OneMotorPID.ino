// Include Wire library
#include <Wire.h>

// Serial interface variables
int input;

// I2C location
const byte ard1 = 8;

// Ratio for angle conversion
const double angleRatio = 5/9; // multiply an angle variable by this constant to get the angle in degrees

// Drive variables
int drive;
byte spd, dir;

// Sensor variables
volatile long theta; // measured in 1.8-degree increments

// PID variables
// to be tested
/*----------------*/
const float kP = 0.5;
const float kI = 0;
const float kD = 0;
/*----------------*/
float P, I, D;
float setPoint;
float error, errorPrev;
float integral, derivative;
unsigned long t = 0;
unsigned long tPrev = 0;

// Input pin for encoder
const byte CHA1pin = 2;

void setup() {
  // Join I2C bus as master
  Wire.begin();

  // Attach interrupt
  attachInterrupt(digitalPinToInterrupt(CHA1pin), updateStateA1, CHANGE);
  
  // Start serial for output
  Serial.begin(9600);
  
  // Initialize pin to input
  pinMode(CHA1pin, INPUT);

  // Initialize variables
  theta = 0;
  dir = 1;
  
  // Print instructions, "Setup complete!" and a divider
  printInstructions();
  Serial.println("Setup complete!");
  printDivider();
}

void loop() {
  if (Serial.available() > 0)
    input = Serial.parseInt();
  if (input != 0) {    
    // Define target angle
    setPoint = 90;

    // Use PID function to calculate drive value
    drive = calcDrive();

    // Constrain drive value
    drive = constrain(drive, -255, 255);

    // Convert drive value into bytes
    spd = abs(drive);
    dir = calcDir(drive);

    // Send drive values
    driveMotor(spd, dir);
  }
  else {
    driveMotor(0, 0);
    error = 0;
    errorPrev = 0;
    integral = 0;
    derivative = 0;
    t = 0;
    tPrev = 0;
    delay(1000);
  }
}

/*======================================================================================*
 *                                   PID FUNCTION                                       *
 *======================================================================================*/
 
// Motor 1
int calcDrive() {
  printDivider();
  Serial.println(digitalRead(CHA1pin));
  
  // Assign time
  tPrev = t;
  t = millis();
  Serial.println("Time: " + String(t));

  // Calculate error
  Serial.println("Theta: " + String(theta*angleRatio));
  errorPrev = error;
  cli();
  error = setPoint - theta*angleRatio;
  sei();

  // Add error to integral term
  integral += error*(t - tPrev);

  // Calculate derivative term
  derivative = (error - errorPrev)/(t - tPrev);

  // Calculate drive
  P = kP*error;
  I = kI*integral;
  D = kD*derivative;
  int value = P + I + D;

  // Print data and return drive value
  Serial.println("P: " + String(P) + ", I: " + String(I) + ", D: " + String(D));
  Serial.println("Drive: " + String(value));
  return value;
}

/*======================================================================================*
 *                                  END PID FUNCTION                                    *
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

// Function that drives the motor via I2C
void driveMotor(byte s1, byte d1) {
  Wire.beginTransmission(ard1);
  Wire.write(s1);
  Wire.write(d1);
  Wire.endTransmission();
}

// Function that returns 1 or 0 for direction given a drive value
int calcDir(int driveValue) {
  if (driveValue < 0) return 0;
  else return 1;
}

// Interrupt Service Routine
void updateStateA1 () {
  if (dir == 1)
    theta += 1;
  else
    theta -= 1;
}
