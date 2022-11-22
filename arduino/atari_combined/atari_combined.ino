#include <Joystick.h>
#include "Mouse.h"
#include "Keyboard.h"

// Paddle I/O Pins
const byte PADDLE_1_CAP_PIN_9 = 10;  // read from cap
const byte PADDLE_2_CAP_PIN_5 = 11;
const byte PADDLE_1_POT_PIN_9 = 12;  // discharge at resistor
const byte PADDLE_2_POT_PIN_5 = 13;
const byte PADDLE_1_BTN_PIN_3 = 2;
const byte PADDLE_2_BTN_PIN_4 = 3;
const byte PADDLES_ACTIVE = 9;  // use this to turn switch on/off
// paddle values
unsigned long startTime;
unsigned long elapsedTime;
float paddle1 = 0;
float paddle2 = 0;

int lastButtonState1 = 0;
int lastButtonState2 = 0;

const float paddle1min = 0.66f;
const float paddle2min = 0.66f;

// Driving I/O Pins
const byte DRIVING_PIN_1 = 0;         // also used by trackball controller
const byte DRIVING_PIN_2 = 1;         // also used by trackball controller
const byte DRIVING_BUTTON_PIN_6 = 4;  // also used by trackball controller
// driving values
byte drivingPin1State = LOW;
byte lastDrivingPin1State = LOW;
byte drivingPin2State = LOW;
byte lastDrivingPin2State = LOW;
byte currentRotationDir = 0;
byte lastRotationDir = 0;

int drivingCounter = 0;
int drivingScale = 10;


// Trackball I/O Pins
const byte TRACKBALL_PIN_1 = 0;  // also used by driving controller
const byte TRACKBALL_PIN_2 = 1;  // also used by driving controller
const byte TRACKBALL_PIN_3 = 2;
const byte TRACKBALL_PIN_4 = 3;
const byte TRACKBALL_BUTTON_PIN_6 = 4;  // also used by driving controller
// trackball values
const int trackballScaleX = 7;
const int trackballScaleY = 7;

int trackballDirectionX = LOW;
int trackballMotionX = LOW;
int trackballDirectionY = LOW;
int trackballMotionY = LOW;


// general values
enum CONTROLLER_TYPE { NOT_FOUND,
                       PADDLE_CTRL,
                       DRIVING_CTRL,
                       TRACKBALL_CTRL };
CONTROLLER_TYPE controllerType = NOT_FOUND;

// setup joystick
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_JOYSTICK,
                   2, 0,                  // Button Count, Hat Switch Count
                   true, true, false,     // X and Y, but no Z Axis
                   false, false, false,   // No Rx, Ry, or Rz
                   false, false,          // No rudder or throttle
                   false, false, false);  // No accelerator, brake, or steering

Joystick_ Joystick2(JOYSTICK_DEFAULT_REPORT_ID + 1,
                   JOYSTICK_TYPE_JOYSTICK,
                   2, 0,                  // Button Count, Hat Switch Count
                   true, true, false,     // X and Y, but no Z Axis
                   false, false, false,   // No Rx, Ry, or Rz
                   false, false,          // No rudder or throttle
                   false, false, false);  // No accelerator, brake, or steering

void setup() { 
  // setup paddles
  pinMode(PADDLE_1_CAP_PIN_9, INPUT);
  pinMode(PADDLE_2_CAP_PIN_5, INPUT);
  pinMode(PADDLE_1_POT_PIN_9, OUTPUT);
  pinMode(PADDLE_2_POT_PIN_5, OUTPUT);
  pinMode(PADDLE_1_BTN_PIN_3, INPUT_PULLUP);
  pinMode(PADDLE_2_BTN_PIN_4, INPUT_PULLUP);
  pinMode(PADDLES_ACTIVE, INPUT_PULLUP);

  // setup driving and trackball pins
  pinMode(DRIVING_PIN_1, INPUT_PULLUP);
  pinMode(DRIVING_PIN_2, INPUT_PULLUP);
  pinMode(TRACKBALL_PIN_3, INPUT_PULLUP);
  pinMode(TRACKBALL_PIN_4, INPUT_PULLUP);
  pinMode(DRIVING_BUTTON_PIN_6, INPUT_PULLUP);

  // Initialize Joystick Library
  Joystick.begin();
  Joystick.setXAxisRange(-127, 127);
  Joystick.setYAxisRange(-127, 127);
  Joystick.setXAxis(0.0f);
  Joystick2.begin();
  Joystick2.setXAxisRange(-127, 127);
  Joystick2.setYAxisRange(-127, 127);
  Joystick2.setXAxis(0.0f);

  // initialize Mouse and Keyboard
  Mouse.begin();
  Keyboard.begin();
}

