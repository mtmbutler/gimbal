double phi = 0;
double new_phi = 0;
double theta = 0;
double new_theta = 0;
double psi = 0;
double new_psi = 0;

void setup() {
  // Start serial for output
  Serial.begin(9600);
}

void loop() {
  while (Serial.available() > 0) {
    // Parse serial input for integers to set as drive values
    new_phi = Serial.parseInt();
    new_theta = Serial.parseInt();
    new_psi = Serial.parseInt();
    Serial.println(String(new_phi) + String(new_theta) + String(new_psi));
  }
}
