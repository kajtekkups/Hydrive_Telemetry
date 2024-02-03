#include "pomiary_elektryczne.hpp"


float CalculateAmp(float Measure){
  float Amp; 
  
  //amperomierz mierzy w zakresie -50 - 50 A, ale my liczymy wynik w zakresie 0 - 100
  float pomiar_znormalizowany = Measure - (ROZDZIELCZOSC_PRZETWORNIKA_ADC/2);  // czujnik mierzy w zakresie -50 - 50 (rozdzielczosc 1666,6)
  float rozdzielczosc_bitu = NAPIECIE_REFERENCYJNE/ROZDZIELCZOSC_PRZETWORNIKA_ADC;  // rozdzielczosc JEDNEGO bitu przetwornika ADC 

  Amp = (pomiar_znormalizowany * rozdzielczosc_bitu) / ACS758_SENSITIVITY_DEFAULT - BLAD_POMIARU;
  
  return Amp; 
  }


float CalculateVolt_for_meter(float Measure){
  float Volt;

  Volt = Measure * 10;

  return Volt;
}