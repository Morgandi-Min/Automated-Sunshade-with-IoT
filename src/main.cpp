#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_B_LUX_V30B.h>

// Stepper Motor Pins
#define stepPin 32
#define dirPin 33
#define enablePin 34

// Limit Switch Pins
#define topLimitSwitch 23
#define bottomLimitSwitch 22

// Light Sensor
DFRobot_B_LUX_V30B myLux(15);  // EN pin = 15

// Parameters
const int stepDelay = 40;               // Faster motor speed
const float thresholdLux = 200.0;       // Light threshold
const float maxLux = 200000.0;          // Reject anything above this

// State tracking
enum MotorState { IDLE, MOVING_UP, MOVING_DOWN };
MotorState currentState = IDLE;

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW); // Enable motor driver

  pinMode(topLimitSwitch, INPUT_PULLUP);     // Active LOW
  pinMode(bottomLimitSwitch, INPUT_PULLUP);  // Active LOW

  Serial.begin(9600);
  Wire.begin();
  myLux.begin();

  Serial.println("System Initialized.");
}

void moveStep(bool direction) {
  digitalWrite(dirPin, direction);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(stepDelay);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(stepDelay);
}

void loop() {
  static unsigned long lastMoveTime = 0;
  float lux = myLux.lightStrengthLux();

  // Reject bad sensor readings silently
  if (lux > maxLux) {
    return;
  }

  // Show valid lux value
  Serial.print("Lux: ");
  Serial.println(lux);

  bool topLimit = digitalRead(topLimitSwitch) == LOW;
  bool bottomLimit = digitalRead(bottomLimitSwitch) == LOW;

  // Decide what to do based on light and limit switches
  if (lux > thresholdLux && !topLimit) {
    currentState = MOVING_UP;
  } else if (lux < thresholdLux && !bottomLimit) {
    currentState = MOVING_DOWN;
  } else {
    currentState = IDLE;
  }

  // Motor control logic
  switch (currentState) {
    case MOVING_UP:
      moveStep(HIGH);  // Move up
      if (digitalRead(topLimitSwitch) == LOW) {
        Serial.println("Top limit reached. Stopping.");
        currentState = IDLE;
      }
      break;

    case MOVING_DOWN:
      moveStep(LOW);  // Move down
      if (digitalRead(bottomLimitSwitch) == LOW) {
        Serial.println("Bottom limit reached. Stopping.");
        currentState = IDLE;
      }
      break;

    case IDLE:
      // Do nothing
      break;
  }
}
