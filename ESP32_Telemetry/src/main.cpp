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
  
  micro_sd_file.begin(); //jezeli nie wykryje sd to zapali lampke i nie bedzie blokowac programu

  init_ADC();

  hallSensorInstance.setup();

}

void loop() {

  if (!MQTT_client.connected()) MQTT_reconnect(); // check if MQTT client is connected

  /****************************************************************
   * 1. wyeliminowac blokowanie braku polaczenia z internetem
   * 2. sprawdzic scenariusze testowe
   * ****************************************************************/ 
  if (WiFi.status() != WL_CONNECTED) WIFI_reconnect();

  //TODO: dodać obsługę diod:
    //brak polaczenia z SD
    //brak polaczenia z serwerem MQTT
  
  Collect_data(); //zbierz pomiary 
  Send_save_data(); //wyslij pomiary na serwer

  hallSensorInstance.loop();

  delay(1000);
}