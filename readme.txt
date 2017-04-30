# # This is an DDM () C++ library for Arduino, created by Oakkar7/MYANMAR

(1) A Lib using small form-factor pluggable (SFP) module DDM Digital diagnostics monitoring (DDM) protocol.
(2)An Optical Power Meter with Arduino example is added for design example.

version : 1.3 beta
Date    : 27 Nov 2016
By      : oakkar7, Myanmar
blog    ; okelectronic.wordpress.com
code    : https://github.com/oakkar7

## Credit to:
1) Nokia 5110 LCD is powered by Adafruit, Adafruit-PCD8544-Nokia-5110-LCD-library
https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library

2) SPI DDM interface is based on the works of 
SFPDDM (Arduino library for interfacing SFP modules and reading DDM information as per SFF-8472)
https://github.com/SloMusti/sfpddm


## Installation
--------------------------------------------------------------------------------

To install this library, just place this entire folder as a subfolder in your
Arduino/lib/targets/libraries folder.

When installed, this library should look like:

Arduino/lib/targets/libraries/ddm              (this library's folder)
Arduino/lib/targets/libraries/ddm/ddm.cpp     (the library implementation file)
Arduino/lib/targets/libraries/ddm/ddm.h       (the library description file)
Arduino/lib/targets/libraries/ddm/keywords.txt (the syntax coloring file)
Arduino/lib/targets/libraries/ddm/examples     (the examples in the "open" menu)
Arduino/lib/targets/libraries/ddm/readme.txt   (this file)

Building
--------------------------------------------------------------------------------

After this library is installed, you just have to start the Arduino application.
You may see a few warning messages as it's built.

To use this library in a sketch, go to the Sketch | Import Library menu and
select Test.  This will add a corresponding line to the top of your sketch:
#include <ddm.h>

To stop using this library, delete that line from your sketch.

Geeky information:
After a successful build of this library, a new file named "ddm.o" will appear
in "Arduino/lib/targets/libraries/ddm". This file is the built/compiled library
code.

If you choose to modify the code for this library (i.e. "ddm.cpp" or "ddm.h"),
then you must first 'unbuild' this library by deleting the "ddm.o" file. The
new "ddm.o" with your code will appear after the next press of "verify"

## License

This project is licensed under the GNU-V3 License - see the [LICENSE.md](LICENSE.md) file for details

