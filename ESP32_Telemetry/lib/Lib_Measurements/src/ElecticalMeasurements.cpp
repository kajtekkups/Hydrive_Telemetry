#include "ElecticalMeasurements.hpp"


void AdsNodeInterface::begin(AttinyADC* node_ads, uint8_t initialized, uint8_t current_pin, uint8_t voltage_pin){
  _node_ads = node_ads; 
  _initialized = initialized;
  _current_pin = current_pin;
  _voltage_pin = voltage_pin;
}


void AdsNodeInterface::get_messurements(int16_t& voltage, int16_t& current){
  if(_initialized){
    voltage = _node_ads->readADC_SingleEnded(_voltage_pin);
    current = _node_ads->readADC_SingleEnded(_current_pin);
  }
  else{
    voltage = 0;
    current = 0;
  }
}

// list of ADC converters
AttinyADC ads[ADC_NUMBER];  
AdsNodeInterface ads_objects[ADC_NUMBER];  

void init_ADC(){
  pinMode(ADC_STATE_LED, OUTPUT);

  if (!ads[0].begin(ADS_ADRESS_ZA_OGNIWEM)) {
    Serial.println("blad polaczenia z ADS 0");
    digitalWrite(ADC_STATE_LED, HIGH);
    ads_objects[0].begin(&ads[0], 0, ADS_0_CURRENT, ADS_0_VOLT);
  }
  else{
    ads_objects[0].begin(&ads[0], 1, ADS_0_CURRENT, ADS_0_VOLT);
  }
  
  if (!ads[1].begin(ADS_ADRESS_PRZED_PRZETWORNICAMI)) {
    Serial.println("blad polaczenia z ADS 1");
    digitalWrite(ADC_STATE_LED, HIGH);
    ads_objects[1].begin(&ads[1], 0, ADS_1_CURRENT, ADS_1_VOLT);
  }
  else{
    ads_objects[1].begin(&ads[1], 1, ADS_1_CURRENT, ADS_1_VOLT);
  }

  if (!ads[2].begin(ADS_ADRESS_ZA_PRZETWORNICAMI)) {
    Serial.println("blad polaczenia z ADS 2");
    digitalWrite(ADC_STATE_LED, HIGH);
    ads_objects[2].begin(&ads[2], 0, ADS_2_CURRENT, ADS_2_VOLT);
  }
  else{
    ads_objects[2].begin(&ads[2], 1, ADS_2_CURRENT, ADS_2_VOLT);
  }

  if(!ads[3].begin(ADS_ADRESS_KONDENSATORY)) {
    Serial.println("blad polaczenia z ADS 3");
    digitalWrite(ADC_STATE_LED, HIGH);
    ads_objects[3].begin(&ads[3], 0, ADS_3_CURRENT, ADS_3_VOLT);
  }
  else{
    ads_objects[3].begin(&ads[3], 1, ADS_3_CURRENT, ADS_3_VOLT);
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

