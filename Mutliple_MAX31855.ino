/* 

This code has been written on the base of the one used in the video at the following link:

https://www.youtube.com/watch?v=qub3yzqEwek

All the rights to the original developer.

*/


#include "MAX31855.h"
#include <SPI.h>

const int     spi_miso_pin      = 12;
const int     spi_sck_pin       = 13;
const byte    max31855_num      = 2;
const int     max31855_cs_pins[max31855_num] = {  3,4 };
SPISettings   max31855_spi(1000000, MSBFIRST, SPI_MODE1);               

MAX31855 max31855s[max31855_num]=                                       
{
  MAX31855(spi_sck_pin, max31855_cs_pins[0], spi_miso_pin),
  MAX31855(spi_sck_pin, max31855_cs_pins[1], spi_miso_pin)

};

void setup()
{
  Serial.begin(115200); //Inizializzazine comunicazione seriale;;
  Serial.print("Starting...");
  Serial.println();
	int i;
	for (i = 0; i < max31855_num; i++)
	{
		max31855s[i].begin();	//Inizializzazione dei moduli MAX31855
    delay(200);
    int state;
    max31855s[i].read();
    state = max31855s[i].getStatus();
    switch (state)
    {
      case 0:
      {
      Serial.print("Thermocouple ");
      Serial.print( i +1 );
      Serial.print(" working properly!\n");
      Serial.println();
      break;
      }
      case 1:
      {      
      Serial.print("Thermocouple ");
      Serial.print( i +1 );
      Serial.print(" open circuit!\n");
      Serial.println();
      break;
      }
      case 2:
      {      
      Serial.print("Thermocouple ");
      Serial.print( i +1 );
      Serial.print(" short to GND!\n");
      Serial.println();
      break;
      }
      case 3:
      {
      Serial.print("Thermocouple ");
      Serial.print( i +1 );
      Serial.print(" short to VCC!\n");
      Serial.println();
      break;
      }
      case 7:
      {
      Serial.print("Thermocouple ");
      Serial.print( i +1 );
      Serial.print(" generic error, check wiring \n");
      Serial.println();
      break;
      }
      case 128:
      {
      Serial.print("Thermocouple ");
      Serial.print( i +1 );
      Serial.print(" no read done yet,check wiring!\n");
      Serial.println();
      break;
      }
      case 129:
      {
      Serial.print("Thermocouple ");
      Serial.print( i +1 );
      Serial.print(" no communication,check wiring!\n");
      Serial.println();
      break;
      }
    
    
    }
	}
		
}

void loop()
{
  const float mov_avg_alpha = 0.1;
  static float mov_avgs[max31855_num] = {-100, -100, };
  double  value;
  int     i;
  delay (85);

  
  for(i=0; i<max31855_num; i++)
  {
    value = max31855Read(max31855_cs_pins[i]);
    if(mov_avgs[i]==-100) mov_avgs[i] = value;
    mov_avgs[i] = mov_avg_alpha*value + (1-mov_avg_alpha)*mov_avgs[i];
    Serial.print(round(mov_avgs[i]));
    Serial.print("\t");
  }

  Serial.println();
}

