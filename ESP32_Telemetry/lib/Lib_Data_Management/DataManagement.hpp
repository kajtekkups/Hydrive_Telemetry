#ifndef OBSLUGA_DANYCH_HPP
#define OBSLUGA_DANYCH_HPP

#include "Lib_microSD.hpp"
#include "MQTT_Communication.hpp"
#include "HallSensor.h"
#include "ElecticalMeasurements.hpp"
#include "GNSS.hpp"

struct DataStorage{
  unsigned long time;
  unsigned long measurement_time;
  float voltage_measurement[ADC_number];
  float current_measurement[ADC_number];
  float velocity;

  float longitude;
  float latitude;
  float GPS_speed;
  float course;

  uint16_t in_acceleration_point;
  uint16_t in_control_point;
  uint16_t pwm_width;
};
extern DataStorage measure_data;

float computeVolts(int16_t counts);


/**** collects data and saces it to measure_data structure*****/
void Collect_data();

/**** saves and sends data from measure_data structure*****/
void Send_save_data();


#endif //OBSLUGA_DANYCH_HPP