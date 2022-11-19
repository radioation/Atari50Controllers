#include "Mouse.h"
#include "Keyboard.h"

const byte DRIVING_PIN_1 = 0;  // 9;
const byte DRIVING_PIN_2 = 1;  // 8;

const byte DRIVING_BUTTON_PIN_6 = 4;

volatile byte drivingPin1State = LOW;
volatile byte lastDrivingPin1State = LOW;
volatile byte drivingPin2State = LOW;
volatile byte lastDrivingPin2State = LOW;
volatile byte currentRotationDir = 0;
volatile byte lastRotationDir = 0;

int drivingCounter = 0; 
volatile int tick = 0;
int magnitude = 10;

// void handleEncoderInterrupt() {
//   drivingPin1State = digitalRead(DRIVING_PIN_1);

//   // check if pulse occured.
//   if (drivingPin1State != lastDrivingPin1State) {  //&& drivingPin1State == 1) {
//     // determine direction
//     if (digitalRead(DRIVING_PIN_2) != drivingPin1State) {  // CCW
//       currentRotationDir = 1;                              // CCW but atari 50 mouse is the opposite?
//     } else {                                               // CW
//       currentRotationDir = -1;
//     }
//   }

//   lastDrivingPin1State = drivingPin1State;
// }

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
  //attachInterrupt(digitalPinToInterrupt(DRIVING_PIN_1), handleEncoderInterrupt, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(DRIVING_PIN_2), handleEncoderInterrupt, CHANGE);

}

void loop() {
  currentRotationDir = 0;
  drivingPin1State = digitalRead(DRIVING_PIN_1);
  drivingPin2State = digitalRead(DRIVING_PIN_2);

  if (drivingPin2State != lastDrivingPin2State) {
    currentRotationDir = (drivingPin2State - lastDrivingPin2State) * (drivingPin1State ? +1 : -1);

  } else if (drivingPin1State != lastDrivingPin1State) {
    currentRotationDir = (drivingPin1State - lastDrivingPin1State) * (drivingPin2State ? -1 : +1);
  }


  if (currentRotationDir != 0) {

    lastDrivingPin1State = drivingPin1State;
    lastDrivingPin2State = drivingPin2State;

    if (currentRotationDir == lastRotationDir) {
      drivingCounter++;
    } else {
      drivingCounter = 1;
    }
 

    Mouse.move(currentRotationDir * min( drivingCounter * magnitude, 127) , 0);

    lastRotationDir = currentRotationDir;
  } else {
    drivingCounter = 1;
  }

  // Atari 50 defaults to spacebar for fire button with mouse controls.
  if (!digitalRead(DRIVING_BUTTON_PIN_6)) {
    Keyboard.press(' ');
  } else {
    Keyboard.release(' ');
  }
}

