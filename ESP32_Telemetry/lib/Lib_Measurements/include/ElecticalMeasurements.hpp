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
    ADC modules management
    -----------------------------------------------------------------------*/
#define ADS_ADRESS_BEHIND_CELL 0x48 // addr floating or gnd
#define ADS_ADRESS_BEFORE_CONVERTERS 0x4a //addr to sda
#define ADS_ADRESS_AFTER_CONVERTERS 0x49 // addr to vdd
#define ADS_ADRESS_SUPERCAPACITORS 0x4b // addr to scl

constexpr uint16_t adresses[] = {
    ADS_ADRESS_BEHIND_CELL,
    ADS_ADRESS_BEFORE_CONVERTERS,
    ADS_ADRESS_AFTER_CONVERTERS,
    ADS_ADRESS_SUPERCAPACITORS
};

constexpr uint16_t ADC_number = sizeof(adresses)/sizeof(adresses[0]);

extern AttinyADC ads[ADC_number];
/*=========================================================================*/

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