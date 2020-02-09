### 2020 Feb 9
The 400-mV cutoff means that ADC-values of 7 and below return densities of zero, while ADC-values of 8 and above give non-zero densities. The minium dust density that can be returned is therefore 5.94 ug/m^3. The next highest value (ADC = 9) corresponds to a density of 16.68 ug/m^3, and then ADC = 10 gives 27.42 ug/m^3. How useful will these measurements be?

Metzger et al. (2011 - https://ui.adsabs.harvard.edu/abs/2011Icar..214..766M/abstract) say, "In situ (mobile) sampling of 33 natural dust devil vortices reveals very high total suspended particle (TSP) mean values of 296 mg/m^3 and fine dust loadings (PM10) mean values ranging from 15.1 to 43.8 mg/m^3." According the manufacturer's calibration curve, the sensor flatlines at 0.5 mg/m^3 and 3.5 V. 

Looking at Ralph's notes from our previous deployment with the dust sensor, returned voltages did not exceed 1.5 V, so we will probably be OK.

### 2020 Feb 8 

I started trying to set up the Waveshare GP2Y1010AU0F and decided to document the process (since I will undoubtedly goof up and need to figure out what I did).

~~To start, I tried DLing the sketch provided by the manufacturer from https://www.waveshare.com/wiki/File:Dust-Sensor-code.7z. When I tried to unzip the Arduino file, it didn't work.~~

~~So I went with the code posted here - https://forum.mysensors.org/topic/6391/the-dust-sensor-is-now-finally-working.~~

Actually, the zipped file here does work - https://www.waveshare.com/wiki/File:Dust-Sensor-code.7z.

Next, I needed to wire up the sensor. Keep in mind that I have purchased Waveshare's version of the sensor, which includes the required electronic components. So the blogs that describe how to wire up the basic GP2Y1010AU0F (e.g., https://www.instructables.com/id/How-to-Interface-With-Optical-Dust-Sensor/) are not directly applicable to me.

The first thing I did was broke another sensor set, cannibalizing the header pins.

![Header Pin](https://github.com/BoiseStatePlanetary/Dust-Sensor-Experiments-with-Arduino/blob/master/Setting_Up_Waveshare_GP2Y1010AU0F/IMG_0239.jpg "Cannibalizing Header Pins")

I then stripped the plastic rings off the pins so that I had pins long enough (it seemed) to fit into both the sensor plugs and the ports on the Arduino.

I then plugged the plugs as instructed on Waveshare's website, compiled, and uploaded the code, and the sensor began registering data.
