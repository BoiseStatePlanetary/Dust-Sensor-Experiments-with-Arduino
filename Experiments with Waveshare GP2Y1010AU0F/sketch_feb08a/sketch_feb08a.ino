/**
* The MySensors Arduino library handles the wireless radio link and protocol
* between your home built sensors/actuators and HA controller of choice.
* The sensors forms a self healing radio network with optional repeaters. Each
* repeater and gateway builds a routing tables in EEPROM which keeps track of the
* network topology allowing messages to be routed to nodes.
*
* Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
* Copyright (C) 2013-2015 Sensnology AB
* Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
*
* Documentation: http://www.mysensors.org
* Support Forum: http://forum.mysensors.org
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* version 2 as published by the Free Software Foundation.
*
*******************************
*
* REVISION HISTORY
* Version 1.2 - created by anonymous user
* Version 1.3 - Added iled pinout in setup
* 
* DESCRIPTION
* A MySensors Arduino script for the WaveShare Dust Sensor. This is a Sharp GP2Y1010AU0F
* sensor that is built on top of some extra regulating and dasiy-chainable bit of electronics. 
* In practice it can only detect about 70 dust levels. More details about it can be found here:
* www.waveshare.com/wiki/Dust_Sensor
*
* Connect the sensor as follows:
* 
*   GND        to Arduino GND
*   VCC        to Arduino 5V (The Sensor needs 5v to operate, 3.3v is not ok!)
*   AOUT       to Arduino pin A0 (the values from the sensor arrive here)
*   ILED       to Arduino pin 3 (this is used to control the LED that the sensor uses internally)
*   
* It is also HIGHLY RECOMMENDED that you connect a tiny fan that will blow air into the sensor. 
* Search for "5v cooler" on Aliexpress to find a very cheap and small 2.5cm fan which costs about a dollar.
* Just hook that up to your 5V and GND on your arduino, with a resistor. I've attached a variable 
* resistor so that I could change the fan's speed (which I then keep constant and quiet).
*
* This script is inspired by the example file shared by the WaveShare company that explains how to use their creation.
* The script has been written in such a way that it doesn' use the Sleep command. This allows it to easily function as a repeater in a node network.
* It has also been written in a way to relatively easily allow for the addition of another sensor.
* 
* http://www.mysensors.org/build/dust
* 
*/


// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24                                // Radio device. There is a long-range radio version available. Don't power that from the Arduino pins, but connect it to a separate GND and 5V line instead.
// #define MY_RF24_PA_LEVEL RF24_PA_LOW               // If you DO want to connect the long-range radio directly to the arduino's 5V and GND, then set it to low-power mode.
// #define MY_RADIO_RFM69                             // An alternative radio technology.

// #define MY_REPEATER_FEATURE                        // This script has been written in such a way that it can simultaneously function as a MySensors repeater. Just remove the two slashes at the beginning on this line.

#include <MySensors.h>  
#include <SPI.h>



// VARIABLES


// WaveSchare 2,5 micrometer sensor specific settings. These all use "25" in the variable name to make it easy to copy/paste it all if you want to add a second sensor.

//You can change these:
#define ILED 7                                        // Drive the sensor' led of sensor via pin 7 on the arduino. Feel free to change.
#define VOUT A3                                       // Analog input for the Sharp sensor, in this case connectedto the A0 on the arduino. Feel free to change.
#define SEND_FREQUENCY25                   30000      // Similar to SLEEP TIME. Time between sending measurements to MySensors. In this case every minute. Should be higher than INTERNAL_MEASUREMENT_INTERVAL25 * NUM_READS25.
#define INTERNAL_MEASUREMENT_INTERVAL25    1280       // MILLIseconds to wait until the next internal measurement.
#define NUM_READS25                        10         // Number of Waveshare sensor readings that will be averaged before that average value is sent to the MySensors gateway.

// Only change these if you know what you're doing:
#define SENSOR_WARMUP_MICROTIME25          280        // MICROseconds that the LED has to be on before a reading is made.
#define CONVERSION_RATIO25                 0.2        // A quick way to go form the calculated voltage to the dust weight, expressed in ug/m3 / mv
#define NO_DUST_VOLTAGE25                  400        // A compensation in Millivolts for leaked in light? If so, in theory you should calibrate this. 
#define VOLTAGE_SUPPLIED_TO_SENSOR25       5000       // The maximum milli-voltage you will supply your sensor. Here 5000 means it's 5 Volts.

