2020 Feb 22

In Death Valley, trying to set up the Waveshare GP2 according to this wiki - https://www.waveshare.com/wiki/Dust_Sensor.

And then hooking up the BMP180 according to the website below. 

(As an aside, the narrative on the randomnerd website about the BMP180 says to hook the BMP180 up to 5 V to power, 
but the code that they provide explicitly says NOT to use 5 V. I opted to use the 3.3 V power supply for the BMP180
and the 5 V for the Waveshare GP2.)



2020 Feb 21

In Vegas hotel room, trying to set up instrument stack.

First thing I want to check is whether I can set up two PPDs at once. So I'm modifying
the code instrument_stack/PPD_pressure_datalogger.ino to take data from two dust PPDs.

That resulted in a code called Both_PPDs_pressure_datalogger.ino.

Data file with control data will be called 20Feb21C.txt, and columns will be:
datetime + ", " + String(exposed_concentration) + ", " + String(unexposed_concentration) + ", " + String(P) + ", " + String(T);

Be sure you installed the BMP180 library from https://randomnerdtutorials.com/guide-for-bmp180-barometric-sensor-with-arduino/. That library is not available via Arduino's library manager.

Couldn't get both PPDs working at once, so I'm going to plan to test them one at a time.

I wrote PPD.ino files for each PPD and for each test (bagged and exposed). The only 
difference between the files is that they save to different filenames - 
* GreePPDE - PPD with green duck tape exposed to dust*
* GreePPDB - PPD with green duck tape not exposed to dust (i.e., bagged)*
* OrigPPDB - Paper-covered PPD not exposed to dust (i.e., bagged)*
* OrigPPDE - Paper-covered PPD exposed to dust *

Next, I tried to set up the Waveshare GP2 instrument. I referred to this website -
https://www.waveshare.com/wiki/Dust_Sensor.

Actually, I decided I won't have time to set up the Waveshare (it's 11:25a, and I have to
check out at 12p). We also already flew it last year. So I'm going to wrap up and head out
to the field.

...

2020 Feb 20

And now to try to stack all the instruments together and power from a battery. Let's start with using the battery.

<Picture of battery plugged in>

Worked! Let's go for the gold and plug the dust sensor in and try to get it to write the pressure logger data (and timestamp) to the SD card.

<Picture of dust sensor, SD card shield, etc.>

OK, one issue with printing a timestamp is that the RTC seems to have a different sampling rate (baud) than the SD card. In the Datalogger example sketch (File > Examples > SD > Datalogger), I see:

> Serial.begin(9600);

But in the RTC example sketch (File > Examples > RTClib > pcf8523):

> Serial.begin(57600);
