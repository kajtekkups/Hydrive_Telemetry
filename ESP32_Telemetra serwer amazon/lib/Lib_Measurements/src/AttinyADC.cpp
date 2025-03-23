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
#include "Pin_Defines.hpp"

#define CONVERSION_NOT_COMPLETE 0
#define CONVERSION_COMPLETE 1


/**************************************************************************/
/*!
    @brief  Sets up the HW (reads coefficients values, etc.)

    @param i2c_addr I2C address of device
    @param wire I2C bus

    @return true if successful, otherwise false
*/
/**************************************************************************/
bool AttinyADC::begin(uint8_t i2c_addr, TwoWire *wire) {
  m_i2c_dev = new Attiny_I2CDevice(i2c_addr, wire);
  if(m_i2c_dev->begin()){
    initialized = true;
  }else{
    initialized = false;
  }
  return initialized;
}



/**************************************************************************/
/*!
    @brief  Gets a single-ended ADC reading from the specified channel

    @param channel ADC channel to read

    @return the ADC reading
*/
/**************************************************************************/
int16_t AttinyADC::readAdcSingleEnded(uint8_t channel) {

  if(!choseChannel(channel)) return CONVERSION_NOT_COMPLETE;
  return getLastConversionResults(channel);
}


/**************************************************************************/
/*!
    @brief  This function reads the last conversion
            results without changing the config value.

    @return the last ADC reading
*/
/**************************************************************************/
int16_t AttinyADC::getLastConversionResults(uint8_t channel) {
  // Read the conversion results
  return readRegister(REG_POINTER_CONV_VALUE);
}


/**************************************************************************/
/*!
    @brief  Gets messurements of current and voltage from the module

    @param referenses to return value variables
*/
/**************************************************************************/
void AttinyADC::getMessurements(int16_t& voltage, int16_t& current){
  if(initialized){
    voltage = readAdcSingleEnded(ADS_VOLT);
    current = readAdcSingleEnded(ADS_CURRENT);
  }
  else{
    voltage = 0;
    current = 0;
  }
}


/**************************************************************************/
/*!
    @brief  Gets messurements of hydrogen flow from the module

    @param referenses to return value variable
*/
/**************************************************************************/
int16_t AttinyADC::getMessurements(){
  int16_t flow;
  if(initialized){
    flow = readAdcSingleEnded(ADS_FLOW);
  }
  else{
    flow = 0;
  }
  return flow;
}


/**************************************************************************/
/*!
    @brief  chosing pin for ADC meassurement

    @param channel value
*/
/**************************************************************************/
uint8_t AttinyADC::choseChannel(uint8_t channel) {
  // Write config register to the ADC module
  writeRegister(REG_POINTER_CONFIG, channel);
  delay(1);
  uint8_t i = 0;
  while(!conversionComplete()){
    if(i == 5) break;
    i++;
    delay(1);
  }

  if(i == 5){
    return CONVERSION_NOT_COMPLETE;
  }  else{
    return CONVERSION_COMPLETE;
  }
}


/**************************************************************************/
/*!
    @brief  checks if attiny ADC conversation is complete
*/
/**************************************************************************/
uint8_t AttinyADC::conversionComplete(){
  return readRegister(REG_POINTER_CONVERT);
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
  m_i2c_dev->write(buffer, 2);
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
