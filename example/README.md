# METAR example

To build:<br />
$ make<br />

Get the current weather condtions in Saint Louis:<br />
$ ./weather KSTL

Get the current weather condtions in Saint Louis and display the temperatue in Fahrenheit:<br />
$ ./weather -f KSTL

Pass in a METAR string:

$ ./weather -f -d "METAR LBBG 041600Z 12012MPS 090V150 1400 R04/P1500N R22/P1500U +SN BKN022 OVC050 M04/M07 Q1020 NOSIG 8849//91="
