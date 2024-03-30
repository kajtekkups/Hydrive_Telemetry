#ifndef ADC_HPP
#define ADC_HPP

#include <Arduino.h>
#include "MQTT_Communication.hpp"
#include "Adafruit_ADS1X15.h"
#include "Pin_Defines.hpp"

/**** stale zwiazane z pomiarami *****/
#define NAPIECIE_REFERENCYJNE 5
#define ACS758_SENSITIVITY_DEFAULT 0.04  // wrazliwosc czujnika wyrazona w V/A
#define BLAD_POMIARU 0.65

/**** Analog Digital Converter modules *****/
#define LICZBA_PRZETWORNIKOW 4  //przy zmianie liczby przetwornikow nalezy zmienic funkcje init_ADC


#define ADS_ADRESS_ZA_OGNIWEM 0x48 // addr floating or gnd
#define ADS_ADRESS_PRZED_PRZETWORNICAMI 0x4a //addr to sda
#define ADS_ADRESS_ZA_PRZETWORNICAMI 0x49 // addr to vdd
#define ADS_ADRESS_KONDENSATORY 0x4B // addr to scl

extern Adafruit_ADS1015 ads[LICZBA_PRZETWORNIKOW];

struct {
  unsigned long time;
  float pomiar_VT[LICZBA_PRZETWORNIKOW];
  float pomiar_I[LICZBA_PRZETWORNIKOW];

} dane_elektryczne;


class AdsNodeInterface{
  public:

    void begin(Adafruit_ADS1015* node_ads, uint8_t initialized, uint8_t current_pin, uint8_t voltage_pin);

    // zbierz pomiary i zwroc w kolejnosci napiecie - prad
    void get_messurements(int16_t& voltage, int16_t& current);

  private:
    uint8_t _initialized;
    Adafruit_ADS1015* _node_ads;
    uint8_t _current_pin;
    uint8_t _voltage_pin;
};


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
float CalculateVolt(float Measure);
 

/**** zbiera pomiary i zapisuje je w strukturze dane_elektryczne *****/
void Collect_electrical_data();


/**** przesyla pomiary zapisane w strukturze dane_elektryczne *****/
void Send_save_electrical_data();


#endif