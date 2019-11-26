// demo of Starter Kit V2.0 - Grove Temperature Sensor
//

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <rgb_lcd.h>

byte DEGREE_SYMBOL = 0;
byte degree[8] = {
  0b00000,
  0b00010,
  0b00101,
  0b00010,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

const float coldest = 18;
const float hottest = 30;

const int pinTemp = A0;      // pin of temperature sensor

float temperature;
int B=3975;                  // B value of the thermistor
float resistance;

rgb_lcd lcd;

void setup()
{
    Serial.begin(9600);     //Baud rate for the serial communication of Arduino
    pinMode(A0,INPUT);      //Setting the A0 pin as input pin to take data from the temperature sensor 

    lcd.begin(16, 2); // Establish connection to display and initialise display - 2 lines with 16 characters respectively
    lcd.createChar(DEGREE_SYMBOL, degree); // Register the new "°" symbol with the display
    
}

void loop()
{
    int val = analogRead(pinTemp);                               // get analog value
    resistance=(float)(1023-val)*10000/val;                      // get resistance
    temperature=1/(log(resistance/10000)/B+1/298.15)-273.15;     // calc temperature

    int red = limit(255 * (temperature - coldest) / (hottest - coldest));
    int green = 0;
    int blue = limit(255 * (hottest - temperature) / (hottest - coldest));

    updateDisplay(red, green, blue, temperature);
    delay(1000);          // delay 1s
}

void updateDisplay(int red, int green, int blue, float temperature) {

  // From https://www.reichelt.com/magazin/en/how-to-code-a-temperature-log-with-the-arduino-uno/

  lcd.setRGB(red, green, blue);
  
  lcd.setCursor(0, 0);
  lcd.print("T: ");
  lcd.print(temperature);
  lcd.write(DEGREE_SYMBOL);
  lcd.print("C");

}

float limit(float color) { // colour values need to be in the area between 0..255

  if (color < 0) {
    return 0;
  }
  else if (color > 255) {
    return 255;
  }
  else {
    return color;
  }
}
