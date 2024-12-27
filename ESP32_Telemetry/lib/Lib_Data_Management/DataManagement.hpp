#ifndef OBSLUGA_DANYCH_HPP
#define OBSLUGA_DANYCH_HPP

#include "Lib_microSD.hpp"
#include "MQTT_Communication.hpp"
#include "HallSensor.h"
#include "ElecticalMeasurements.hpp"
#include "Position.hpp"
#include "HydrogenFlow.hpp"

struct {
  unsigned long time;
  unsigned long measurement_time;
  float voltage_measurement[ADC_number];
  float current_measurement[ADC_number];
  float velocity;

  float longitude;
  float latitude;
  float GPS_speed;
  float course;

  float flow;

} measure_data;


class DataManagement{
public:
  void begin();
  void saveSendData();    // collects data and saces it to measure_data structure
  void collectData();     // saves and sends data from measure_data structure
};

extern DataManagement data_management_instance;


#endif //OBSLUGA_DANYCH_HPP