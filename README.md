# <a href="https://www.storage-b.com/c/572">METAR</a>
C++ <a href="https://en.wikipedia.org/wiki/METAR">METAR</a> Decoder

Metar is class for decoding weather information encoded in the <a href="https://en.wikipedia.org/wiki/METAR">METAR</a> format

This is a work in progress.  So far, the following have been implemented:
  * Message Type
  * Location Identifier
  * Date and Time of Report
  * Wind
  * Visibility
  * Weather Phenomena (ongoing)
  * Sky Conditions
  * Vertical Visibility
  * Pressure
  * Temperature

Look <a href="https://github.com/jachappell/METAR/blob/master/example/main.cpp">here</a> to see it in action.

To build the library:<br />
$ make
 
To build the example:<br />
$ cd example <br />
$ make<br />

To build and run tests:<br />
$ cd tests <br />
$ mkdir build<br />
$ cd build<br />
$ cmake ..<br />
$ make<br />
$ ./metar_test
