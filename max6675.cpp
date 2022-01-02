// this library is public domain. enjoy!
// https://learn.adafruit.com/thermocouple/
// NDC added fixed precision routine returning integer temperature in Celsius to be compatible with our Sensor library.
// My original based on an earlier version at www.ladyada.net/learn/sensors/thermocouple

#include "max6675.h"
// Note references to delay are available without needing explicit include statement.

/**************************************************************************/
/*!
    @brief  Initialize a MAX6675 sensor
    @param   SCLK The Arduino pin connected to Clock
    @param   CS The Arduino pin connected to Chip Select
    @param   MISO The Arduino pin connected to Data Out
*/
/**************************************************************************/
MAX6675::MAX6675(int8_t SCLK, int8_t CS, int8_t MISO) {
  sclk = SCLK;
  cs = CS;
  miso = MISO;

  // define pin modes
  pinMode(cs, OUTPUT);
  pinMode(sclk, OUTPUT);
  pinMode(miso, INPUT);

  // de-select
  digitalWrite(cs, HIGH);
  // NDC: initial value is high we pulse low to read in spiread() routine
  digitalWrite(sclk, HIGH);

}

/**************************************************************************/
/*!
    @brief  Read the Celsius temperature
    @returns Temperature in C or NAN on failure!
*/
/**************************************************************************/
float MAX6675::readCelsius(void) {

  uint16_t v;

  digitalWrite(cs, LOW);
  // shorter delay than previous which improves read time.
  delayMicroseconds(10);

  v = spiread();
  v <<= 8;
  v |= spiread();

  digitalWrite(cs, HIGH);

  if (v & 0x4) {
    // uh oh, no thermocouple attached!
    return NAN;
    // return -100;
  }

  v >>= 3;

  return v * 0.25;
}

/**************************************************************************/
/*!
    @brief  Read the Celsius temperature
    @returns integer Temperature in C
	or fixed very negative constant value OPENCIRCUIT on failure!
*/
// MSB bit 14, LSB bit 3.  T/C input status bit 2
// NDC> added integer version of the input, resolution one degree
// returns 0...1023 in-range, or -300 if not connected.
int16_t MAX6675::readC(void) {

  uint16_t v;

  digitalWrite(cs, LOW);
  // shorter delay than previous which improves read time.
  delayMicroseconds(10);

  v = spiread();
  v <<= 8;
  v |= spiread();

  digitalWrite(cs, HIGH);

  if (v & 0x4) {
    // uh oh, no thermocouple attached!
    return OPENCIRCUIT;
	// Note NAN is not valid for integer type
  }

  // remove status bits and the 0.25 resolution leaving data in C
  v >>= 5;

  return (int16_t) v;
}

/**************************************************************************/
/*!
    @brief  Read the Fahenheit temperature
    @returns Temperature in F or NAN on failure!
*/
/**************************************************************************/
float MAX6675::readFahrenheit(void) {
	return readCelsius() * 9.0 / 5.0 + 32;
}


// private function for SPI read on digital pins
// requires CS to be activated outside this routine.  It controls SCLK
byte MAX6675::spiread(void) {
  int i;
  byte d = 0;

  for (i = 7; i >= 0; i--) {
    digitalWrite(sclk, LOW);
    delayMicroseconds(10);
    if (digitalRead(miso)) {
      // Bit will remain 0 if condition is false
      d |= (1 << i);
    }

    digitalWrite(sclk, HIGH);
    delayMicroseconds(10);
  }

  return d;
}
