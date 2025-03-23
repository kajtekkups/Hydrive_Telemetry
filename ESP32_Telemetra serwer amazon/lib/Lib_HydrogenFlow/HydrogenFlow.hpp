#ifndef HYDROGEN_FLOW_HPP
#define HYDROGEN_FLOW_HPP

#include "AttinyADC.h"

#define HYDROGEN_FLOW_MODULE 0x69 // addr floating or gnd

class HydrogenFlow{
public:
    void begin();
    float calculateFlow();
    float computeAdcConverterVolts(int16_t counts);
private:
    AttinyADC adc_module;
};

extern HydrogenFlow hydrogen_flow_instance;

#endif //HYDROGEN_FLOW_HPP