// Don't change these:
float   CALCULATED_VOLTAGE25 = 0;                     // Variable to hold the voltage which will be calculated from the received Waveshare sensor value.
float   DUST_DENSITY25 = 0;                           // Variable that will hold the calculated dust weight per cubic meter in ug/m3.
int     RAW_INPUT25 = 0;                              // Variable to hold the measurement received from the WaveShare sensor. Doesn't get much higher than 80. The WaveShave uses an internal 1/10 voltage divider, so multiply by 11 to get to the originally measures voltage. Weird design choice..
float   SENSOR_SUM25 = 0;                             // A variable that stores the sum of all sensor values during collection run. If you divide it through the number of measurements, you get the average value.
int     GATHERED_READINGS25 = 0;                      // A simple counter to see if we have enough readings to average and then send to the MySensors gateway.
unsigned long PREVIOUS_MEASUREMENT_MILLIS25 = 0;      // Used for timing the internal gathering loop, in this case counting to see it a second has passed since the previous measurement.
unsigned long PREVIOUS_SEND_MILLIS25 = 0;             // Used to remember the previous time the sensor work up to start gathering fresh data, to count if the SEND_FREQUENCY interval has passed.
unsigned long LED_ON_MICROTIME25 = 0;                 // To keep track of the time since the Sharp's LED was turned on. Voltage should be read 280 microseconds after turning on.
boolean CURRENTLY_SENSING25 = true;                   // Allows for sleeping. It is set to "true" every time we reach the SEND_FREQUENCY counter, and set to false when enough internal measurements have been gathered to send and average to the gateway.
boolean CURRENTLY_MEASURING25 = false;                // Used to regulate each internal measument. Indicates if the Sharp sensor' LED is on. It needs to be on for 280 milliseconds before taking the measurement.

// MySensors settings
#define CHILD_ID_DUST_25 0                            // For MySensors. Give each attached sensor a number.
MyMessage DUST_MSG25(CHILD_ID_DUST_25, V_LEVEL);      // Set up the value we will be updating to the MySensors gateway.



// General settings
unsigned long CURRENT_MILLIS = 0;                     // The millisecond clock in the main loop.
unsigned long CURRENT_MICROS = 0;                     // The microsecond clock in the main loop.



void setup(){
  sleep(2000);                                        // For old times sake.
  pinMode(ILED, OUTPUT);                              // Setting LED pin.
  digitalWrite(ILED, LOW);                            // ILED's default position is low (an advantage of using the WaveShare).
  Serial.begin(115200);                               // for serial debugging
  Serial.print("Sensor online \n ");
}



void presentation()
{  
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Dust Sensor", "1.2");
  present(CHILD_ID_DUST_25, S_DUST);
  
  //If you mix in multiple sensors, here is a suggestion:
  //present(CHILD_ID_DUST_10, S_DUST);                // Bigger particles. It's coarse and rough and irritating and it gets everywhere. Your nose filters these quite well.
  //present(CHILD_ID_DUST_25, S_DUST);                // Fine dust. Known to be detrimental to health in larger quantities, as they can get deeper into your lungs.
  //present(CHILD_ID_DUST_01, S_DUST);                // One micrometer. The biggest of the so-called 'ultra-fine' nano-particles, of which the health effects are still under investigation.
}



