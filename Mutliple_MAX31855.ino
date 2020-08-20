/* 

This code has been written on the base of the one used in the video at the following link:

https://www.youtube.com/watch?v=qub3yzqEwek

All the rights to the original developer.

*/


#include <SPI.h>

const int     spi_miso_pin      = 12;
const int     spi_sck_pin       = 13;
const byte    max31855_num      = 2;
const int     max31855_cs_pins[max31855_num] = {  3,4 };
SPISettings   max31855_spi(1000000, MSBFIRST, SPI_MODE1);               


void setup()
{
  int i;

  
  for(i=0; i<max31855_num; i++)
  {
    pinMode(max31855_cs_pins[i],OUTPUT);                                
    digitalWrite(max31855_cs_pins[i],HIGH);                             
  }

  
  SPI.begin();
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
    value = max31855Read(max31855_cs_pins[i]);
    if(mov_avgs[i]==-100) mov_avgs[i] = value;
    mov_avgs[i] = mov_avg_alpha*value + (1-mov_avg_alpha)*mov_avgs[i];
    Serial.print(round(mov_avgs[i]));
    Serial.print("\t");
  }

  Serial.println();
}



/*Modified according to Adafruit-MAX31855 library
* https://github.com/adafruit/Adafruit-MAX31855-library/blob/47d42486e0d625695806c39dbc995b407f0d66a7/Adafruit_MAX31855.cpp#L121
*/
float max31855Read(int max31855_cs_pin)
{

  uint8_t buf[4];
  int32_t v = 0;

  SPI.beginTransaction(max31855_spi);
  digitalWrite(max31855_cs_pin, LOW);
  SPI.transfer(&buf, 4); //MAX31855 memory map is 32bit
  digitalWrite(max31855_cs_pin, HIGH);
  SPI.endTransaction();

  /*from buffer of 4 bytes to uint32_t */
  v = buf[0];
  v <<= 8;
  v |= buf[1];
  v <<= 8;
  v |= buf[2];
  v <<= 8;
  v |= buf[3];

  if (v & 0x7)
  {
    // uh oh, a serious problem!
    return NAN;
  }

  if (v & 0x80000000)
  {
    // Negative value, drop the lower 18 bits and explicitly extend sign bits.
    v = 0xFFFFC000 | ((v >> 18) & 0x00003FFF);
  }
  else
  {
    // Positive value, just drop the lower 18 bits.
    v >>= 18;
  }

  double centigrade = v;

  // LSB = 0.25 degrees C
  centigrade *= 0.25;
  return centigrade;
}
