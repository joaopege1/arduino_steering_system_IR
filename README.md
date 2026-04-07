# IR Remote Servo Controller & Turn Signals

## About the Project
This project is an interactive Arduino build that simulates a steering mechanism. It uses an Infrared (IR) remote to control the angle of a servo motor. As the servo turns left or right, corresponding LEDs blink to act as turn signals. The current angle of the servo is continuously updated and displayed on an I2C LCD screen. 

This is a great starting point for building remote-controlled RC cars, robotic pan-and-tilt mounts, or automated turrets!

## Hardware Requirements
* Arduino (Uno, Nano, or Mega)
* 1x Servo Motor (e.g., SG90)
* 1x IR Receiver Module (e.g., VS1838B)
* 1x IR Remote Control
* 1x 16x2 LCD Display with I2C Module
* 2x LEDs (Left and Right indicators)
* Breadboard and jumper wires

## Libraries Used
Make sure you have the following libraries installed in your Arduino IDE:
* `Wire.h` (Built-in)
* `Servo.h` (Built-in)
* `LiquidCrystal_I2C.h` (By Frank de Brabander or similar)
* `IRremote.hpp` (By shirriff, z3t0, ArminJo - *Ensure you use version 3.0 or higher*)
