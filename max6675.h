
/* this library is based on the AdaFruit public domain. enjoy!
 * https://learn.adafruit.com/thermocouple/
 * I've updated this based on work I did no a previous public domain library at www.ladyada.net/learn/sensors/thermocouple
 * This will interface over digital pins due to low data rate no need for HW SPI.
 * NDC added an integer version to avoid floating point operations and to work with our Sensor class
 */

#ifndef ADAFRUIT_MAX6675_H
#define ADAFRUIT_MAX6675_H

#include "Arduino.h"

/**************************************************************************/
/*!
    @brief  Class for communicating with thermocouple sensor
*/
/**************************************************************************/
class MAX6675 {
public:
  MAX6675(int8_t SCLK, int8_t CS, int8_t MISO);

  /* Does in-line query of SPI sensor and returns value converting to Celsius as float */
  float readCelsius(void);
  /* Does in-line query of SPI sensor and returns value converting to Fahrenheit as float */
  float readFahrenheit(void);
  /* Does in-line query of SPI sensor and returns value converting to Celsius as 16-bit signed integer
   * Returns fixed value on failure  */
  int16_t readC(void);

  // a value to return when we detect open circuit
  const int16_t OPENCIRCUIT = -300;

private:
  int8_t sclk, miso, cs;
  uint8_t spiread(void);
};

#endif
