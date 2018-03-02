# <a href="https://www.storage-b.com/c/572">METAR</a>
C++ <a href="https://en.wikipedia.org/wiki/METAR">METAR</a> Decoder

Metar is class for decoding weather information encoded in the <a href="https://en.wikipedia.org/wiki/METAR">METAR</a> format

This is a work in progress.  So far, the following have been implemented:
  * Message Type
  * Location Identifier
  * Date and Time of Report
  * Wind
  * Pressure
  * Temperature

Look <a href="https://github.com/jachappell/METAR/blob/master/example/main.cpp">here</a> to see an example.
 
To build the example:
$ cd example
$ make -f weather.mak

To build and run tests:
$ cd tests
$ ./run_tests.sh
