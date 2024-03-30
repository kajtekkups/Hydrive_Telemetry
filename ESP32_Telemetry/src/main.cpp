#include <Arduino.h>
#include "HallSensor.h"
#include "pomiary_elektryczne.hpp"
#include "MQTT_Communication.hpp"
#include "Lib_microSD.hpp"

HallSensor hallSensorInstance;

void setup() {
  
  Serial.begin(9600);
  Serial.println("PC Serial connection established");

  establish_mqtt_connection();

  init_ADC();

  micro_sd_file.begin();

  hallSensorInstance.setup();

}

void loop() {

  if (!MQTT_client.connected()) MQTT_reconnect(); // check if MQTT client is connected
  //TODO: zrobic podobnie dla wifi

  unsigned long start = millis();
  micro_sd_file.appendFile("test");
  unsigned long stop = millis() - start;
  Serial.println(stop);
  // Collect_electrical_data(); //zbierz pomiary ADC z czujnikow
  // Send_save_electrical_data(); //wyslij pomiary na serwer

  hallSensorInstance.loop();

  delay(3000);
}