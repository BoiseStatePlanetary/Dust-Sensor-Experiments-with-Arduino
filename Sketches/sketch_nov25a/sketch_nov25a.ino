#include <SPI.h>
#include <SD.h>

/* 2019 Nov 25 - 
 * From https://www.reichelt.com/magazin/en/how-to-code-a-temperature-log-with-the-arduino-uno/
 */

int SELECTED_CHIP = 4;
int count = 0;

void setup() {
  Serial.begin(9600);
  if (SD.begin(SELECTED_CHIP)) {
    Serial.println("SD-Card initialized.");
  } else {
    Serial.println("SD-Card failed or is missing.");
  }
}

void loop() {
  String line = String("Hallo Welt! #") + String(count);

  Serial.println(line);

//  writeToSD(line);
  count = count + 1;
//  delay(1000);
}

void writeToSD(String line) {
  File dataFile = SD.open("test.csv", FILE_WRITE);

  if (dataFile) {
    dataFile.println(line); // Write onto SD card
    dataFile.close();       // Close file
    Serial.println(line);
  } else {
    Serial.println("Here's a change to the sketch!");
  }
}
