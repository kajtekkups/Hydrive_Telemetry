#ifndef ADC_HPP
#define ADC_HPP

#include <Arduino.h>
#include "MQTT_Communication.hpp"
#include "AttinyADC.h"
#include "Pin_Defines.hpp"


/*=========================================================================
    CONSTANTS
    -----------------------------------------------------------------------*/
#define ACS758_SENSITIVITY_DEFAULT 0.04  // sensor sensitivity [V/A]
#define MEASUREMENT_ERROR 0.04
/*=========================================================================*/


/*=========================================================================
    Number of ADC modules
    -----------------------------------------------------------------------*/
#define ADC_NUMBER 4  //if changeing this function, change init_ADC function
/*=========================================================================*/

#define ADS_ADRESS_ZA_OGNIWEM 0x48 // addr floating or gnd
#define ADS_ADRESS_PRZED_PRZETWORNICAMI 0x4a //addr to sda
#define ADS_ADRESS_ZA_PRZETWORNICAMI 0x49 // addr to vdd
#define ADS_ADRESS_KONDENSATORY 0x4b // addr to scl


class AdsNodeInterface{
  public:

    void begin(AttinyADC* node_ads, uint8_t initialized, uint8_t current_pin, uint8_t voltage_pin);

    void get_messurements(int16_t& voltage, int16_t& current);

  private:
    uint8_t _initialized;
    AttinyADC* _node_ads;
    uint8_t _current_pin;
    uint8_t _voltage_pin;
};

extern AttinyADC ads[ADC_NUMBER];
extern AdsNodeInterface ads_objects[ADC_NUMBER];


/**************************************************************************/
/*!
    @brief  Initialize all I2C ADC converters

*/
/**************************************************************************/
void init_ADC();


/**************************************************************************/
/*!
    @brief  Converts voltage measure to Amps. ACS has 100A resolution (-50A - 50A)

    @param measured value [V]

    @return ACS sensor value [A]
*/
/**************************************************************************/
float CalculateAmp(float Measure);


/*!
    @brief  Converts voltage measure to voltage in electrical system.  

    @param measured value [V]

    @return sensor value [V]
*/
/**************************************************************************/
float CalculateVolt(float Measure);
 


#endif