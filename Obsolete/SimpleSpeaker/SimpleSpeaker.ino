/* This program expects serial input of the following form:
 * #,#,#
 * where the three numbers are the respective drive values for the motors.
 * A negative number will change the direction of the motor.
 */

// Include Wire library
#include <Wire.h>

// Declare variables
int drive1, drive2, drive3;
byte spd3, dir3;

// Name motor pins for clarity
const int PWM1pin = 3;
const int PWM2pin = 11;
const int DIR1pin = 12;
const int DIR2pin = 13;

void setup() {
  // Join I2C bus as master
  Wire.begin();

  // Start serial for output
  Serial.begin(9600);
  
  // Set pins to output and initialize to low
  pinMode(PWM1pin, OUTPUT);
  pinMode(PWM2pin, OUTPUT);
  pinMode(DIR1pin, OUTPUT);
  pinMode(DIR2pin, OUTPUT);
  digitalWrite(PWM1pin, LOW);
  digitalWrite(PWM2pin, LOW);
  digitalWrite(DIR1pin, LOW);
  digitalWrite(DIR2pin, LOW);

  // Give instructions
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
    Serial.println("Received:    " + String(drive1) + ", " + String(drive2) + ", " + String(drive3));
    
    // Executes if there's a line break (serial monitor must be set to 'Newline')
    if (Serial.read() == '\n') {
      // Convert values to safe ones
      drive1 = constrain(drive1, -255, 255);
      drive2 = constrain(drive2, -255, 255);
      drive3 = constrain(drive3, -255, 255);
      Serial.println("Constrained: " + String(drive1) + ", " + String(drive2) + ", " + String(drive3));

      // Set direction of each motor
      if (drive1 < 0) digitalWrite(DIR1pin, LOW);
      else digitalWrite(DIR1pin, HIGH);
      if (drive2 < 0) digitalWrite(DIR2pin, LOW);
      else digitalWrite(DIR2pin, HIGH);

      // Convert drive3 into two bytes for Ard2
      if (drive3 < 0) dir3 = 0;
      else dir3 = 1;
      spd3 = abs(drive3);

      // Drive motors 1 and 2
      analogWrite(PWM1pin, abs(drive1));
      analogWrite(PWM2pin, abs(drive2));

      // Send drive3 to Ard2
      Wire.beginTransmission(8);
      Wire.write(spd3);
      Wire.write(dir3);
      Wire.endTransmission();

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
  Serial.println("Drive values should be between -255 and 255.");
  Serial.println("Make sure serial settings are set to \"Newline\" and \"9600 baud\".");
  Serial.println("Example input: \"-255, 255, 80\"");
  printDivider();
}

