#include "HydrogenFlow.hpp"


HydrogenFlow hydrogen_flow_instance;


void HydrogenFlow::begin(){

    if (!adc_module.begin(HYDROGEN_FLOW_MODULE)) {
      String messege = "connection error ADS " + String(HYDROGEN_FLOW_MODULE);
      Serial.println(messege);
    }
}

float HydrogenFlow::calculateFlow(){
    int16_t measure_volt_value = adc_module.getMessurements();
    float flow = computeAdcConverterVolts(measure_volt_value);
    flow = flow * 4.0;

    return flow;
}


float HydrogenFlow::computeAdcConverterVolts(int16_t counts) {
  // see data sheet Table 3
  return ((float)counts) * (FS_RANGE / ADC_RESOLUTION);
}