void loop(){
  CURRENT_MILLIS = millis();                          // This script tries to avoid using the Sleep function, so that it could at the same time be a MySensors repeater.
  CURRENT_MICROS = micros();                          // For extra precise measurements at sensors. Rolls over every 70 minutes, but it shouldn' affect measurements; subtraction will still work.
 
  if (CURRENT_MICROS - LED_ON_MICROTIME25 >= SENSOR_WARMUP_MICROTIME25) {
        
    if (CURRENTLY_MEASURING25 == true){
      
      RAW_INPUT25 = analogRead(VOUT);
      digitalWrite(ILED, LOW);
      
      Serial.print("WaveShare: read value: ");
      Serial.print(RAW_INPUT25);
      Serial.print("\n");
      
      CALCULATED_VOLTAGE25 = (VOLTAGE_SUPPLIED_TO_SENSOR25 / 1024.0) * RAW_INPUT25 * 11; // This is the conversion recommended by WaveShare.
  
      if (CALCULATED_VOLTAGE25 >= NO_DUST_VOLTAGE25) { //Filtering out leaked light apparently. I'm not even mentioning the spikes..
        CALCULATED_VOLTAGE25 -= NO_DUST_VOLTAGE25;
        Serial.print("WaveShare: measured voltage is high enough: ");
        Serial.print(CALCULATED_VOLTAGE25);
        Serial.print("\n");
        DUST_DENSITY25 = CALCULATED_VOLTAGE25 * CONVERSION_RATIO25;
        
        SENSOR_SUM25 = SENSOR_SUM25 + DUST_DENSITY25;
        GATHERED_READINGS25++;
        Serial.print(GATHERED_READINGS25);
        Serial.print(" gathered readings. ------------\n");
      }
      else
      {
        Serial.print("Waveshare: too little dust detected. \n");
        // Low readings have a way too big influence on the average value. I am not adding them to the average, which is cheating a little bit. But the other way around it would have way more detrimental influence.
        DUST_DENSITY25 = 0;
      }
      CURRENTLY_MEASURING25 = false;
    }  
  }

  if (CURRENT_MILLIS - PREVIOUS_MEASUREMENT_MILLIS25 >= INTERNAL_MEASUREMENT_INTERVAL25) {
    if(CURRENTLY_MEASURING25 == false && CURRENTLY_SENSING25 == true){
      // Time for a new internal measurement
      PREVIOUS_MEASUREMENT_MILLIS25 = CURRENT_MILLIS;
      CURRENTLY_MEASURING25 = true;
      LED_ON_MICROTIME25 = micros();
      digitalWrite(ILED, HIGH);

    }
  }
  
  if(GATHERED_READINGS25 >= NUM_READS25){
    if(CURRENTLY_SENSING25 == true){
      Serial.print("Gathered enough readings, time to send an average and then sleep. \n");
      float sendMe = SENSOR_SUM25 / GATHERED_READINGS25;
      send(DUST_MSG25.set(sendMe,2)); // Send the 2,5 micrometer data to the MySensors gateway. It makes no scientific sense to send decimals really, it gives afalse sense of accuracy.
      Serial.print("=== WaveShare: the SENT average was ");
      Serial.print(sendMe);
      Serial.print(" micrograms of 2,5 micrometer dust per cubic meter ===\n");
      Serial.print("Waveshare: zzzzZZZZzzzzZZZZzzzz\n");
      CURRENTLY_SENSING25 = false; // The work is done, time to sleep.
      CURRENTLY_MEASURING25 = false; // No need to make another measurement, time to sleep.
      digitalWrite(ILED, LOW); // Turn off LED, just to be sure. Time to sleep.
    }
  }
  
  if(CURRENT_MILLIS - PREVIOUS_SEND_MILLIS25 >= SEND_FREQUENCY25){
    if(CURRENTLY_SENSING25 == true){
      // Uh oh! We're at the end of the run, and still don't have enough good measurements! Is the air that clean?
      float sendMe = 0;
      if(GATHERED_READINGS25 <= (NUM_READS25 / 2)){
        send(DUST_MSG25.set(sendMe,2)); // Very few measurements succeeded, so it's better to send a 0 value to indicate this.
        }
        else
        {
         Serial.print("Sending what we have..\n");
         float sendMe = SENSOR_SUM25 / GATHERED_READINGS25;
         send(DUST_MSG25.set(sendMe,2)); // Send the 2,5 micrometer data to the MySensors gateway. It makes no scientific sense to send decimals really, it gives a false sense of accuracy.
        }
      CURRENTLY_MEASURING25 = false; // No need to make another measurement in this run, time's up.
    }
    Serial.print("Time to wake up and start fresh. \n");
    PREVIOUS_SEND_MILLIS25 = CURRENT_MILLIS; // Save the current time, to later calculate when we the sensor should wake up again.
    GATHERED_READINGS25 = 0; // Reset measurements counter.
    SENSOR_SUM25 = 0; // Reset the variable that stores the sum of the internally gathered measurements.
    CURRENTLY_SENSING25 = true; // Here we go again.
  }
}
