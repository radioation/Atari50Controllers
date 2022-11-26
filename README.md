# Overview
I started working on this after printing out Mike Mika's [Xbox paddle adapter](https://www.thingiverse.com/digitaleclipse/designs)
for the "Atari 50: The Anniversary Celebration".   I realized I still had some 
old Atari 800 controllers and thought it might be fun to learn
how to use an Arduino Leonardo as a Joystick, Keyboard, and Mouse.

While working on this, I discovered there are already several other 
projects and products that adapt Atari 8-bit controllers to PC.  So it's probably 
easier fto just buy an existing adapter, but I do this sort of thing for fun.
I've been meaning to make custom game controllers for a while now. So this is 
a nice, easy learning exercise.

This project uses an knockff Leonardo and Matthew Heironimus's 
[Arduino Joystick Library](https://github.com/MHeironimus/ArduinoJoystickLibrary/tree/version-2.0)
and the Arduino [Keyboard and Mouse](https://docs.arduino.cc/built-in-examples/usb/KeyboardAndMouseControl)
libraries.   

## CX30 Paddle Controllers
Each paddle controller has a single fire button and a 1 MΩ potentiometer 
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
estimated maximum value.   It doesn't seem to affect gameplay much.
Another problem is the paddles don't automatically return to a center position. 
So the sketch may continously send left or right signals while you're navigating 
your game menu.   

## CX22  Trackball Controller
My sketch is based on the code found [here](https://jeffpiepmeier.blogspot.com/2016/08/vintage-atari-cx22-trak-ball-with-usb.html). 
The trackball controller should be set to _trackball_ mode but can also 
function as a joystick.

When in trackball mode, the CX22 figures out both the X and Y direction 
and rotation speed of the trackball for you.  I'm translating the trackball movements into 
mouse movements and using the button as a 'space-bar' press to match
the default fire setup in **Atari 50: 5200 _Missile Command_** 

If the trackball is set to `joytsick` mode, the sketch will act like 
a joystick instead of a mouse and keyboard.

## CX20  Driving Controller
The driving controller is a rotary encoder.  My implementation is based on
[this](https://howtomechatronics.com/tutorials/arduino/rotary-encoder-works-use-arduino/).
I added a counter to handle the rotation speed.

## CX40 Joystick
The Atari joystick controller is very simple.  It has 4 switches that pull down pins 1-4 of
the Atari DE9 port a fire button that pulls down pin 6.  

# The Circuit
This circuit can be found
[here](https://github.com/radioation/Atari50Controllers/blob/master/images/combined.pdf).
It can handle all 4 controller types.   The images folder also has simplified circuits that handle the 
paddle, driving, or trackball/joystick controllers individually.   The sketch should work with any
of them.

THe circuit lists 5 switches.  SW1, SW2, and SW3 are extra game controller buttons. These were 
added to support games that use more than one button.  These buttons are optional if you only
play games that use one button.  SW4 is a rocker switch.  It's used to center the paddle joystick 
output.  SW5 is is a reset button and optional.


The sketch can automatically detect the type of controller attached.   I based my detection on Control Freak's
description at [hackaday.io](https://hackaday.io/project/170908-control-freak/log/176711-atariing).    
It's not exactly the same.  I don't  have an Atari keypad and they don't mention the trackball controller.  
My sketch will first check for a valid paddle value.  If a paddle isn't detected, the sketch checks the 
joystick direction pins to determine what type of controller is attached.   If LEFT and RIGHT pins are detected 
simultaneously, the controller is a trackball controller.  If UP and DOWN are detected simultaneously, 
it's a driving controller.  If only one direction is detected, it's a joystick.



# Usage
Plug your Atari controller into the DE9 port on the Leonardo and connect the Leonardo to your PC's USB port.  
Move the controller around a bit to let the sketch determine what type of controller is connected.

If you have a paddle controller or joystick controller, the sketch will act like a USB 
Joystick.   The buttons will be mapped to joystick buttons. The paddle controllers will 
only affect the X axis of the joystick.  The second paddle controller will be mapped 
to a second USB joystick.  

If you have a trackball or driving controler, the sketch will act like a Keyboard and 
Mouse.  Based on the games I've played in Atari50, I mapped the fire button to 
the `Spacebar`.  The three other buttons are mapped to the `A`, `B`, and `C` keys.
You can either alter the sketch or remap the controls in Atari 50.

If you want to change the controller, swap it out and press the RESET button on the 
Arduino.  This will restart controller detection.






