
#define        COV_RATIO                       0.2            //ug/mmm / mv
#define        NO_DUST_VOLTAGE                 400            //mv
#define        SYS_VOLTAGE                     5000           

int pin = 8;
unsigned long duration;
unsigned long very_first_starttime;
unsigned long starttime;
unsigned long sampletime_ms = 1000;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

/*
I/O define
*/
const int iled = 7;                                            //drive the led of sensor
const int vout = 0;                                            //analog input

/*
variable
*/
float density, voltage;
int   adcvalue;

/*
private function
*/
int Filter(int m)
{
  static int flag_first = 0, _buff[10], sum;
  const int _buff_max = 10;
  int i;
  
  if(flag_first == 0)
  {
    flag_first = 1;

    for(i = 0, sum = 0; i < _buff_max; i++)
    {
      _buff[i] = m;
      sum += _buff[i];
    }
    return m;
  }
  else
  {
    sum -= _buff[0];
    for(i = 0; i < (_buff_max - 1); i++)
    {
      _buff[i] = _buff[i + 1];
    }
    _buff[9] = m;
    sum += _buff[9];
    
    i = sum / 10.0;
    return i;
  }
}


void setup() 
{
  Serial.begin(9600);
  pinMode(pin, INPUT);
  very_first_starttime = millis();
  starttime = millis();//get the current time;

  pinMode(iled, OUTPUT);
  digitalWrite(iled, LOW);                                     //iled default closed
    
}

void loop() 
{
    duration = pulseIn(pin, LOW);
    lowpulseoccupancy = lowpulseoccupancy+duration;

    if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
    {
        ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
        concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve

        // Based on poly-fit to data from https://journals.plos.org/plosone/article?id=10.1371/journal.pone.0137789
//        concentration = 0.0008*pow(ratio, 4) - 0.049*pow(ratio, 3) + 0.9326*pow(ratio, 2) - 1.6215*ratio;
        
        
      /*
       * get adcvalue
       */
      digitalWrite(iled, HIGH);
      delayMicroseconds(280);
      adcvalue = analogRead(vout);
      digitalWrite(iled, LOW);
  
      adcvalue = Filter(adcvalue);
  
      /*
       * covert voltage (mv)
       */
       voltage = (SYS_VOLTAGE / 1024.0) * (double) adcvalue * 11.0;
       
      /*
      * voltage to density
      */
      if(voltage >= NO_DUST_VOLTAGE)
      {
        voltage -= NO_DUST_VOLTAGE;
        density = voltage * COV_RATIO;
      }
      else
        density = 0;
      
      Serial.print("GPU, PPD: ");
      Serial.print(density);
      Serial.print(", ");
      Serial.print(concentration);
      Serial.print("\n");
      
      lowpulseoccupancy = 0;
      starttime = millis();
    }

}
