#include <Arduino.h>
#include "HallSensor.h"
#include "pomiary_elektryczne.hpp"
#include "MQTT_Communication.hpp"
#include "Lib_microSD.hpp"

#include "obsluga_danych.hpp"

void setup() {
  
  Serial.begin(9600);
  Serial.println("PC Serial connection established");

  establish_mqtt_connection();

  init_ADC();

  hallSensorInstance.setup();

  init_micro_sd();
}

void loop() {

  if (!MQTT_client.connected()) MQTT_reconnect(); // check if MQTT client is connected
  //TODO: zrobic podobnie dla wifi
  
  Collect_data(); //zbierz pomiary 
  Send_save_data(); //wyslij pomiary na serwer

  hallSensorInstance.loop();

  delay(3000);
}