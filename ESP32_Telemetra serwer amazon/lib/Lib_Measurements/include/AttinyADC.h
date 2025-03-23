#ifndef ATTINY_H
#define ATTINY_H

#include <Attiny_I2CDevice.h>
#include <Arduino.h>
#include <Wire.h>

/*=========================================================================
    POINTER REGISTER
    -----------------------------------------------------------------------*/
#define REG_POINTER_CONV_VALUE (0x03)      ///< ADC Conversion value
#define REG_POINTER_CONVERT (0x00)   ///< Conversion
#define REG_POINTER_CONFIG (0x01)    ///< Configuration
/*=========================================================================*/


#define ADC_RESOLUTION 1024.0f
#define FS_RANGE 5.0f
#define MASTER_ADDRESS (0x50) /// 1010 000


/**************************************************************************/
/*!
    @brief  Sensor driver for the Adafruit ADS1X15 ADC breakouts.
*/
/**************************************************************************/
class AttinyADC {
protected:
  // Instance-specific properties
  Attiny_I2CDevice *m_i2c_dev; ///< I2C bus device

public:
  bool begin(uint8_t i2c_addr, TwoWire *wire = &Wire);
  
  int16_t readAdcSingleEnded(uint8_t channel);
  uint8_t choseChannel(uint8_t mux);
  uint8_t conversionComplete();
  int16_t getLastConversionResults(uint8_t channel);

  void getMessurements(int16_t& voltage, int16_t& current);
  int16_t getMessurements();

private:
  void writeRegister(uint8_t reg, uint8_t value);
  uint16_t readRegister(uint8_t reg);
  uint8_t buffer[2];
  bool initialized;
};


#endif //ATTINY_H