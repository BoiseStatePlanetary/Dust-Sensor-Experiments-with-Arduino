/*
  SD card datalogger

 This example shows how to log data from three analog sensors
 to an SD card using the SD library.

 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>
#include "RTClib.h"
#include <string.h>

#include <SFE_BMP180.h>
#include <Wire.h>

// You will need to create an SFE_BMP180 object, here called "pressure":

SFE_BMP180 pressure;

#define ALTITUDE 824.0 // Altitude of Boise

int pin = 8;
unsigned long duration;
unsigned long very_first_starttime;
unsigned long starttime;
unsigned long sampletime_ms = 1000;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

const int chipSelect = 10;

RTC_PCF8523 RTC; // define the Real Time Clock object

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }

  Serial.println("card initialized.");

// From PPD_sensor.ino
  pinMode(pin,INPUT);
  very_first_starttime = millis();
  starttime = millis();//get the current time;

  while (!Serial) {
    delay(1);  // for Leonardo/Micro/Zero
  }

  if (! RTC.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! RTC.initialized()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

// Initialize the sensor (it is important to get calibration values stored on the device).
  if (pressure.begin())
      Serial.println("BMP180 init success");
    else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

      Serial.println("BMP180 init fail\n\n");
      while(1); // Pause forever.
  }

}

void loop() {
  char status;
  double T,P,p0,a;

  DateTime now = RTC.now();
  String yer = String(now.year());
  String mon = String(now.month());
  String dy = String(now.day());
  String hr = String(now.hour());
  String mn = String(now.minute());
  String sc = String(now.second());

  String datetime = yer + ", " + mon + ", " + dy + ", " + hr + ", " + mn + ", " + sc;
  
  // make a string for assembling the data to log:
  String dataString = "";

  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;

  if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve

    // Based on poly-fit to data from https://journals.plos.org/plosone/article?id=10.1371/journal.pone.0137789
//        concentration = 0.0008*pow(ratio, 4) - 0.049*pow(ratio, 3) + 0.9326*pow(ratio, 2) - 1.6215*ratio;
    Serial.print(lowpulseoccupancy);
    Serial.print(",");
    Serial.print(ratio);
    Serial.print(",");
    Serial.println(concentration);
    lowpulseoccupancy = 0;
    starttime = millis();
  }

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:
    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {

            // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Print out the measurement:
          Serial.print("absolute pressure: ");
          Serial.print(P,2);
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");     
          
  dataString = datetime + ", " + String(concentration) + ", " + String(P) + ", " + String(T);
  Serial.println(dataString);
  // if the file is available, write to it:
  String filename = "GreePPDE.txt";
  Serial.println(filename);
// Open log file
  File logfile = SD.open(filename, FILE_WRITE);
  
  if (logfile) {
    logfile.println(dataString);
    // logfile.close(); Don't seem to have to close the file...
    // print to the serial port too:
    Serial.println(dataString);

    logfile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening data file");
  }
}
