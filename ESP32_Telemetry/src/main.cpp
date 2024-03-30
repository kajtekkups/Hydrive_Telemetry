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
  //jezeli nie wykryje sd to zapali lampke i nie bedzie blokowac programu

  micro_sd_file.begin();

  hallSensorInstance.setup();
}

void loop() {

  // wyeliminowac blokowanie braku polaczenia z internetem
  if (!MQTT_client.connected()) MQTT_reconnect(); // check if MQTT client is connected
  //TODO: zrobic podobnie dla wifi
  //TODO: dodać obsługę diod:
    //brak polaczenia z SD
    //brak polaczenia z serwerem MQTT
  
  Collect_data(); //zbierz pomiary 
  Send_save_data(); //wyslij pomiary na serwer


  hallSensorInstance.loop();

  delay(3000);
}