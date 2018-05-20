/* Declare variables */
int drive1 = 0;
int driveLimit = 0;
int go = 0;

// Ratio for RPM calculation
const double ratio = 1250000/13;

// PID variables
float P, I, D;
float kP = 0.95;
float kI = 0.02;
float kD = 0.4;
float targetRPM = 0;
float currentRPM = 0;
float error = 0;
float errorPrev = 0;
float integral = 0;
float derivative = 0;
unsigned long t = 0;
unsigned long tPrev = 0;

// Name motor pins for clarity
const int PWM1pin = 3;
const int DIR1pin = 12;
const int CHA1pin = 4;
const int CHB1pin = 5;

void setup() {
  // Start serial for output
  Serial.begin(9600);
  
  // Set pins to output and initialize to low
  pinMode(PWM1pin, OUTPUT);
  pinMode(DIR1pin, OUTPUT);
  digitalWrite(PWM1pin, LOW);
  digitalWrite(DIR1pin, LOW);

  // Print "Setup complete!" and a divider
  Serial.println("Setup complete!");
  printDivider();
}

void loop() {
  //Serial.println(go);
  if (Serial.available() > 0)
    go = Serial.parseInt();

  if (go != 0) {
    Serial.println();
    
    // Define setpoint function
    //targetRPM = 25*sin(2*3.14*(millis())/5000) + 75; // Simple oscillations
    targetRPM = 80;
    Serial.println("Setpoint: " + String(targetRPM));
    driveLimit = map(targetRPM, 50, 104, 60, 255);
    //driveLimit = 255;

    // Get current RPM
    currentRPM = getRPM();
    Serial.println("Current RPM: " + String(currentRPM));

    // Use PID function to calculate drive value
    Serial.println("Current drive: " + String(drive1));
    drive1 += adjustDrive(targetRPM, currentRPM);
    
    // Constrain drive value
    drive1 = constrain(drive1, -255, driveLimit);
    if (drive1 < 60) drive1 = 60;
    Serial.println("Constrained drive: " + String(drive1));

    // Set motor direction
    if (drive1 < 0) digitalWrite(DIR1pin, LOW);
    else digitalWrite(DIR1pin, HIGH);

    // Drive motor
    analogWrite(PWM1pin, abs(drive1));
  }
  else {
    targetRPM = 0;
    currentRPM = 0;
    error = 0;
    errorPrev = 0;
    integral = 0;
    derivative = 0;
    t = 0;
    tPrev = 0;
    analogWrite(PWM1pin, 0);
    delay(500);
  }
}

// Function that prints a divider
void printDivider() {
  Serial.println("---");
}

// Function that calculates the current RPM of a given motor
int getRPM() {
  // Declare variables
  double pulseWidth;
  int rpm;

  // Read pulse width
  pulseWidth = pulseIn(CHA1pin, HIGH);

  // Calculate RPM from pulse width
  if (pulseWidth != 0)
    rpm = ratio/pulseWidth;
  else
    rpm = 0;

  // Return value
  return rpm;
}

// PID function that takes target and current RPM as parameters
int adjustDrive(int target, int current) {
  tPrev = t;
  t = millis();
  Serial.println("Time: " + String(t));
  
  // Calculate error
  errorPrev = error;
  error = target - current;

  // Add error to integral term
  integral += error;

  // Calculate derivative term
  derivative = error - errorPrev;

  // Calculate drive
  P = kP*error;
  I = kI*integral;
  D = kD*derivative;
  int drive = P + I + D;
  Serial.println("P: " + String(P) + ", I: " + String(I) + ", D: " + String(D));
  Serial.println("Drive adjustment: " + String(drive));
  return constrain(drive, -255, 255);
}
