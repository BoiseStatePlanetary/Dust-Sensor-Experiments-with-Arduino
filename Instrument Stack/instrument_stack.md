2020 Feb 20

And now to try to stack all the instruments together and power from a battery. Let's start with using the battery.

<Picture of battery plugged in>

Worked! Let's go for the gold and plug the dust sensor in and try to get it to write the pressure logger data (and timestamp) to the SD card.

<Picture of dust sensor, SD card shield, etc.>

OK, one issue with printing a timestamp is that the RTC seems to have a different sampling rate (baud) than the SD card. In the Datalogger example sketch (File > Examples > SD > Datalogger), I see:

> Serial.begin(9600);

But in the RTC example sketch (File > Examples > RTClib > pcf8523):

> Serial.begin(57600);

