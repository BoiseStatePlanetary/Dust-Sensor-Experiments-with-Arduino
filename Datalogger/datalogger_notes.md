2020 Feb 19

Trying to get datalogger to work following the notes here - https://learn.adafruit.com/adafruit-data-logger-shield.

Need to install the RTClib (https://github.com/adafruit/RTClib) in order to access the real-time clock onboard the datalogger. Follow instructions on this website (https://learn.adafruit.com/adafruit-data-logger-shield/using-the-real-time-clock) for installing the library. 

Initially, I can see the clock using the RTClib example but am unable to reset the time. I pulled the battery out and pressed the reset button.

OK, that seems to have reset the clock, and I see in the serial monitor:

14:09:58.015 -> 2020/2/19 (Wednesday) 14:8:8
14:09:58.015 ->  since midnight 1/1/1970 = 1582121288s = 18311d
14:09:58.015 ->  now + 7d + 12h + 30m + 6s: 2020/2/27 2:38:14'

