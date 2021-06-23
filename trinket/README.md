# Trinket Build Instructions

This folder is for building a Cortana AI Chip with an Adafruit Trinket Mini 5V dev board.

[IMAGE]

## Part List:
* Adafruit trinket https://amzn.to/2SXYDr3
* Neopixel jewel https://amzn.to/3gKQ6ko
* Pushbutton https://amzn.to/3x0Akra
* USB Connectors https://amzn.to/3qiJgWt

* Silver PLA Filament https://amzn.to/3vN5HnV
* Gray Glitter PLA Filament https://amzn.to/3d2W3ab
* Transparent PLA Filament https://amzn.to/3xEOMFm

We are a participant in the Amazon Services LLC Associates Program, an affiliate advertising program designed to provide a means for us to earn fees by linking to Amazon.com and affiliated sites.

## 3D Printing:

The Cortana Data Chip’s 3D model was designed by Dylan Powell. I modified the Halo Cortana Chip to fit the neopixel jewel and usb plug. Model is prescaled to 125%. Download: https://www.thingiverse.com/thing:4891701

I printed the Data Chip with 0.20 layer height as a color change job at 2.80mm. Dark grey and then Silver.

## Assembly and Wiring:
* Trinket #0 to Neopixel Data In
* Trinket #1 to Button LED
* Trinket #2 to Button Switch

I wired the neopixel through the USB socket.  V+ and GND I kept on pins 1 and 4, neopixel data wire on one of the data pins.  This ensures that if the USB is plugged into a real USB port, nothing will break.

## Arduino:
The code was based on an adafruit example for the trinket. I wrote about 10 animations, inspired by my previous energy sword builds.
