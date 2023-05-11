#ifndef ADC_HPP
#define ADC_HPP

#include <Arduino.h>
#include "../include/Adafruit_ADS1X15.h"

void print_ADC_value(Adafruit_ADS1015* ads, int chanel){

    int16_t results = ads->readADC_SingleEnded(chanel);
    Serial.print(results); Serial.print(" ("); Serial.print(ads->computeVolts(results)); Serial.println("V)");
}

    

 
#endif