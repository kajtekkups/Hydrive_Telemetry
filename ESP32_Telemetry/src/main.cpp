#include <Arduino.h>
#include "pomiary_elektryczne.hpp"
#include "MQTT_Communication.hpp"


void setup() {
  
  //define ESP-PC wire communication
  Serial.begin(115200);
  Serial.println("PC Serial connection established");

  establish_mqtt_connection();

  init_ADC();
}

void loop() {

  if (!MQTT_client.connected()) MQTT_reconnect(); // check if MQTT client is connected
  //TODO: zrobic podobnie dla wifi

  Collect_electrical_data(); //zbierz pomiary ADC z czujnikow
  Send_electrical_data(); //wyslij pomiary na serwer
  delay(3000);
}