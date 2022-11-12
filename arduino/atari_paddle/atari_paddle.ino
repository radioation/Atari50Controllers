#include <Joystick.h>

// voltage divider
const int Vin = 5;
float Vout = 0.0f;
const float ref = 1000.0f;  // 1 M Ohm

// I/O Pins
const int PADDLE_1_POT_PIN_9 = 0;  // use analog 0 for paddle 1
const int PADDLE_2_POT_PIN_5 = 5;  // analog 1
const int PADDLE_1_BTN_PIN_3 = 0;
const int PADDLE_1_BTN_PIN_4 = 1;

// paddle values
int rVal = 0;
float paddle1 = 0;
float paddle2 = 0;
float buffer = 0;

int lastButtonState1 = 0;
int lastButtonState2 = 0;

const float paddle1min = 0.66f;
const float paddle2min = 0.66f;


Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_JOYSTICK,
                   2, 0,                  // Button Count, Hat Switch Count
                   true, true, false,     // X and Y, but no Z Axis
                   false, false, false,   // No Rx, Ry, or Rz
                   false, false,          // No rudder or throttle
                   false, false, false);  // No accelerator, brake, or steering

void setup() {
  // setup buttons
  pinMode(PADDLE_1_BTN_PIN_3, INPUT_PULLUP);
  pinMode(PADDLE_1_BTN_PIN_4, INPUT_PULLUP);

  // Initialize Joystick Library
  Joystick.begin();
  Joystick.setXAxisRange(-127, 127);
  Joystick.setYAxisRange(-127, 127);
}

float computePaddle(int val) {
  buffer = val * Vin;
  Vout = (buffer) / 1024.0;
  buffer = (Vin / Vout) - 1;
  return ref * buffer;
}

void loop() {
  rVal = analogRead(PADDLE_1_POT_PIN_9);
  if (rVal) {
    paddle1 = computePaddle(rVal);
  }

  rVal = analogRead(PADDLE_2_POT_PIN_5);
  if (rVal) {
    paddle2 = computePaddle(rVal);
  }

  // scale and translate paddle readings.  (This couild use a proper calibration step)
  float axis_x = -(127.0f * 2.0f * (paddle1 / 700.0f) - 127.0f);
  Joystick.setXAxis(axis_x);

  float axis_y = 127.0f * 2.0f * (paddle2 / 700.0f) - 127.0f;
  Joystick.setYAxis(axis_y);

  // Set buttons  
  int currentButtonState = digitalRead(PADDLE_1_BTN_PIN_3);
  if (currentButtonState != lastButtonState1) {
    Joystick.setButton(0, currentButtonState);
    lastButtonState1 = currentButtonState;
  }

  currentButtonState = digitalRead(PADDLE_1_BTN_PIN_4);
  if (currentButtonState != lastButtonState2) {
    Joystick.setButton(1, currentButtonState);
    lastButtonState2 = currentButtonState;
  }
}