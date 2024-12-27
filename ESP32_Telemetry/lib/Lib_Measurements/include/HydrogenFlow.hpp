#ifndef HYDROGEN_FLOW_HPP
#define HYDROGEN_FLOW_HPP

#include "AttinyADC.h"

#define HYDROGEN_FLOW_MODULE 0x69 // addr floating or gnd

class HydrogenFlow{
public:
    void setup();
    float calculateFlow(uint16_t measure_volt_value);
private:
    AttinyADC adc_module;
};

#endif //HYDROGEN_FLOW_HPP
