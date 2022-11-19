#include <Joystick.h>
 

// I/O Pins
const int PADDLE_1_CAP_PIN_9 = 10;  // read from cap
const int PADDLE_2_CAP_PIN_5 = 11;
const int PADDLE_1_POT_PIN_9 = 12;  // discharge at resistor
const int PADDLE_2_POT_PIN_5 = 13;
const int PADDLE_1_BTN_PIN_3 = 2;
const int PADDLE_2_BTN_PIN_4 = 3;
const int PADDLES_ACTIVE = 9;   // use this to turn switch on/off



// paddle values
unsigned long startTime;
unsigned long elapsedTime;
float paddle1 = 0;
float paddle2 = 0;

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
  pinMode(PADDLE_1_CAP_PIN_9, INPUT);
  pinMode(PADDLE_2_CAP_PIN_5, INPUT);
  pinMode(PADDLE_1_POT_PIN_9, OUTPUT);
  pinMode(PADDLE_2_POT_PIN_5, OUTPUT);
  pinMode(PADDLE_1_BTN_PIN_3, INPUT_PULLUP);
  pinMode(PADDLE_2_BTN_PIN_4, INPUT_PULLUP);
  pinMode(PADDLES_ACTIVE, INPUT_PULLUP);

  // Initialize Joystick Library
  Joystick.begin();
  Joystick.setXAxisRange(-127, 127);
  Joystick.setYAxisRange(-127, 127);
}

 
void loop() {

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
  float axis_x = -(127.0f * 2.0f * (paddle1 / 9000.0f) - 127.0f);
  Joystick.setXAxis(axis_x);
  float axis_y = 127.0f * 2.0f * (paddle2 / 9000.0f) - 127.0f;
  Joystick.setYAxis(axis_y);

  // Set buttons
  int currentButtonState = digitalRead(PADDLE_1_BTN_PIN_3);
  if (currentButtonState != lastButtonState1) {
    Joystick.setButton(0, currentButtonState);
    lastButtonState1 = currentButtonState;
  }

  currentButtonState = digitalRead(PADDLE_2_BTN_PIN_4);
  if (currentButtonState != lastButtonState2) {
    Joystick.setButton(1, currentButtonState);
    lastButtonState2 = currentButtonState;
  }
}