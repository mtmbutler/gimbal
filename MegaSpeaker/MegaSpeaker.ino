// Include Wire library
#include <Wire.h>

// I2C locations
const int ard1 = 8;
const int ard2 = 9;

// Drive variables
int drive1, drive2, drive3;
byte spd1, spd2, spd3, dir1, dir2, dir3;

void setup() {
  // Join I2C bus as master
  Wire.begin();
  
  // Start serial for output
  Serial.begin(9600);

  // Print instructions
  printInstructions();
  
  // Print "Setup complete!" and a divider
  Serial.println("Setup complete!");
  printDivider();
}

void loop() {
  while (Serial.available() > 0) {
    // Parse serial input for integers to set as drive values
    drive1 = Serial.parseInt();
    drive2 = Serial.parseInt();
    drive3 = Serial.parseInt();
    Serial.println("Received values: " + String(drive1) + ", " + String(drive2) + ", " + String(drive3));
    
    // Executes if there's a line break (serial monitor must be set to 'Newline')
    if (Serial.read() == '\n') {
      // Convert values to safe ones
      drive1 = constrain(drive1, -255, 255);
      drive2 = constrain(drive2, -255, 255);
      drive3 = constrain(drive3, -255, 255);
      Serial.println("Constrained values: " + String(drive1) + ", " + String(drive2) + ", " + String(drive3));

      // Convert drive values into bytes
      spd1 = abs(drive1);
      spd2 = abs(drive2);
      spd3 = abs(drive3);
      dir1 = calcDir(drive1);
      dir2 = calcDir(drive2);
      dir3 = calcDir(drive3);

      // Send drive values
      driveMotors(spd1, spd2, spd3, dir1, dir2, dir3);

      // Print a divider
      printDivider();
    }
  }
}

// Function that prints a divider
void printDivider() {
  Serial.println("---");
}

// Function that tells the user how to run the program
void printInstructions() {
  Serial.println();
  Serial.println("Expected input (as ints): \"drive1, drive2, drive3\"");
  Serial.println("Make sure serial settings are set to \"Newline\" and \"9600 baud\".");
  Serial.println("Example input: \"-255, 255, 80\"");
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
byte calcDir(int driveValue) {
  if (driveValue < 0) return 0;
  else return 1;
}
