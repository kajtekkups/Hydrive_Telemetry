#include <Arduino.h>
#include "HallSensor.h"
#include "ElecticalMeasurements.hpp"
#include "MQTT_Communication.hpp"
#include "Lib_microSD.hpp"
#include "GNSS.hpp"
#include "DataManagement.hpp"

void setup() {
  
  Serial.begin(9600);
  Serial.println("PC Serial connection established");

  // establish_mqtt_connection();
  
  // micro_sd_file.begin();

  GPS_init();

  init_ADC();

  // hallSensorInstance.setup();

}

void loop() {

  // if (!MQTT_client.connected()) MQTT_reconnect(); // check if MQTT client is connected

  // if (WiFi.status() != WL_CONNECTED) WIFI_reconnect();
  GPS_update_data();
  
  Collect_data();
  Send_save_data(); //send data to the server

  // hallSensorInstance.loop();
  delay(100);
}