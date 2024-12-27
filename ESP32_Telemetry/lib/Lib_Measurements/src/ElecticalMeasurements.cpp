#include "ElecticalMeasurements.hpp"

// list of ADC converters
AttinyADC ads[ADC_number];  

ElectricalMeassurements electrical_meassurements_instance;

/**************************************************************************/
/*!
    @brief  Initialize all I2C ADC converters

*/
/**************************************************************************/
void ElectricalMeassurements::begin(){
  pinMode(ADC_STATE_LED, OUTPUT);

  for(uint32_t i = 0; i < ADC_number; i++){
    if (!ads[i].begin(adresses[i])) {
      String messege = "connection error ADS " + String(i);
      Serial.println(messege);
      digitalWrite(ADC_STATE_LED, HIGH);
    }
  }
}


/**************************************************************************/
/*!
    @brief  Converts voltage measure to Amps. ACS has 100A resolution (-50A - 50A)

    @param measured value [V]

    @return ACS sensor value [A]
*/
/**************************************************************************/
// Sensor measures current in -50A - 50A range
// 0A corresponds to half of ADCs reference 
// voltage (2.5V for 5V reference)
float ElectricalMeassurements::calculateAmp(float Measure_VT){

  float normalized_measure = Measure_VT - 2.5;
  // normalized_measure = normalized_measure - MEASUREMENT_ERROR;
  
  float Amp = normalized_measure / ACS758_SENSITIVITY_DEFAULT;

  return Amp; 
  }


/*!
    @brief  Converts voltage measure to voltage in electrical system.  

    @param measured value [V]

    @return sensor value [V]
*/
/**************************************************************************/
//Voltage divider devides in a 1/10 ratio, there is also constant 8% error 
float ElectricalMeassurements::calculateVolt(float Measure){
  float Volt;
  Volt = Measure * 11;

  return Volt;
}