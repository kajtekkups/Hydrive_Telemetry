#include <Arduino.h>
#include "HallSensor.h"
#include "ElecticalMeasurements.hpp"
#include "MQTT_Communication.hpp"
#include "Lib_microSD.hpp"
#include "Position.hpp"
#include "DataManagement.hpp"

void setup() {
  
  Serial.begin(9600);
  Serial.println("PC Serial connection established");

  server_communication_instance.begin();
  
  micro_sd_file.begin();

  electrical_meassurements_instance.begin();
  
  position_instance.begin();

  hall_sensor_instance.begin();
}

void loop() {

  if (!MQTT_client.connected()) server_communication_instance.mqttReconnect(); // check if MQTT client is connected
  if (WiFi.status() != WL_CONNECTED) server_communication_instance.wifiReconnect();

  position_instance.GpsUpdatePosition();
  
  data_management_instance.collectData();
  data_management_instance.saveSendData(); //send data to the server

  hall_sensor_instance.loop();
  
}