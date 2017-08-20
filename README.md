# discoStrips
Controlling multiple NEOPixel led strips with a MKR1000.

Dependencies: FastLED, WiFi101

The program implements a simple REST API to control the various modes.

Available modes for now are:
* off
* on
* night
* disco

## REST API

mode
  /off
  /on
  /night
  /disco
brightness/<value 10-255>
colour
  /<colour name>
disco
  /tempo/<milli seconds>
  /brightness/<value 10-255>



