#include "Mouse.h"
#include "Keyboard.h"

const byte DRIVING_PIN_1 = 0;  // 9;
const byte DRIVING_PIN_2 = 1;  // 8;

const byte DRIVING_BUTTON_PIN_6 = 12;
const byte ledPin = 13;

volatile byte drivingPin1State = LOW;
volatile byte lastDrivingPin1State = LOW;
volatile byte currentRotationDir = 0;
volatile int tick = 0;

void handleEncoderInterrupt() {
  drivingPin1State = digitalRead(DRIVING_PIN_1);

  // check if pulse occured.
  if (drivingPin1State != lastDrivingPin1State ) { //&& drivingPin1State == 1) {
    // determine direction
    if (digitalRead(DRIVING_PIN_2) != drivingPin1State) {  // CCW
      currentRotationDir = 1; // CCW but atari 50 mouse is the opposite?
      
    } else {  // CW
      currentRotationDir = -1;  
    }
  }

  lastDrivingPin1State = drivingPin1State;
}

void setup() {

  // setup pins
  pinMode(DRIVING_PIN_1, INPUT_PULLUP);
  pinMode(DRIVING_PIN_2, INPUT_PULLUP);
  pinMode(DRIVING_BUTTON_PIN_6, INPUT_PULLUP);

  lastDrivingPin1State = digitalRead(DRIVING_PIN_1);
  // initialize Mouse and Keyboard
  Mouse.begin();
  Keyboard.begin();

  // update encoder whenever a change is on either line is seen
  attachInterrupt(digitalPinToInterrupt(DRIVING_PIN_1), handleEncoderInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DRIVING_PIN_2), handleEncoderInterrupt, CHANGE);
}

void loop() {

  if (currentRotationDir != 0) {
    Mouse.move(currentRotationDir * 75, 0);
    currentRotationDir = 0;
  }

  // Atari 50 defaults to spacebar for fire button.
  if (!digitalRead(DRIVING_BUTTON_PIN_6)) {
    Keyboard.press(' ');
  } else {
    Keyboard.release(' ');
  }
}