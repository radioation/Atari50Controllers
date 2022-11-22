# Atari50Controllers
This project has several simple Arduino sketches to use Atari controllers 
with "Atari 50: The Anniversary Celebration".

This project uses an Arduino Leonardo and Matthew Heironimus's 
[Arduino Joystick Library.](https://github.com/MHeironimus/ArduinoJoystickLibrary/tree/version-2.0)

For now the circuits and sketches are pretty basic. I plan on combining 
all of the circuits and sketches into a single circuit and sketch that 
can handle all three controllers. 

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
translating the paddle readings into joystick positions.  This interferes
with menu selections when the paddles are plugged in.  I will probably 
add a switch or button to disable the paddles for menu navigation. 

## CX22  Trackball Controller
This is largely the code found [here](https://jeffpiepmeier.blogspot.com/2016/08/vintage-atari-cx22-trak-ball-with-usb.html). 
The trackball controller should be set to _trackball_ mode.  
When in trackball mode, the CX22 figure out both the X and Y direction 
and speed for you.  I'm translating the trackball movements into 
mouse movements and using the button as a 'space-bar' press to match
the default fire setup in _Atari 50: 5200 **Missile Command**_

The circuit diagram is probably unnecessary, because the wiring is 
very simple.  The DE9 pins are connected directly to the Arduino.

|  Joystick | Arduino |
| --------- | ------- |
|   1       | D8      |
|   2       | D9      |
|   3       | D10     |
|   4       | D11     |
|   6       | D12     |
|   7       | +5V     |
|   8       | GND     |

## CX20  Driving Controller
The driving controller is a rotary encoder.  My implementation is based on
[this](https://howtomechatronics.com/tutorials/arduino/rotary-encoder-works-use-arduino/).
I added a counter to handle the rotation speed.

Like the trackball circuit, a diagram is probably un-necessary.  The DE9 pins 
are connected directly to the Arduino.

|  Joystick | Arduino |
| --------- | ------- |
|   1       | D0      |
|   2       | D1      |
|   6       | D4      |
|   8       | GND     |


## TODO
* Improve driving controller reads.
  * Handle contact bounce in hardware.
* Combine circuits and sketches into a single circuit/sketch 
  and automatically detection which type of controller is 
  connected.
* Add more buttons for games that use more than one button
* Handle menu navigation when a paddle controller is connected.



