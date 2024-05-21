#ifndef OBSLUGA_DANYCH_HPP
#define OBSLUGA_DANYCH_HPP

#include "Lib_microSD.hpp"
#include "MQTT_Communication.hpp"
#include "HallSensor.h"
#include "pomiary_elektryczne.hpp"

struct {
  unsigned long time;
  unsigned long czas_pomiaru;
  float pomiar_VT[LICZBA_PRZETWORNIKOW];
  float pomiar_I[LICZBA_PRZETWORNIKOW];
  float velocity;

} dane_pomiarowe;


/**** zbiera pomiary i zapisuje je w strukturze dane_elektryczne *****/
void Collect_data();

/**** przesyla pomiary zapisane w strukturze dane_elektryczne *****/
void Send_save_data();


#endif //OBSLUGA_DANYCH_HPP