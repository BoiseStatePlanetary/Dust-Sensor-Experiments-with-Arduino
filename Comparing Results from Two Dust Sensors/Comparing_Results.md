### 2020 Feb 9

Starting with required documentation - 
* For the PPD42NS, this website provides the manufacturer's information: http://wiki.seeedstudio.com/Grove-Dust_Sensor/.
* For the GP2Y1010AU0F, my own website - https://github.com/BoiseStatePlanetary/Dust-Sensor-Experiments-with-Arduino/blob/master/Setting_Up_Waveshare_GP2Y1010AU0F/Setting_Up_Waveshare_GP2Y1010AU0F.md.

I've posted the combined code that reads signals from both.

Unfortunately, the calibration curves (https://github.com/BoiseStatePlanetary/Dust-Sensor-Experiments-with-Arduino/blob/master/Comparing%20Results%20from%20Two%20Dust%20Sensors/GP2Y1010AU0F_Calibration_Curve.png and https://github.com/BoiseStatePlanetary/Dust-Sensor-Experiments-with-Arduino/blob/master/Comparing%20Results%20from%20Two%20Dust%20Sensors/PPD42NS_Calibration_Curve.jpg) for the two sensors give measurements that are not directly comparable. 

I set up the sensors next to each other to directly compare their measurements.

![alt text](https://github.com/BoiseStatePlanetary/Dust-Sensor-Experiments-with-Arduino/blob/master/Comparing%20Results%20from%20Two%20Dust%20Sensors/IMG_0241.jpg "Experimental Set-Up")

Even though I could directly compare their measurements, I spritzed the sensors using a mister full of rubbing alcohol to see if they would register increases at the same time.

In a first experiment, I spritzed the two sensors twice to see how they responded (Experiments_about_4-50p_2020Feb9.xlsx). In another experiment, I spritzed them three times for five seconds each, separated in time by 10 seconds (Experiments_about_5p_2020Feb9.xlsx).

It seems pretty clear that the GPU has a long slow response, while the PPD responds quickly and then returns to the baseline value.
