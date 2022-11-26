# Overview
This project adapts old-school Atari 8-bit controllers to work with "Atari 50: The Anniversary Celebration".

This project uses an Arduino Leonardo and Matthew Heironimus's 
[Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary/tree/version-2.0)
and the Arduino [Keyboard and Mouse](https://docs.arduino.cc/built-in-examples/usb/KeyboardAndMouseControl)
libraries.   

## CX30 Paddle Controllers
The paddle controller has a single fire button and a 1 MOhm potentiometer 
with just the wiper and one resistive terminal connected.  I originally 
used the voltage divider described
[here](https://www.circuitbasics.com/arduino-ohm-meter/) to estimate the 
rotation of the paddle knob.   One problem with using the voltage divider
was the paddle positions affected each others readings.   To address this,
I've switched to checking the charging time of an
[RC circuit](https://www.icode.com/analog-to-digital-conversion-with-rc-circuit-in-microcontroller-projects/).   
I alternate reading the first and second paddle which seems to eliminate 
the interference. 

I've noticed some problems with this method.  Each paddle has a different 
estimated maximum value.  I plan on improving this.  Another problem is 
translating the paddle readings into joystick positions.

## CX22  Trackball Controller
This is largely the code found [here](https://jeffpiepmeier.blogspot.com/2016/08/vintage-atari-cx22-trak-ball-with-usb.html). 
The trackball controller should be set to _trackball_ mode.  
When in trackball mode, the CX22 figures out both the X and Y direction 
and rotation speed of the trackball for you.  I'm translating the trackball movements into 
mouse movements and using the button as a 'space-bar' press to match
the default fire setup in **Atari 50: 5200 _Missile Command_** 

## CX20  Driving Controller
The driving controller is a rotary encoder.  My implementation is based on
[this](https://howtomechatronics.com/tutorials/arduino/rotary-encoder-works-use-arduino/).
I added a counter to handle the rotation speed.

# Usage
Coming Soon...



