#include "HydrogenFlow.hpp"


void HydrogenFlow::setup(){

    if (!adc_module.begin(HYDROGEN_FLOW_MODULE)) {
      String messege = "connection error ADS " + String(HYDROGEN_FLOW_MODULE);
      Serial.println(messege);
    }
}

float HydrogenFlow::calculateFlow(uint16_t measure_volt_value){
    float flow;
    flow = measure_volt_value * 4.0;

    return flow;

}