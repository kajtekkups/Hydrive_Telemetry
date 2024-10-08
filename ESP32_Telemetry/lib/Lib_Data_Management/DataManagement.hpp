#ifndef OBSLUGA_DANYCH_HPP
#define OBSLUGA_DANYCH_HPP

#include "Lib_microSD.hpp"
#include "MQTT_Communication.hpp"
#include "HallSensor.h"
#include "ElecticalMeasurements.hpp"

struct {
  unsigned long time;
  unsigned long measurement_time;
  float voltage_measurement[ADC_NUMBER];
  float current_measurement[ADC_NUMBER];
  float velocity;

} measure_data;


/**** collects data and saces it to measure_data structure*****/
void Collect_data();

/**** saves and sends data from measure_data structure*****/
void Send_save_data();


#endif //OBSLUGA_DANYCH_HPP