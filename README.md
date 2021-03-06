# MSP432P401R-Lockbox

The goal of the project was to be able to take user input in the form of a keypad connected to the MSP432P401R microcontroller to activate a solenoid-based lockbox.
The user can set a pin to activate a solenoid to the locked position and later type that same pin to move the solenoid to the open position. While the main purpose of the program is to manipulate a solenoid to lock as well as unlock a box, the code goes a step further. Right after typing in a pin, the user will see a blinking light, and during the time the light is blinking, the user can press any button on the keypad to override the pin and set the lockbox to the always open position once again. If nothing is pressed, 
the solenoid is activated, and the box is now locked. When trying to unlock the box, the user types the pin, and if the pin is correct, the solenoid will once again activate to
unlock the box. If the pin is incorrect, the user gets a total of five tries before the keypad becomes inoperable for one minute before they can try again. Once the lockbox is
opened after being locked, a new pin can be set and the process starts again. All user input is displayed on a seven segment display connected to the MSP432P401R microcontroller via a multiplexer to reduce the amount of physical connections. Since the seven segment display is connected via a multiplexer, only one of the LEDs on the display is on at once, 
so it was necessary to find a way for the human eye to recognize all segments of the display as ON without noticeable flickering or ghosting.

# Demo:
https://www.youtube.com/watch?v=zqjpy5bm3PQ&ab_channel=ReyRodriguez