void detectControllers() {

  // just need to check one paddle for decent value
  pinMode(PADDLE_1_POT_PIN_9, OUTPUT);
  digitalWrite(PADDLE_1_POT_PIN_9, LOW);
  delay(5);
  pinMode(PADDLE_1_POT_PIN_9, INPUT);
  auto isPaddle = digitalRead(PADDLE_1_CAP_PIN_9);
  delay(15);
  if (isPaddle == HIGH) {
    controllerType = PADDLE_CTRL;
    return;
  }

  // trackball and driving controllers can both be UP|DOWN at the same time/
  // Read all 4 pins a whole bunch of times, if LEFT|RIGHT even occurse once,
  // it's a trackball.
  for (int i = 0; i < 32767; ++i) {  // 32767 might be excessive.
    auto pin1 = digitalRead(DRIVING_PIN_1);
    auto pin2 = digitalRead(DRIVING_PIN_2);
    auto pin3 = digitalRead(TRACKBALL_PIN_3);
    auto pin4 = digitalRead(TRACKBALL_PIN_4);
    if (pin4 == LOW && pin3 == LOW) {  // left/right is trackball ctr
      controllerType = TRACKBALL_CTRL;
      return;  // exit out, it's a trackball.
    }
    if (pin1 == LOW && pin2 == LOW) {  // could be driving or trackball, but go with DRIVING 
      controllerType = DRIVING_CTRL; 
    }
  }
}

void updatePaddles() {
  for (int i = 0; i < 2; ++i) {
    pinMode(PADDLE_1_POT_PIN_9 + i, OUTPUT);    // set discharge pin to output
    digitalWrite(PADDLE_1_POT_PIN_9 + i, LOW);  // set discharge pin LOW
    delay(4);                                   // 1 seemed sufficient discharge time in tests but 4 should sufficient the hell out of it.
    pinMode(PADDLE_1_POT_PIN_9 + i, INPUT);
    startTime = micros();
    while (digitalRead(PADDLE_1_CAP_PIN_9 + i) < 1)
      ;
    if (i == 0) {
      paddle1 = micros() - startTime;
    } else {
      paddle2 = micros() - startTime;
    }
  }
  // scale and translate paddle readings.  (This could still use a proper calibration step)
  float joy1_x = -(127.0f * 2.0f * (paddle1 / 9000.0f) - 127.0f);
  Joystick.setXAxis(joy1_x);
  float joy2_x = -(127.0f * 2.0f * (paddle2 / 9000.0f) - 127.0f);
  Joystick2.setXAxis(joy2_x);

  // Set buttons
  int currentButtonState = digitalRead(PADDLE_1_BTN_PIN_3);
  if (currentButtonState != lastButtonState1) {
    Joystick.setButton(0, currentButtonState);
    lastButtonState1 = currentButtonState;
  }

  currentButtonState = digitalRead(PADDLE_2_BTN_PIN_4);
  if (currentButtonState != lastButtonState2) {
    Joystick2.setButton(0, currentButtonState);
    lastButtonState2 = currentButtonState;
  }
}

void updateTrackball() {
  // logic from http://jeffpiepmeier.blogspot.com/2016/08/vintage-atari-cx22-trak-ball-with-usb.html
  int xold = trackballMotionX;
  int yold = trackballMotionY;

  trackballDirectionX = digitalRead(TRACKBALL_PIN_1) * 2 - 1;
  trackballMotionX = digitalRead(TRACKBALL_PIN_2);
  trackballDirectionY = digitalRead(TRACKBALL_PIN_3) * 2 - 1;
  trackballMotionY = digitalRead(TRACKBALL_PIN_4);

  if (trackballMotionX != xold) {
    Mouse.move(trackballDirectionX * trackballScaleX, 0);
    digitalWrite(13, 1);
  }

  if (trackballMotionY != yold) {
    Mouse.move(0, trackballDirectionY * trackballScaleY);
    digitalWrite(13, 1);
  }

  // Atari 50 defaults to spacebar for fire button.
  if (!digitalRead(TRACKBALL_BUTTON_PIN_6)) {
    Keyboard.press(' ');
  } else {
    Keyboard.release(' ');
  }
}


void updateDriving() {
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


    Mouse.move(currentRotationDir * min(drivingCounter * drivingScale, 127), 0);

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


void loop() {
  switch (controllerType) {
    case NOT_FOUND:
      { 
        detectControllers();
        break;
      }
    case PADDLE_CTRL:
      { 
        updatePaddles();
        break;
      }
    case DRIVING_CTRL:
      { 
        updateDriving();
        break;
      }
    case TRACKBALL_CTRL:
      { 
        updateTrackball();
        break;
      }
  }
}