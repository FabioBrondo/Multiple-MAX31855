/* 
This code has been written on the base of the one used in the video at the following link:
https://www.youtube.com/watch?v=qub3yzqEwek
All the rights to the original developer.
*/


#include "MAX31855.h" //https://github.com/RobTillaart/MAX31855_RT library
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
  int i;


  for(i=0; i<max31855_num; i++)
  {
    //Setta lo stato di ingressi / uscite 
    max31855s[i].begin();                             
  }

  Serial.begin(9600);
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
    //Comando di lettura al sensore
    max31855s[i].read();
    //Conversione del dato
    value = max31855s[i].getTemperature();
    //Se ti serve internal puoi usare riga seguente
    //max31855s[i].getInternal();
    if(mov_avgs[i]==-100) mov_avgs[i] = value;
    mov_avgs[i] = mov_avg_alpha*value + (1-mov_avg_alpha)*mov_avgs[i];
    Serial.print(round(mov_avgs[i]));
    Serial.print("\t");
  }

  Serial.println();
}