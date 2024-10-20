#include "ElecticalMeasurements.hpp"

// list of ADC converters
AttinyADC ads[ADC_number];  

void init_ADC(){
  pinMode(ADC_STATE_LED, OUTPUT);

  for(uint32_t i = 0; i < ADC_number; i++){
    if (!ads[i].begin(adresses[i])) {
      String messege = "connection error ADS " + String(i);
      Serial.println(messege);
      digitalWrite(ADC_STATE_LED, HIGH);
    }
  }
}


// Sensor measures current in -50A - 50A range
// 0A corresponds to half of ADCs reference 
// voltage (2.5V for 5V reference)
float CalculateAmp(float Measure_VT){

  float normalized_measure = Measure_VT - 2.5;
  normalized_measure = normalized_measure - MEASUREMENT_ERROR;
  
  float Amp = normalized_measure / ACS758_SENSITIVITY_DEFAULT;

  return Amp; 
  }

//Voltage divider devides in a 1/10 ratio, there is also constant 8% error 
float CalculateVolt(float Measure){
  float Volt;
  Volt = Measure * 10;
  Volt = Volt * 1.11;

  return Volt;
}

