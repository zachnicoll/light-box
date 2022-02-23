# Light Box

_Happy Birthday Cass!_

This repo contains the code and files required for programming and 3D-printing a "light box". The lamp operates from an Arudino Nano and utlises the Adafruit NeoPixel library to control a NeoPixel ring. A button is used to change the colour of the ring, while a potentiometer is used to change the brightness.

## Get Started

To program the Arduino Nano, the Arduino IDE needs to be downloaded from [here](https://www.arduino.cc/en/software).
- Inside the IDE, open up `light_box/light_box.ino`.
- From the navigation bar, click Tools > Library Manager.
- Search for "Adafruit NeoPixel", and install the library of that exact name. _You can find these instructions on the Adafruit NeoPixel library's [GitHub page](https://github.com/adafruit/Adafruit_NeoPixel)._
- From the navigation bar, click Tools > Port, and select the COM port corresponding to the one you plugged the Nano into.
- Then click Upload, and you're done!

## Materials

### Parts List

- Arduino Nano
- NeoPixel Ring (24 pixels)
- Push Button
- Potentiometer
- 4 Screws and Nuts (M4)

### 3D Printing

- Clear acrylic
- Black acrylic
