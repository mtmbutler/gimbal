// Include Wire library
#include <Wire.h>

byte dir1 = 0;

// Serial interface variables
int input;

// I2C locations
const byte ard1 = 8;

// Sensor variables
volatile long theta; // measured in ninths of a degree

// Input pins for encoders
// The Arduino Mega has 6 interrupt pins, but 2 are used for I2C.
// I chose to use both channels for the largest ring because it's the one
//   most likely to be going in a different direction than the drive value.
const byte CHA1pin = 2;

void setup() {
  // Join I2C bus as master
  Wire.begin();

  // Attach interrupts
  attachInterrupt(digitalPinToInterrupt(CHA1pin), updateStateA1, CHANGE);
  
  // Start serial for output
  Serial.begin(9600);
  
  // Initialize pins to input
  pinMode(CHA1pin, INPUT);

  // Initialize variables
  theta = 0;
  
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
    driveMotor(150, 1);
    delay(10000);
    driveMotor(0, 0);
    printData();
    delay(5000);
  }
  else {
    driveMotor(0, 0);
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
  Serial.println("Theta: " + String(theta*5/9));
}

// Function that sends 2 drive bytes to each motor
void driveMotor(byte s1, byte d1) {
  Wire.beginTransmission(ard1);
  Wire.write(s1);
  Wire.write(d1);
  Wire.endTransmission();
}

// Interrupt Service Routine
void updateStateA1 () {
  if (dir == 1)
    theta += 1;
  else
    theta -= 1;
}
