#include <Arduino.h>
#include "HallSensor.h"
#include "ElecticalMeasurements.hpp"
#include "MQTT_Communication.hpp"
#include "Lib_microSD.hpp"

#include "DataManagement.hpp"

void setup() {
  
  Serial.begin(9600);
  Serial.println("PC Serial connection established");

  // establish_mqtt_connection();
  
  // micro_sd_file.begin(); //jezeli nie wykryje sd to zapali lampke i nie bedzie blokowac programu

  init_ADC();

  // hallSensorInstance.setup();

}

void loop() {

  // if (!MQTT_client.connected()) MQTT_reconnect(); // check if MQTT client is connected

  // if (WiFi.status() != WL_CONNECTED) WIFI_reconnect();
  
  Collect_data(); //zbierz pomiary 
  // Send_save_data(); //wyslij pomiary na serwer

  // hallSensorInstance.loop();
  delay(300);
}