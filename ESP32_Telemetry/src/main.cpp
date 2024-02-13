#include <Arduino.h>
#include "pomiary_elektryczne.hpp"
#include "MQTT_Communication.hpp"
#include "Lib_microSD.hpp"

void setup() {
  
  Serial.begin(115200);
  Serial.println("PC Serial connection established");

  establish_mqtt_connection();

  init_ADC();

  init_micro_sd();
}

void loop() {

  if (!MQTT_client.connected()) MQTT_reconnect(); // check if MQTT client is connected
  //TODO: zrobic podobnie dla wifi

  Collect_electrical_data(); //zbierz pomiary ADC z czujnikow
  Send_save_electrical_data(); //wyslij pomiary na serwer
  delay(3000);
}