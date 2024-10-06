/**************************************************************************/
/*!
    @file     AttinyADC.cpp
    @author   K.Townsend (Adafruit Industries)

    @mainpage Adafruit ADS1X15 ADC Breakout Driver

    @section intro_sec Introduction

    This is a library for the Adafruit ADS1X15 ADC breakout boards.

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

    @section author Author

    Written by Kevin "KTOWN" Townsend for Adafruit Industries.

    @section  HISTORY

    v1.0  - First release
    v1.1  - Added ADS1115 support - W. Earl
    v2.0  - Refactor - C. Nelson

    @section license License

    BSD license, all text here must be included in any redistribution
*/
/**************************************************************************/
#include "AttinyADC.h"


/**************************************************************************/
/*!
    @brief  Sets up the HW (reads coefficients values, etc.)

    @param i2c_addr I2C address of device
    @param wire I2C bus

    @return true if successful, otherwise false
*/
/**************************************************************************/
bool AttinyADC::begin(uint8_t i2c_addr, TwoWire *wire) {
  m_i2c_dev = new Adafruit_I2CDevice(i2c_addr, wire);
  return m_i2c_dev->begin();
}



/**************************************************************************/
/*!
    @brief  Gets a single-ended ADC reading from the specified channel

    @param channel ADC channel to read

    @return the ADC reading
*/
/**************************************************************************/
int16_t AttinyADC::readADC_SingleEnded(uint8_t channel) {

  choseChannel(channel);
  delay(10);  // Wait for the conversion to complete

  // Serial.print("ADS");
  // Serial.print(channel);
  // Serial.println("utracony");

  // Read the conversion results
  return getLastConversionResults();
}


/**************************************************************************/
/*!
    @brief  This function reads the last conversion
            results without changing the config value.

    @return the last ADC reading
*/
/**************************************************************************/
int16_t AttinyADC::getLastConversionResults() {
  // Read the conversion results
  return (int16_t)readRegister(REG_POINTER_CONVERT);
}


/**************************************************************************/
/*!
    @brief  Converts ADC reading into volts.

    @param counts the ADC reading in raw counts

    @return the ADC reading in volts
*/
/**************************************************************************/
float AttinyADC::computeVolts(int16_t counts) {
  // see data sheet Table 3
  float fsRange = 5.0f;
  return counts * (fsRange / ADC_RESOLUTION);
}

/**************************************************************************/
/*!
    @brief  chosing pin for ADC meassurement

    @param channel value
*/
/**************************************************************************/
void AttinyADC::choseChannel(uint8_t channel) {
  // Write config register to the ADC
  writeRegister(REG_POINTER_CONFIG, channel);
}



/**************************************************************************/
/*!
    @brief  Writes 16-bits to the specified destination register

    @param reg register address to write to
    @param value value to write to register
*/
/**************************************************************************/
void AttinyADC::writeRegister(uint8_t reg, uint8_t value) {
  buffer[0] = reg;
  buffer[1] = value;
  m_i2c_dev->write(buffer, 3);
}

/**************************************************************************/
/*!
    @brief  Read 16-bits from the specified destination register

    @param reg register address to read from

    @return 16 bit register value read
*/
/**************************************************************************/
uint16_t AttinyADC::readRegister(uint8_t reg) {
  buffer[0] = reg;
  m_i2c_dev->write(buffer, 1);
  m_i2c_dev->read(buffer, 2);
  return ((buffer[0] << 8) | buffer[1]);
}
