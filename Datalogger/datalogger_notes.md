2020 Feb 19

Trying to get datalogger to work following the notes here - https://learn.adafruit.com/adafruit-data-logger-shield.

<Picture of set-up>

Need to install the RTClib (https://github.com/adafruit/RTClib) in order to access the real-time clock onboard the datalogger. Follow instructions on this website (https://learn.adafruit.com/adafruit-data-logger-shield/using-the-real-time-clock) for installing the library. 

Initially, I can see the clock using the RTClib example but am unable to reset the time. I pulled the battery out and pressed the reset button.

OK, that seems to have reset the clock, and I see in the serial monitor:

14:09:58.015 -> 2020/2/19 (Wednesday) 14:8:8

14:09:58.015 ->  since midnight 1/1/1970 = 1582121288s = 18311d

14:09:58.015 ->  now + 7d + 12h + 30m + 6s: 2020/2/27 2:38:14'

Now let's addressing the SD card. In the Arduino sketch editor, I used File > Examples > SD > Cardinfo Woot! It worked. Be sure to modify this line in the SD/Cardinfo example:
> const int chipSelect = 10; // Defaults to 4

~~I moved on to the next example - https://learn.adafruit.com/adafruit-data-logger-shield/light-and-temperature-logger-use-it.~~ 

Actually, that example isn't really relevant. Let's try plugging in the expansion shield on top of the data logger and make sure I can grab data from the dust sensor.

Picture of dust sensor plugged in with expansion shield on top of data logger

~~At least initially, I couldn't get the dust sensor to generate a signal if I had the expansion shield plugged in on top of the datalogger. Let's plug the expansion shield directly into the arduino and make sure *that* works. 

~~Picture of expansion shield and dust sensor plugged directly into arduino~~

Oops! I had accidentally left the sample time as 30 seconds. No wonder I didn't see anything. Let's try plugging the expansion shield and dust sensor BACK into the datalogger, as I intended.

That works fine!

Now let's try writing the data to the SD card using File > Examples > SD > Datalogger.

Definitely wrote a data file!
