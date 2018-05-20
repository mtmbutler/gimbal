// Include Wire library
#include <Wire.h>

byte dir1 = 0;
byte dir2 = 0;
byte dir3 = 0;

// Serial interface variables
int input;

// I2C locations
const byte ard1 = 8;
const byte ard2 = 9;

// Sensor variables
volatile long theta, phi, psi; // measured in ninths of a degree

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
  
  // Print instructions, "Setup complete!" and a divider
  printInstructions();
  Serial.println("Setup complete!");
  printDivider();
}

void loop() {
  if (Serial.available() > 0)
    input = Serial.parseInt();
  if (input != 0) {    
    // Start motors
    driveMotors(150, 150, 150, 1, 0, 0);
    delay(10000);
    driveMotors(0, 0, 0, 0, 0, 0);
    printData();
    delay(5000);
  }
  else {
    driveMotors(0, 0, 0, 0, 0, 0);
    delay(1000);
  }
}

// Function that prints a divider
void printDivider() {
  Serial.println("---");
}

// Function that tells the user how to run the program
void printInstructions() {
  Serial.println();
  Serial.println("Send a number to run the program; send 0 to stop.");
}

void printData() {
  printDivider();
  Serial.println("Theta: " + String(theta/9));
  Serial.println("Phi: " + String(phi/9));
  Serial.println("Psi: " + String(psi/9));
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

// Interrupt Service Routines
// A states update position, B states update direction of motion

// Motor 1 ISRs
void updateStateA1 () {
  if (dir1 == 1)
    theta += 5;
  else
    theta -= 5;
}

// Motor 2 ISR
void updateStateA2 () {
  if (dir2 == 1)
    phi += 5;
  else
    phi -= 5;
}

// Motor 3 ISR
void updateStateA3 () {
  if (dir3 == 1)
    psi += 5;
  else
    psi -= 5;
}
