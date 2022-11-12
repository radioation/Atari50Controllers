#include "Mouse.h"
#include "Keyboard.h"

const int TRACKBALL_PIN_1 = 8;
const int TRACKBALL_PIN_2 = 9;
const int TRACKBALL_PIN_3 = 10;
const int TRACKBALL_PIN_4 = 11;
const int TRACKBALL_BUTTON_PIN_6 = 12;

const int XINC = 7;
const int YINC = 7;

int xDirection = LOW;
int xMotion = LOW;
int yDirection = LOW;
int yMotion = LOW;

void setup() {
  // setup pins
  pinMode(TRACKBALL_PIN_1, INPUT_PULLUP);
  pinMode(TRACKBALL_PIN_2, INPUT_PULLUP);
  pinMode(TRACKBALL_PIN_3, INPUT_PULLUP);
  pinMode(TRACKBALL_PIN_4, INPUT_PULLUP);
  pinMode(TRACKBALL_BUTTON_PIN_6, INPUT_PULLUP);


  // initialize Mouse and Keyboard
  Mouse.begin();
  Keyboard.begin();
}

void loop() {

  // logic from http://jeffpiepmeier.blogspot.com/2016/08/vintage-atari-cx22-trak-ball-with-usb.html
  int xold = xMotion;
  int yold = yMotion;

  xDirection = digitalRead(TRACKBALL_PIN_1) * 2 - 1;
  xMotion = digitalRead(TRACKBALL_PIN_2);
  yDirection = digitalRead(TRACKBALL_PIN_3) * 2 - 1;
  yMotion = digitalRead(TRACKBALL_PIN_4);

  if (xMotion != xold) {
    Mouse.move(xDirection * XINC, 0);
    digitalWrite(13, 1);
  }

  if (yMotion != yold) {
    Mouse.move(0, yDirection * YINC);
    digitalWrite(13, 1);
  }

  // Atari 50 defaults to spacebar for fire button.
  if (!digitalRead(TRACKBALL_BUTTON_PIN_6)) {
    Keyboard.press(' ');
  } else {
    Keyboard.release(' ');
  }
}