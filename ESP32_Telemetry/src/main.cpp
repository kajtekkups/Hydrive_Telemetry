#include <Arduino.h>
#include "HallSensor.h"
#include "ElecticalMeasurements.hpp"
#include "MQTT_Communication.hpp"
#include "Lib_microSD.hpp"
#include "GNSS.hpp"
#include "DataManagement.hpp"
#include "EnergyManagement.hpp"

void setup() {
  
  Serial.begin(9600);
  Serial.println("PC Serial connection established");

  server_communication_instance.begin();
  
  // micro_sd_file.setup();

  electrical_meassurements_instance.begin();

  GPS_init();
  
  // hall_sensor_instance.setup();
}

void loop() {

  if (!MQTT_client.connected()) server_communication_instance.mqttReconnect(); // check if MQTT client is connected

  if (WiFi.status() != WL_CONNECTED) server_communication_instance.wifiReconnect();
  GPS_update_data();

  energy_management_instance.loop(measure_data.latitude, measure_data.longitude);
  
  Collect_data();
  Send_save_data(); //send data to the server

  // hall_sensor_instance.loop();
   delay(100);
}