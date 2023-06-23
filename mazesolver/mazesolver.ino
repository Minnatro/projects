#define fthold 12
#define rthold 20
#define lthold 20
const int t = 1050;
int tfr = 2750;
int timefr;
int tibef = 440;
int tlaf = 1150;
bool found = false;
int nf = 0;
int nlr = 0;
int dir[100];
int i = 1;
int j = 0;
const int irPinF = A0; // IR distance sensor pin for front
const int irPinR = A1; // IR distance sensor pin for right
const int irPinL = A2; // IR distance sensor pin for left

// Function to measure distance using IR distance sensor
float measureDistance(int irPin) {
  int sensorValue = analogRead(irPin);

  // Convert analog reading to distance using sensor-specific formula
  float distance = convertToDistance(sensorValue);

  return distance;
}

// Function to convert analog reading to distance based on sensor characteristics
float convertToDistance(int sensorValue) {
  // Sensor calibration values
  const float voltsPerUnit = 5.0 / 1024;  // Convert analog reading to voltage
  const float sensorVoltage = sensorValue * voltsPerUnit;

  // Sensor-specific formula for distance calculation
  // This formula is an example, adjust it based on your sensor's characteristics
  float distance = 27.86 * pow(sensorVoltage, -1.15);

  return distance;
}

// Moves the robot in the forward direction
void goFront() {
  // Moves forward adjusting its path
  float idist1 = measureDistance(irPinL);
  float lconst = idist1;

  while (idist1 <= 5) {
    // Should turn a little to its right
    digitalWrite(10, HIGH);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    delay(t / 65);

    idist1 = measureDistance(irPinL);

    if (abs(lconst - idist1) >= 0.8 || (idist1 >= 13.6)) {
      break;
    }
  }

  float rdist1 = measureDistance(irPinR);
  float rconst = rdist1;

  while (rdist1 <= 5.4) {
    // Should turn a little to its left
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);

    delay(t / 65);

    rdist1 = measureDistance(irPinR);
    if (abs(rconst - rdist1) >= 0.9) {
      break;
    }
  }

  if (measureDistance(irPinL) >= 7.2) {
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
    delay(t / 30);
  }

  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
}

// Returns the distance of the wall in front of the robot
float frontDist() {
  return measureDistance(irPinF);
}

// Returns the distance of the wall on the right side of the robot
float rightDist() {
  return measureDistance(irPinR);
}

// Returns the distance of the wall on the left side of the robot
float leftDist() {
  return measureDistance(irPinL);
}

void setup() {
  Serial.begin(9600);

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  // Move forward if no obstacle detected
  if (frontDist() >= fthold) {
    goFront();
  }

  // If there is an obstacle in front
  else {
    // Check right and left distances
    float distarr[5] = {rightDist(), leftDist(), rightDist(), leftDist(), frontDist()};
    float midd = middleVal(distarr);

    // Turn left if left distance is clear
    if (midd == distarr[1] && midd >= lthold) {
      turnLeft();
      delay(500);  // Adjust this delay based on your robot's turning speed
    }

    // Turn right if right distance is clear
    else if (midd == distarr[0] && midd >= rthold) {
      turnRight();
      delay(500);  // Adjust this delay based on your robot's turning speed
    }

    // If both left and right distances are blocked or below threshold,
    // turn around by 180 degrees and continue moving forward
    else {
      turnRight();
      delay(1000);  // Adjust this delay based on your robot's turning speed
    }
  }
}

// Finds the middle value in an array of floats
float middleVal(float arr[]) {
  float middle = arr[0];
  for (int i = 1; i < 5; i++) {
    if (arr[i] < middle)
      middle = arr[i];
  }
  return middle;
}

// Turns the robot to the left
void turnLeft() {
  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
}

// Turns the robot to the right
void turnRight() {
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
}
