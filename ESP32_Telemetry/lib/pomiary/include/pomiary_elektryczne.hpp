#ifndef ADC_HPP
#define ADC_HPP

#include <Arduino.h>
#include "MQTT_Communication.hpp"
#include "Adafruit_ADS1X15.h"
#include "Pin_Defines.hpp"

/**** stale zwiazane z pomiarami *****/
#define ROZDZIELCZOSC_PRZETWORNIKA_ADC 1666.67 
#define NAPIECIE_REFERENCYJNE 5
#define ACS758_SENSITIVITY_DEFAULT 0.04  // wrazliwosc czujnika wyrazona w V/A
#define BLAD_POMIARU 0.65

/**** Analog Digital Converter modules *****/
#define LICZBA_POMIAROW 6
#define LICZBA_PRZETWORNIKOW 4

#define ADS_ADRESS_ZA_OGNIWEM 0x48 //Activate default ads (addr floating or gnd)
#define ADS_ADRESS_PRZED_PRZETWORNICAMI 0x4a // Activate ads1 (addr to sda)
#define ADS_ADRESS_ZA_PRZETWORNICAMI
#define ADS_ADRESS_KONDENSATORY

extern Adafruit_ADS1015 ads[LICZBA_PRZETWORNIKOW];

struct {
  unsigned long time;
  float pomiar_VT[LICZBA_POMIAROW];
  float pomiar_I[LICZBA_POMIAROW];

} dane_elektryczne;


/************************
 *  Inicjalizuje ADC lub zewnetrzne kontrolery ADC 
 * 
 *  Przetwornikow ADC na plytce ma byc 4, 
 *  ale nie wszystkie musza dzialac na raz, brak 
 *  jednego przetwornika nie wplywa na system
 * 
 ************************/
void init_ADC();


/************************
 * przelicza odczytana wartosc z adc na natezenie
 *  
 * rozdzielczosc modulu pomiaru pradu wynosi 100A natomiast rozdzielczość ADC 2048, 
 * jednak nasze napiecie referencyjne to 5V a ADC mierzy 0-6,144V,
 *  dlatego realna rozdzielczosc to 1666,67, co mozna policzyc z prostej proporcji:
 *             6,144 = 2048
 *                5  =   x
 *************************/
float CalculateAmp(float Measure);


/************************
 * przelicza wartosc otrzymana z przetwornika 
 * na zmierzone napiecie, dla przetwornika Grove 5/43 V
*************************/
float CalculateVolt_for_meter(float Measure);
 

/**** zbiera pomiary i zapisuje je w strukturze dane_elektryczne *****/
void Collect_electrical_data();


/**** przesyla pomiary zapisane w strukturze dane_elektryczne *****/
void Send_save_electrical_data();


#endif