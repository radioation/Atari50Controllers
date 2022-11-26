# Overview
I started working on this after printing out Mike Mika's [paddle adapter](https://www.thingiverse.com/digitaleclipse/designs)
for the "Atari 50: The Anniversary Celebration".   I realized I still had my 
old Atari 800 controllers and thought this would be a fun project to teach 
myself about using the Arduino Leonardo as a Joystick, Keyboard, and Mouse.

While working on this, I discovered there are already several other 
projects and products that adapt Atari controllers to PC.  So it's probably 
easier to just buy an existing adapter, but I do this sort of thing for fun.
Also, I've been meaning to create custom controllers so it's a nice little
learning exercise.

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

## CX40 Joystick
The Atari joystick controller is very simple.  It has 4 switches that pull down pins 1-4 of
the Atari DE9 port a fire button that pulls down pin 6. 

# The Circuit
This circuit:
![Combined Schematic](https://github.com/radioation/Atari50Controllers/blob/master/images/combined.pdf)
Can handle all 4 controller types.   The images folder has simplified circuits that handle the 
paddle, driving, or trackball/joystick controllers individually.   The sketch should work with any
of them.

The sketch can automatically detect the type of controller attached.   I based my detection on Control Freak's
description at [hackaday.io](https://hackaday.io/project/170908-control-freak/log/176711-atariing).    
It's not exactly the same.  I don't  have an Atari keypad and they don't mention the trackball controller.  
My sketch will first check for a valid paddle value.  If a paddle isn't detected, the sketch checks the 
joystick pins to determine what type of controller is attached.   If LEFT and RIGHT pins are detected 
simultaneously, the controller is a trackball controller.  If UP and DOWN are detected simultaneously, 
it's a driving controller.  If only one direction is detected, it's a joystick.



# Usage
Plug your Atari controller into the DE9 and connect the Arduino to your PC.  Move the 
controller around a bit to let the Sketch determine what type of joystick is connected.

If you want to change the controller, swap it out and press the RESET button on the 
Arduino.  This will restart controller detection.

If you have a paddle controller or joystick controller, the sketch will act like a USB 
Joystick.   The buttons will be mapped to joystick buttons. The paddle controllers will 
only affect the X axis of the joystick.

If you have a trackball or driving controler, the sketch will act like a Keyboard and 
Mouse.  Based on the games I've played in Atari50, I mapped the fire button to 
the `Spacebar`.  The three other buttons are mapped to the `A`, `B`, and `C` keys.
You can either alter the sketch or remap the controls in Atari 50.






